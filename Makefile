CC = gcc
#CFLAGS = `pkg-config --cflags sdl`\
		-Wall -Wextra -pedantic -Wshadow -Wpointer-arith -Wcast-align \
		-Wwrite-strings -Wmissing-prototypes -Wmissing-declarations \
		-Wredundant-decls -Wnested-externs -Winline -Wno-long-long \
		-Wconversion -Wstrict-prototypes

CFLAGS = `pkg-config --cflags sdl SDL_mixer`-Wall

LIBS = `pkg-config --libs sdl SDL_mixer`

all:
	$(CC) $(CFLAGS) $(LIBS) tunedoodler.c -o tunedoodler
