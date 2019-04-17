#include <stdint.h>

#include "packet.h"
#include "command.h"


void move_cmd(float x, float y)
{
	uint8_t packet[PACKET_SIZE];
	float args[2] = {x, y};
	build_packet(MOVE_CMD, (uint8_t*) &args, (uint8_t*) &packet);
	write_packet((uint8_t*) &packet);
}

void action_cmd(int8_t s)
{
	uint8_t packet[PACKET_SIZE];
	int8_t args[8] = {s, 0, 0, 0, 0, 0, 0, 0};
	build_packet(ACTION_CMD, (uint8_t*) args, (uint8_t*) &packet);
	write_packet((uint8_t*) &packet);
}

void toggle_cmd(uint8_t s)
{
	uint8_t packet[PACKET_SIZE];
	uint8_t args[8] = {s, 0, 0, 0, 0, 0, 0, 0};
	build_packet(TOGGLE_CMD, (uint8_t*) &args, (uint8_t*) &packet);
	write_packet((uint8_t*) &packet);
}

void abort_cmd()
{
	uint8_t packet[PACKET_SIZE];
	uint8_t args[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	build_packet(ABORT_CMD, (uint8_t*) &args, (uint8_t*) &packet);
	write_packet((uint8_t*) &packet);
}
