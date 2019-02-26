CC = clang
TARGETS = src/main.c src/joystick.c src/command.c src/packet.c src/serial.c
CFLAGS = -Wall -Wextra -pedantic -std=gnu99
DFLAGS = -DDEBUG -O0 -g
RFLAGS = -D_FORTIFY_SOURCE=2 -O2

debug: $(TARGETS)
	mkdir -p bin/
	$(CC) $(CFLAGS) $(DFLAGS) -o bin/joystick $(TARGETS)

release: $(TARGETS)
	mkdir -p bin/
	$(CC) $(CFLAGS) $(RFLAGS) -o bin/joystick $(TARGETS)
	strip bin/joystick

clean:
	rm -rf bin/
