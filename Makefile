CC = gcc
CFLAGS = -I/mingw64/include/SDL2 -D_REENTRANT
LIBS = -L/mingw64/lib -lmingw32 -lSDL2main -lSDL2 -mwindows

TARGET = cube
SRC = main.c

$(TARGET): $(SRC)
	@echo "Compiling..."
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET).exe $(LIBS)
	@echo "Done!"

clean:
	rm -f $(TARGET).exe

run: $(TARGET)
	./$(TARGET).exe
