#include <stdint.h>

#include "packet.h"
#include "command.h"


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

void abort_cmd(int fd)
{
	uint8_t packet[PACKET_SIZE];
	uint8_t args[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	build_packet(ABORT_CMD, (uint8_t*) &args, (uint8_t*) &packet);
	write_packet((uint8_t*) &packet, fd);
}
