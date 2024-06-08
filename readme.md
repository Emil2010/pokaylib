# pokaylib
This project aims to be a Pokemon "engine" built in C with [raylib](https://github.com/raysan5/raylib).

## Structure
pokaylib's organization is such that:
- `src/` is where all `*.c` files live
    - `main.c` contains the game, where the gameloop... loops
    - `pokaylib.c` pokaylib's functions implementations
- `include/` is where all `*.h` files live
    - `pokaylib.h` pokaylib's functions declarations
    - `abilities.h` enum list
    - `moves.h` enum list
    - `pokemons.h` enum list
- `assets/` is where all sprites, sounds, dialogues and map data are stored

## Build
The build process compiles `raylib-5.0` and `pokaylib` with CMake.
The script `build.sh` makes it easier if you are not using an IDE and provides some other information.
```
Usage: ./build.sh [-hcdvsr]
 -h  Show this information
 -c  Remove the build directory, ie. full recompile
 -d  Faster builds that have debug symbols, and enable warnings
 -v  More detailed output from the final make command
 -s  Run strip on the executable after compilation (before -r)
 -r  Run the executable after compilation
```

Example:
```
~$> cd pokaylib
~$> ./build.sh -d
```
Will create a `build` directory in which the final executable will be compiled.
You can change the build directory name in the `build.sh` script, as well as the final executable name (default is `pokexec`).
> TODO: a `build.ps1` for Windows.

## Additional credits
The sprites found in `assets/pokeprism` are taken from the [Pokemon Prism](https://rainbowdevs.com/title/prism/) fangame. The `assets` folder is not well organized right now, this will change in the future.
