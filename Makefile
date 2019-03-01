CC = clang
TARGETS = src/serialjs.c src/joystick.c src/command.c src/packet.c src/serial.c
CFLAGS = -Wall -Wextra -pedantic -std=gnu99
DFLAGS = -DDEBUG -O0 -g
RFLAGS = -D_FORTIFY_SOURCE=2 -O2

DESTDIR =
PREFIX := /usr/local

debug: $(TARGETS)
	mkdir -p bin/
	$(CC) $(CFLAGS) $(DFLAGS) -o bin/serialjs $(TARGETS)

release: $(TARGETS)
	mkdir -p bin/
	$(CC) $(CFLAGS) $(RFLAGS) -o bin/serialjs $(TARGETS)
	strip bin/serialjs

install: release
	install -Dm755 'bin/serialjs' '$(DESTDIR)$(PREFIX)/bin'

clean:
	rm -rf bin/
