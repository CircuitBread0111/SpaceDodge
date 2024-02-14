CFLAGS=-Wall -Wextra -O2 `sdl2-config --cflags`
LDFLAGS=`sdl2-config --libs` -lSDL2_mixer

SRC=$(wildcard *.c)
OBJ=$(SRC:.c=.o)

sd: $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

.PHONY: clean
clean:
	rm -f $(OBJ) sd
