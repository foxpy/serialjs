CC = clang
CFLAGS = -Wall -Wextra -pedantic

debug: src/joystick.c src/command.c src/packet.c
	mkdir -p bin/
	$(CC) $(CFLAGS) -DDEBUG -O0 -g -o bin/joystick.o src/joystick.c src/command.c src/packet.c

release: src/joystick.c src/command.c src/packet.c
	mkdir -p bin/
	$(CC) $(CFLAGS) -D_FORTIFY_SOURCE=2 -O2 -o bin/joystick.o src/joystick.c src/command.c src/packet.c
	strip bin/joystick.o

clean:
	rm -rf bin/
