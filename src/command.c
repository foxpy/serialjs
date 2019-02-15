#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "packet.h"

#define PACKET_SIZE 16
#define MOVE_CMD 0x01
#define ACTION_CMD 0x11
#define TOGGLE_CMD 0x12


void write_packet(uint8_t *packet, int fd)
{
#ifdef DEBUG
	printf("0x");
	for (uint8_t i = 0; i < PACKET_SIZE; i++) {
		printf("%02hhx", *(packet+i));
	}
	printf("\n");
#endif
	if (write(fd, packet, PACKET_SIZE) == -1) {
		perror("write");
		exit(EXIT_FAILURE);
	}
}

void move_cmd(float x, float y, int fd)
{
	uint8_t packet[PACKET_SIZE];
	float args[2] = {x, y};
	build_packet(MOVE_CMD, (uint8_t*) &args, (uint8_t*) &packet);
	write_packet((uint8_t*) &packet, fd);
}

void action_cmd(int8_t s, int fd)
{
	uint8_t packet[PACKET_SIZE];
	int8_t args[8] = {s, 0, 0, 0, 0, 0, 0, 0};
	build_packet(ACTION_CMD, (uint8_t*) args, (uint8_t*) &packet);
	write_packet((uint8_t*) &packet, fd);
}

void toggle_cmd(uint8_t s, int fd)
{
	uint8_t packet[PACKET_SIZE];
	uint8_t args[8] = {s, 0, 0, 0, 0, 0, 0, 0};
	build_packet(TOGGLE_CMD, (uint8_t*) &args, (uint8_t*) &packet);
	write_packet((uint8_t*) &packet, fd);
}
