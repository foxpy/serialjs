CC = clang
TARGETS = src/joystick.c src/command.c src/packet.c
CFLAGS = -Wall -Wextra -pedantic
DFLAGS = -DDEBUG -O0 -g
RFLAGS = -D_FORTIFY_SOURCE=2 -O2

debug: $(TARGETS)
	mkdir -p bin/
	$(CC) $(CFLAGS) $(DFLAGS) -o bin/joystick.o $(TARGETS)

release: $(TARGETS)
	mkdir -p bin/
	$(CC) $(CFLAGS) $(RFLAGS) -o bin/joystick.o $(TARGETS)
	strip bin/joystick.o

clean:
	rm -rf bin/
