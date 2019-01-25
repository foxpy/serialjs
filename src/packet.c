#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

void crc32(uint8_t *dst, uint8_t *data, uint32_t len) {
	uint32_t val;
	uint32_t crc = 0xFFFFFFFF;

	while(len--) {
		val = (crc ^ *(data++)) & 0xFF;
		for (char i = 0; i < 8; i++) {
			val = (val & 1) ? (val>>1) ^ 0xEDB88320 : val>>1;
		}
		crc = val ^ crc>>8;
	}
	crc ^= 0xFFFFFFFF;

	memcpy(dst, &crc, sizeof(crc));
}

void build_packet(char command, char *args, char *packet) {
	packet[0] = 0xFF; // packet start identification
	packet[1] = command;

	// command arguments
	memcpy(packet+2, args, 8);

	// packet[10] is reserved

	// checksum
	crc32((uint8_t*) packet+11, (uint8_t*) packet, 10);

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
