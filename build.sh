#!/bin/sh
# Change your build directory name and executable name here
BUILD_DIR="build"
GAME_NAME="pokexec"
ROOT_DIR=$PWD

# ANSI escape codes for colors
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[0;33m'
BLUE='\033[0;34m'
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
if [ -d "$BUILD_DIR" ] && [ -n "$BUILD_ALL" ]; then
    rm -r "$BUILD_DIR"
fi

# Create a build directory if it does not exist yet
if [ ! -d "$BUILD_DIR" ]; then
    mkdir $BUILD_DIR
fi
cd $BUILD_DIR

# Display what we're doing and executing cmake
if [ -n "$BUILD_DEBUG" ]; then
    echo "${YELLOW}COMPILE-INFO:${NC} Compiling in debug mode."
    cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ..
else
    echo "${YELLOW}COMPILE-INFO:${NC} Compiling in release mode."
    cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ..
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
    echo "(debug)."
else
    echo "(release)."
fi
echo "${YELLOW}COMPILE-INFO:${NC} Game compiled into an executable in: ${YELLOW}$ROOT_DIR/$BUILD_DIR/${NC}"

# Strip the executable
if [ -n "$STRIP_IT" ]; then
    strip pokexec
    echo "${YELLOW}COMPILE-INFO:${NC} $GAME_NAME has been stripped."
fi

# Display sizes
lib_size=$(du -sh "libpokaylib.a" | cut -f1 | sed 's/K/ko/' | sed 's/M/Mo/')
exec_size=$(du -sh "pokexec" | cut -f1 | sed 's/K/ko/' | sed 's/M/Mo/')
assets_size=$(du -sh "assets" | cut -f1 | sed 's/M/Mo/' | sed 's/G/Go/')

max_width=13
game_name_length=${#GAME_NAME}
if (( game_name_length > max_width )); then
    max_width=$game_name_length
fi

echo "╔══════════════════════════════════╗"
printf "║ ${BLUE}%s${NC} %-${max_width}s %7s ║\n" "SIZE-INFO:" "libpokaylib.a" "$lib_size"
printf "║ ${BLUE}%s${NC} %-${max_width}s %7s ║\n" "SIZE-INFO:" "$GAME_NAME" "$exec_size"
printf "║ ${BLUE}%s${NC} %-${max_width}s %7s ║\n" "SIZE-INFO:" "assets/..." "$assets_size"
echo "╚══════════════════════════════════╝"

# Grant execute permission and rename it
chmod +x pokexec
mv pokexec "$GAME_NAME"

# Run the executable after all the build process
if [ -n "$RUN_AFTER_BUILD" ]; then
    ./"$GAME_NAME"
fi

# Go back to the root directory
cd ..
