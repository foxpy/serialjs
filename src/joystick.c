#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/joystick.h>

#include "buttons.h"

void parse_button(struct js_event *e) {
	printf("[%d]:[", e->time);
	switch (e->number) {
		case A_BTN: printf("A"); break;
		case B_BTN: printf("B"); break;
		case X_BTN: printf("X"); break;
		case Y_BTN: printf("Y"); break;
		case L_BTN: printf("L"); break;
		case R_BTN: printf("R"); break;
		case BACK_BTN: printf("BACK"); break;
		case START_BTN: printf("START"); break;
		case XBOX_BTN: printf("XBOX"); break;
	}
	printf("] %s.\n", (e->value == 0x01) ? "pressed" : "released");
}

void parse_dpad(struct js_event *e) {
	printf("[%d]:[", e->time);
	if ((e->number == DPAD_Y) && (e->value == DPAD_UP)) {
		printf("DPAD UP");
	} else if ((e->number == DPAD_Y) && (e->value == DPAD_DOWN)) {
		printf("DPAD DOWN");
	} else if ((e->number == DPAD_Y) && (e->value == 0)) {
		printf("DPAD Y ZERO");
	} else if ((e->number == DPAD_X) && (e->value == DPAD_RIGHT)) {
		printf("DPAD RIGHT");
	} else if ((e->number == DPAD_X) && (e->value == DPAD_LEFT)) {
		printf("DPAD LEFT");
	} else if ((e->number == DPAD_X) && (e->value == 0)) {
		printf("DPAD X ZERO");
	}
	printf("]\n");
}

void parse_stick(struct js_event *e) {
	if ((e->number == DPAD_X) || (e->number == DPAD_Y)) {
		parse_dpad(e);
		return;
	}
	printf("[%d]:[", e->time);
	switch (e->number) {
	}
	printf("] %d.\n", e->value);
}

void log_event(struct js_event *e) {
	printf("[%d]: input 0x%08x type of 0x%02x, button: 0x%02x.\n",
			e->time, e->value, e->type, e->number);
}


int main(int argc, char *argv[]) {
	char* jsfile;
	if (argc == 1) {
		jsfile = "/dev/input/js0";
	} else {
		jsfile = argv[1];
	}

	int fd = open(jsfile, O_RDONLY);
	if (fd < 0) {
		puts("Can't open file.");
		return EXIT_FAILURE;
	}
	struct js_event e;

	while (read(fd, &e, sizeof(e)) == sizeof(e)) {
		switch (e.type) {
			case JS_EVENT_BUTTON: parse_button(&e); break;
			case JS_EVENT_AXIS: parse_stick(&e); break;
			default: log_event(&e); break;
		}
	}

	close(fd);
	return EXIT_SUCCESS;
}
