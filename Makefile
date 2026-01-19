TARGET = tictactoe.exe


SDL_PATH = ../SDL_assets/x86_64-w64-mingw32


CC = gcc


LDFLAGS = -L$(SDL_PATH)/lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_image

# Pliki źródłowe
SRCS = tictactoe.c
OBJS = $(SRCS:.c=.o)

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

run: all
	./$(TARGET)

clean:
	del /q *.o $(TARGET)