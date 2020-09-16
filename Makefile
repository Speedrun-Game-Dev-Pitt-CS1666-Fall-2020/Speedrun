#OBJS specifies which files to compile as part of the project
OBJ = main Screen SimplexNoise XorShifter
OBJS = $(foreach d, $(OBJ), Speedrun\$d.cpp)

#CC specifies which compiler we're using
CC = g++

#INCLUDE_PATHS specifies the additional include paths we'll need
INCLUDE_PATHS = -IC:\Dev\SDL2\include\SDL2 -I"deps\include"
# We might as well use relative paths for our stuff

#LIBRARY_PATHS specifies the additional library paths we'll need
LIBRARY_PATHS = -LC:\Dev\MinGW\lib

#COMPILER_FLAGS specifies the additional compilation options we're using
# -w suppresses all warnings
# -Wl,-subsystem,windows gets rid of the console window
COMPILER_FLAGS = -w
# -Wl,-subsystem,windows

#LINKER_FLAGS specifies the libraries we're linking against
LINKER_FLAGS = -lmingw32 -lSDL2main -lSDL2

#OBJ_NAME specifies the name of our exectuable
OBJ_NAME = Speedrun

#This is the target that compiles our executable
all : $(OBJS)
	$(CC) $(OBJS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)