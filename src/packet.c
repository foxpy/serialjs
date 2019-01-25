#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void build_packet(char command, char *args, char *packet) {
	packet[0] = 0xFF; // packet start identification
	packet[1] = command;

	// command arguments
	memcpy(packet+2, args, 8);

	// packet[10] is reserved

	// checksum ??? TODO: Write CRC32
	// (https://en.wikipedia.org/wiki/Cyclic_redundancy_check)
	for (int i = 11; i <= 14; i++) {
		packet[i] = 0x00;
	}

	packet[15] = 0x00; // packet end identification
}

int main(int argc, char *argv[]) {
	char packet[16] = {0, 0, 0, 0, 0, 0, 0, 0,
	                   0, 0, 0, 0, 0, 0, 0, 0};
	float args[2] = {4.0f, -12.5f};
	build_packet(0x10, (char*) &args, (char*) &packet);

	printf("0x");
	for (char i = 0; i <= 15; i++) {
		printf("%02hhx", *(packet+i));
	}
	printf("\n");

	return EXIT_SUCCESS;
}
