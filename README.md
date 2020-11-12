# Requirements

You must be using Linux.

# Dependencies

In order for the project to build, you must have your environment configured correctly.

Users will need the default installation of SDL2 and some of its extensions, obtained via:

1. `apt-cache search libsdl2`
2. `sudo apt-get install libsdl2-dev`

and...

1. `apt-cache search libsdl2-image`
2. `sudo apt-get install libsdl2-image-dev`

# Building

**Make sure you run the executable from inside the `bin` directory!** If you run it from somewhere else, your images may not load (the image paths will be wrong from that location)!

Simply run `make -f Makefile_client` or `make -f Makefile_server`. If you do not have a `bin` and `build` folder, then you must create them.
