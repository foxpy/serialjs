CC = clang
CFLAGS = -Wall -Wextra -pedantic

debug: src/joystick.c
	mkdir -p bin/
	$(CC) $(CFLAGS) -O0 -g -o bin/joystick.o src/joystick.c

release: src/joystick.c
	mkdir -p bin/
	$(CC) $(CFLAGS) -D_FORTIFY_SOURCE=2 -O2 -o bin/joystick.o src/joystick.c
	strip bin/joystick.o

clean:
	rm -rf bin/
