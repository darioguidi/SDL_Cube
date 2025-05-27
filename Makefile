# Detect OS
OS := $(shell uname -s)

# Compiler
CC = gcc

# Common settings
SRC = main.c
TARGET = cube

# Platform-specific settings
ifeq ($(OS), Linux)
    CFLAGS = `sdl2-config --cflags`
    LIBS = `sdl2-config --libs` -lm
    EXE = $(TARGET)
else
    # Assume MinGW on Windows
    CFLAGS = -I/mingw64/include/SDL2 -D_REENTRANT
    LIBS = -L/mingw64/lib -lmingw32 -lSDL2main -lSDL2 -mwindows
    EXE = $(TARGET).exe
endif

# Build rule
$(TARGET): $(SRC)
	@echo "Compiling for $(OS)..."
	$(CC) $(CFLAGS) $(SRC) -o $(EXE) $(LIBS)
	@echo "Done!"

# Clean rule
clean:
	rm -f $(TARGET) $(TARGET).exe

# Run rule (Linux assumes ./cube, Windows will also use ./cube.exe)
run: $(TARGET)
	./$(EXE)
