OBJ = main Screen SimplexNoise XorShifter
OBJS = $(foreach d, $(OBJ), $d.cpp)
# Speedrun\$d.cpp for outside this folder...

#CC specifies which compiler we're using
CC = g++

#INCLUDE_PATHS specifies the additional include paths we'll need
#LIBRARY_PATHS specifies the additional library paths we'll need
#LINKER_FLAGS specifies the libraries we're linking against
ifdef OS
    # Windows
    INCLUDE_PATHS = -IC:\Dev\SDL2\include\SDL2 -IC:\Dev\SDL2_image\include\SDL2
    LIBRARY_PATHS = -LC:\Dev\MinGW\lib -LC:\Dev\SDL2_image\lib
    LINKER_FLAGS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_image
else
    # Linux
    INCLUDE_PATHS = -I/usr/include/SDL2
    LIBRARY_PATHS = -L/usr/lib/x86_64-linux-gnu
    LINKER_FLAGS = -lSDL2main -lSDL2 -lSDL2_image
endif

#COMPILER_FLAGS specifies the additional compilation options we're using
# -w suppresses all warnings
# -Wl,-subsystem,windows gets rid of the console window
COMPILER_FLAGS = -w

#OBJ_NAME specifies the name of our exectuable
OBJ_NAME = Speedrun

#This is the target that compiles our executable
all : $(OBJS)
    $(CC) $(OBJS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)
