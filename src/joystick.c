#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/joystick.h>

#include "config.h"

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
	static char x, y; // DPAD state
	printf("[%d]:[DPAD]: ", e->time);

	if ((e->number == DPAD_Y) && (e->value == DPAD_UP)) {y = 1;}
	else if ((e->number == DPAD_Y) && (e->value == DPAD_DOWN)) {y = -1;}
	else if ((e->number == DPAD_Y) && (e->value == DPAD_CENTERED)) {y = 0;}
	else if ((e->number == DPAD_X) && (e->value == DPAD_RIGHT)) {x = 1;}
	else if ((e->number == DPAD_X) && (e->value == DPAD_LEFT)) {x = -1;}
	else if ((e->number == DPAD_X) && (e->value == DPAD_CENTERED)) {x = 0;}

	if ((x == 0) && (y == 1)) { printf("UP"); }
	else if ((x == 1) && (y == 1)) { printf("UPRIGHT"); }
	else if ((x == 1) && (y == 0)) { printf("RIGHT"); }
	else if ((x == 1) && (y == -1)) { printf("DOWNRIGHT"); }
	else if ((x == 0) && (y == -1)) { printf("DOWN"); }
	else if ((x == -1) && (y == -1)) { printf("DOWNLEFT"); }
	else if ((x == -1) && (y == 0)) { printf("LEFT"); }
	else if ((x == -1) && (y == 1)) { printf("UPLEFT"); }
	else printf("CENTERED");
	printf("\n");
}

void parse_stick(struct js_event *e) {
	int16_t value;
	if ((abs(e->value) < STICK_MIN_THRESHOLD) &&
			! ((e->number == RT) || (e->number == LT))) {
		value = 0;
	} else {
		value = e->value;
	}

	printf("[%d]:[", e->time);
	switch (e->number) {
		case LSTICK_X:
			value *= LSTICK_X_MULTIPLIER;
			printf("LEFT STICK X"); break;
		case LSTICK_Y:
			value *= LSTICK_Y_MULTIPLIER;
			printf("LEFT STICK Y"); break;
		case RSTICK_Y:
			value *= RSTICK_X_MULTIPLIER;
			printf("RIGHT STICK X"); break;
		case RSTICK_X:
			value *= RSTICK_Y_MULTIPLIER;
			printf("RIGHT STICK Y"); break;
		case RT:
			value *= LT_MULTIPLIER;
			printf("RIGHT TRIGGER"); break;
		case LT:
			value *= RT_MULTIPLIER;
			printf("LEFT TRIGGER"); break;
	}
	printf("]: %d\n", value);
}

void parse_event(struct js_event *e) {
	if (e->type == JS_EVENT_BUTTON) {
		parse_button(e);
	} else if (e->type == JS_EVENT_AXIS) {
		if ((e->number == DPAD_X) || (e->number == DPAD_Y)) {
			parse_dpad(e);
		} else {
			parse_stick(e);
		}
	}
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

	char number_of_axes, number_of_buttons;
	char gamepad_name[64];
	ioctl(fd, JSIOCGAXES, &number_of_axes);
	ioctl(fd, JSIOCGBUTTONS, &number_of_buttons);
	ioctl(fd, JSIOCGNAME(sizeof(gamepad_name)), &gamepad_name);
	printf("Listening for %s with %d axes and %d buttons.\n",
			gamepad_name, number_of_axes, number_of_buttons);

	struct js_event e;
	while (read(fd, &e, sizeof(e)) == sizeof(e)) {
		parse_event(&e);
	}

	close(fd);
	return EXIT_SUCCESS;
}
