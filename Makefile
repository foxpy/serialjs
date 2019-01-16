CC = clang
CFLAGS = -Wall

debug: src/packet.c src/joystick.c
	mkdir -p bin/
	$(CC) $(CFLAGS) -g -o bin/packet.o src/packet.c
	$(CC) $(CFLAGS) -g -o bin/joystick.o src/joystick.c

release: src/packet.c src/joystick.c
	mkdir -p bin/
	$(CC) $(CFLAGS) -o bin/packet.o src/packet.c
	$(CC) $(CFLAGS) -o bin/joystick.o src/joystick.c
	strip bin/packet.o bin/joystick.o

clean:
	rm -rf bin/
