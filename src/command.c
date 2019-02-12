#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include "packet.h"

#define PACKET_SIZE 16
#define MOVE_CMD 0x01
#define ACTION_CMD 0x11
#define TOGGLE_CMD 0x12

void show_packet(uint8_t *packet) {
	fprintf(stderr, "0x");
	for (uint8_t i = 0; i < PACKET_SIZE; i++) {
		fprintf(stderr, "%02hhx", *(packet+i));
	}
	fprintf(stderr, "\n");
}

void move_cmd(float x, float y) {
	uint8_t packet[PACKET_SIZE];
	float args[2] = {x, y};
	build_packet(MOVE_CMD, (uint8_t*) &args, (uint8_t*) &packet);
	show_packet((uint8_t*) &packet);
}

void action_cmd(int8_t s) {
	uint8_t packet[PACKET_SIZE];
	int8_t args[8] = {s, 0, 0, 0, 0, 0, 0, 0};
	build_packet(ACTION_CMD, (uint8_t*) args, (uint8_t*) &packet);
	show_packet((uint8_t*) &packet);
}

void toggle_cmd(uint8_t s) {
	uint8_t packet[PACKET_SIZE];
	uint8_t args[8] = {s, 0, 0, 0, 0, 0, 0, 0};
	build_packet(TOGGLE_CMD, (uint8_t*) &args, (uint8_t*) &packet);
	show_packet((uint8_t*) &packet);
}
