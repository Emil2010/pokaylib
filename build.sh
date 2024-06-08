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
            echo "Usage: ./build.sh [-hcdvsr]"
            echo " -h  Show this information"
            echo " -c  Remove the build directory, ie. full recompile"
            echo " -d  Faster builds that have debug symbols, and enable warnings"
            echo " -v  More detailed output from the final make command"
            echo " -s  Run strip on the executable after compilation (before -r)"
            echo " -r  Run the executable after compilation"
            echo ""
            echo "Examples:"
            echo " Build a release build:                               ./build.sh"
            echo " Build a release build, with make verbose:            ./build.sh -v"
            echo " Build a release build, full recompile, stripped :    ./build.sh -cs"
            echo " Build a debug build and run:                         ./build.sh -dr"
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
            echo "Invalid option: -$OPTARG" >&2
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
    cmake -DCMAKE_BUILD_TYPE=Debug ..
else
    echo "${YELLOW}COMPILE-INFO:${NC} Compiling in release mode."
    cmake -DCMAKE_BUILD_TYPE=Release ..
fi

# Execute the final make command
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
echo "${YELLOW}COMPILE-INFO:${NC} Game compiled into an executable in: ${RED}$ROOT_DIR/$BUILD_DIR/${NC}"

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
