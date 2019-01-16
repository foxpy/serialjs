CC = clang
CFLAGS = -Wall

debug: src/packet.c
	mkdir -p bin/
	$(CC) $(CFLAGS) -g -o bin/packet.o src/packet.c

release: src/packet.c
	mkdir -p bin/
	$(CC) $(CFLAGS) -o bin/packet.o src/packet.c
	strip bin/packet.o

clean:
	rm -rf bin/
