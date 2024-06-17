#!/bin/sh
# Change your build directory name and executable name here
GAME_NAME="pokexec"
ROOT_DIR=$PWD

# ANSI escape codes for colors
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[0;33m'
BLUE='\033[0;34m'
MAG='\033[0;35m'
CYAN='\033[0;36m'
NC='\033[0m' # No Color

# Stop the script if a compilation (or something else?) fails
set -e

# Get arguments
while getopts ":hcdvsr" opt; do
    case $opt in
        h)
            echo "Usage: ${YELLOW}./build.sh [-hcdvsr]${NC}"
            echo " -h  Show this information"
            echo " -c  Remove the build directory, ie. full recompile"
            echo " -d  Faster builds that have debug symbols, and enable warnings"
            echo " -v  More detailed output from CMake and Make"
            echo " -s  Run strip on the executable after compilation (before -r)"
            echo " -r  Run the executable after compilation"
            echo ""
            echo "Examples:"
            echo " Build a release build:                               ${YELLOW}./build.sh${NC}"
            echo " Build a release build, with Make verbose:            ${YELLOW}./build.sh -v${NC}"
            echo " Build a release build, full recompile, stripped:     ${YELLOW}./build.sh -cs${NC}"
            echo " Build a debug build and run:                         ${YELLOW}./build.sh -dr${NC}"
            echo ""
            echo "You can save everything into a log file by doing:"
            echo " ${YELLOW}./build.sh -<flags> | tee log.txt${NC}"
            echo ""
            exit 0
            ;;
        c)
            BUILD_ALL="1"
            ;;
        d)
            BUILD_DEBUG="1"
            ;;
        v)
            VERBOSE="1"
            ;;
        s)
            STRIP_IT="1"
            ;;
        r)
            RUN_AFTER_BUILD="1"
            ;;
        \?)
            echo "${RED}Invalid option: -$OPTARG${NC}" >&2
            exit 1
            ;;
    esac
done

# Remove the build directory if it exists to do a full recompile
if [ -n "$BUILD_ALL" ]; then
    if [ -d build/debug ] && [ -n "$BUILD_DEBUG" ]; then
        rm -r build/debug
    elif [ -d build/release ]; then
        rm -r build/release
    fi
fi

# Create a build directory and /release /debug if they don't exist yet
if [ ! -d build/debug ] && [ -n "$BUILD_DEBUG" ]; then
    mkdir -p build/debug
elif [ ! -d release ]; then
    mkdir -p build/release
fi

# Display what we're doing and executing CMake
if [ -n "$BUILD_DEBUG" ]; then
    echo "${YELLOW}COMPILE-INFO:${NC} Compiling in debug mode."
    cmake -B build/debug -DCMAKE_BUILD_TYPE=Debug -Wno-dev .
    mv build/debug/compile_commands.json ./     # for clangd (it works on my machine)
    cd build/debug
elif [ ! -d release ]; then
    echo "${YELLOW}COMPILE-INFO:${NC} Compiling in release mode."
    cmake -B build/release -DCMAKE_BUILD_TYPE=Release -Wno-dev .
    mv build/release/compile_commands.json ./   # for clangd (it works on my machine)
    cd build/release
fi

# Execute the final Make command
if [ -n "$VERBOSE" ]; then
    make VERBOSE=1
else
    make
fi

# End of compilation messages
echo "${YELLOW}COMPILE-INFO:${NC} Compilation done \c"
if [ -n "$BUILD_DEBUG" ]; then
    echo "${YELLOW}(debug)"
    echo "COMPILE-INFO:${NC} Game compiled into an executable in: ${YELLOW}$ROOT_DIR/"$BUILD_DIR"debug${NC}"
else
    echo "${YELLOW}(release)"
    echo "COMPILE-INFO:${NC} Game compiled into an executable in: ${YELLOW}$ROOT_DIR/"$BUILD_DIR"release${NC}"
fi

# Strip the executable
if [ -n "$STRIP_IT" ]; then
    strip pokexec
    echo "${YELLOW}COMPILE-INFO:${NC} $GAME_NAME has been stripped."
fi

# Display sizes
lib_size=$(du -sh "libpokaylib.a" | cut -f1 | sed 's/K/ko/' | sed 's/M/Mo/')
exec_size=$(du -sh "pokexec" | cut -f1 | sed 's/K/ko/' | sed 's/M/Mo/')
assets_size=$(du -sh "assets" | cut -f1 | sed 's/M/Mo/' | sed 's/G/Go/')
# prism_size=$(du -sh "assets/pokeprism" | cut -f1 | sed 's/K/ko/' | sed 's/M/Mo/')
# tilesets_size=$(du -sh "assets/tilesets" | cut -f1 | sed 's/K/ko/' | sed 's/M/Mo/')
# sprites_size=$(du -sh "assets/sprites" | cut -f1 | sed 's/K/ko/' | sed 's/M/Mo/')

total_size=$(du -ch "assets" "libpokaylib.a" "pokexec" | grep total$ | cut -f1 | sed 's/K/ko/' | sed 's/M/Mo/' | sed 's/G/Go/')

# Cool ass display box
echo "╔══════════════════════════════════╗"
echo "║ ${BLUE}SIZE-INFO:${NC}                       ║"
printf "║     %-20s ${BLUE}%7s${NC} ║\n" "libpokaylib.a" "$lib_size"
printf "║     %-20s ${BLUE}%7s${NC} ║\n" "$GAME_NAME" "$exec_size"
printf "║     %-20s ${BLUE}%7s${NC} ║\n" "assets/..." "$assets_size"
# printf "║     %-20s ${CYAN}%7s${NC} ║\n" "      /pokeprism/..." "$prism_size"
# printf "║     %-20s ${CYAN}%7s${NC} ║\n" "      /tilesets/..." "$tilesets_size"
# printf "║     %-20s ${CYAN}%7s${NC} ║\n" "      /sprites/..." "$sprites_size"
printf "║     ╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌ ║\n"
printf "║     %-20s ${BLUE}%7s${NC} ║\n" "total" "$total_size"
echo "╚══════════════════════════════════╝"

# Grant execute permission and rename it
chmod +x pokexec
mv pokexec "$GAME_NAME"

# Run the executable after all the build process
if [ -n "$RUN_AFTER_BUILD" ]; then
    ./"$GAME_NAME"
fi

# Go back to the root directory
cd "$ROOT_DIR"
