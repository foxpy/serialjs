#include <stdio.h>
#include <string.h>

char build_packet(char command, char *args, char *packet) {
	packet[0] = 0xFF; // packet start identification
	packet[1] = command;

	// command arguments
	memcpy(packet+2, args, 12);

	// checksum ??? TODO: Write CRC16
	// (https://en.wikipedia.org/wiki/Cyclic_redundancy_check)
	packet[14] = 0x00;
	packet[15] = 0x00;

	// packet[16-30] is reserved

	packet[31] = 0x00; // packet end identification
	return 0;
}

int main(int argc, char *argv[]) {
	char packet[32] = {0, 0, 0, 0, 0, 0, 0, 0,
	                   0, 0, 0, 0, 0, 0, 0, 0,
	                   0, 0, 0, 0, 0, 0, 0, 0,
	                   0, 0, 0, 0, 0, 0, 0, 0};
	float args[3] = {4.0f, 2.0f, -12.5f};
	build_packet(0x10, (char*) &args, (char*) &packet);

	printf("0x");
	for (char i = 0; i <= 31; i++) {
		printf("%02hhx", *(packet+i));
	}
	printf("\n");

	return 0;
}
