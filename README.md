# Dependencies

In order for the project to build, you must have your environment configured correctly.

## Linux

Linux users will need the default installation of SDL2 and some of its extensions, obtained via:

1. `sudo apt-get install libsdl2-dev`
2. `sudo apt-get install libsdl2-image-dev`
3. `sudo apt-get install libsdl2-ttf-dev`

and...

## Windows

Windows users will need three folders not included in this repository: `bin`, `lib`, and `include`. These three folders contain the necessary files for MinGW and SDL2.

`bin` contains the relevant dlls (and .exe), `lib` contains the relevant libraries, and `include` contains the relevant header files. The three folders can be obtained in all their glory [here](https://drive.google.com/file/d/1ct0FDnnhdu88gBQrrQQw1WwjKjYjrwKp/view?usp=sharing). Place these three folders in the root directory (alongside the likes of `src`).


# Building

**Make sure you run the executable from inside the `bin` directory!** If you run it from somewhere else, your images may not load (the image paths will be wrong from that location)!

## Linux

Linux users simply run `make`. If you do not have a `bin` folder, the Makefile will create one for you and store the executable in there.

## Windows

Windows users will need to run `mingw32-make.exe` provided by MinGW. The executable will appear in `bin`.
