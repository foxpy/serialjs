#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

void crc32(uint8_t *dst, uint8_t *data, uint32_t len)
{
	uint32_t val;
	uint32_t crc = 0xFFFFFFFF;

	while(len--) {
		val = (crc ^ *(data++)) & 0xFF;
		for (char i = 0; i < 8; i++)
			val = (val & 1) ? (val >> 1) ^ 0xEDB88320 : val >> 1;
		crc = val ^ crc >> 8;
	}
	crc ^= 0xFFFFFFFF;

	memcpy(dst, &crc, sizeof(crc));
}

void build_packet(char command, char *args, char *packet)
{
	packet[0] = 0xFF; // packet start identification
	packet[1] = command;

	// command arguments
	memcpy(&packet[2], args, 8);

	// reserved
	packet[10] = 0x00;

	// checksum
	crc32((uint8_t*) &packet[11], (uint8_t*) packet, 10);

	packet[15] = 0x00; // packet end identification
}
