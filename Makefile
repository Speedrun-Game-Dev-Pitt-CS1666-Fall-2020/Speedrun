CC = g++ # This is the main compiler

SRCDIR = ./src
BUILDDIR = ./build
TARGET = ./bin/Speedrun

SOURCES = $(wildcard $(SRCDIR)/*.cpp)
#$(warning $(SOURCES))
OBJECTS = $(addprefix $(BUILDDIR)/, $(notdir $(SOURCES:.cpp=.o)))
#$(warning $(OBJECTS))

ifdef OS
    # Windows
    INCLUDE_PATHS = -I ./include
    LIBRARY_PATHS = -L ./lib
    LINKER_FLAGS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_image
else
    # Linux
    INCLUDE_PATHS = -I/usr/include/SDL2
    LIBRARY_PATHS = 
    LINKER_FLAGS = -lSDL2 -lSDL2_image
endif

COMPILER_FLAGS = -w

all : $(OBJS)
	$(CC) $(SOURCES) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(TARGET)