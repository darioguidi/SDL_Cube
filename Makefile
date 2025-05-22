# Compilatore
CC = gcc

CFLAGS = -Wall -std=c99

# Librerie
LIBS = -lSDL2 -lm

TARGET = cube
SRC = main.c

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC) $(LIBS)

clean:
	rm -f $(TARGET)

run: $(TARGET)
	./$(TARGET)