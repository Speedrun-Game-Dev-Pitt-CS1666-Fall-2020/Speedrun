# Requirements

You must be using linux.

# Dependencies

In order for the project to build, you must have your environment configured correctly.

Linux users will need the default installation of SDL2 and some of its extensions, obtained via:

1. `sudo apt-get install libsdl2-dev`
2. `sudo apt-get install libsdl2-image-dev`
3. `sudo apt-get install libsdl2-ttf-dev`

# Building

**Make sure you run the executable from inside the `bin` directory!** If you run it from somewhere else, your images may not load (the image paths will be wrong from that location)!

Simply run `make_client` or `make_server`. If you do not have a `bin` folder, the Makefile will create one for you and store the executable in there.
