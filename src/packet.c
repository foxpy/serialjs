#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "packet.h"

void crc32(uint32_t *dst, void *src, size_t len)
{
	uint8_t *data = src;
	uint32_t val, crc = 0xFFFFFFFF;

	for(size_t i = 0; i < len; i++) {
		val = (crc ^ data[i]) & 0xFF;
		for (uint8_t i = 0; i < 8; i++)
			val = (val & 1) ? (val >> 1) ^ 0xEDB88320 : val >> 1;
		crc = val ^ crc >> 8;
	}
	crc ^= 0xFFFFFFFF;

	*dst = crc;
}

void build_packet(uint8_t command, uint8_t *args, uint8_t *packet)
{
	packet[0] = 0xFF; // packet start identification
	packet[1] = command;

	// command arguments
	memcpy(&packet[2], args, 8);

	// reserved
	packet[10] = 0x00;

	// checksum
	crc32((uint32_t*) &packet[11], packet, 10);

	packet[15] = 0x00; // packet end identification
}

void write_packet(uint8_t *packet, int fd)
{
#ifdef DEBUG
        printf("0x");
        for (uint8_t i = 0; i < PACKET_SIZE; i++) {
                printf("%02hhx", packet[i]);
        }
        printf("\n");
#endif
        if (write(fd, packet, PACKET_SIZE) == -1) {
                perror("write");
                exit(EXIT_FAILURE);
        }
}
