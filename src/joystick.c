#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <linux/joystick.h>

#include "config.h"
#include "command.h"

void parse_button(struct js_event *e) {
	static uint8_t s = 0;
	static char* button;
	switch (e->number) {
		case A_BTN:
			button = "A";
			if (e->value == 0x00) {
				s = (s) ? 0 : 1;
				toggle_cmd(s);
			}
			break;
		case B_BTN: button = "B"; break;
		case X_BTN: button = "X"; break;
		case Y_BTN: button = "Y"; break;
		case L_BTN:
			button = "L";
			if (e->value == 0x01) {
				action_cmd(-1);
			} else {
				action_cmd(0);
			}
			break;
		case R_BTN:
			button = "R";
			if (e->value == 0x01) {
				action_cmd(1);
			} else {
				action_cmd(0);
			}
			break;
		case BACK_BTN: button = "BACK"; break;
		case START_BTN: button = "START"; break;
		case XBOX_BTN: button = "XBOX"; break;
	}
#ifdef DEBUG
	fprintf(stderr, "[%d]:[%s] %s.\n", e->time, button,
			(e->value == 0x01) ? "pressed" : "released");
#endif
}

void parse_dpad(struct js_event *e) {
	static char x, y; // DPAD state
	static char* direction;

	if ((e->number == DPAD_Y) && (e->value == DPAD_UP)) {y = 1;}
	else if ((e->number == DPAD_Y) && (e->value == DPAD_DOWN)) {y = -1;}
	else if ((e->number == DPAD_Y) && (e->value == DPAD_CENTERED)) {y = 0;}
	else if ((e->number == DPAD_X) && (e->value == DPAD_RIGHT)) {x = 1;}
	else if ((e->number == DPAD_X) && (e->value == DPAD_LEFT)) {x = -1;}
	else if ((e->number == DPAD_X) && (e->value == DPAD_CENTERED)) {x = 0;}

	if ((x == 0) && (y == 1))        { direction = "UP"; }
	else if ((x == 1) && (y == 1))   { direction = "UPRIGHT"; }
	else if ((x == 1) && (y == 0))   { direction = "RIGHT"; }
	else if ((x == 1) && (y == -1))  { direction = "DOWNRIGHT"; }
	else if ((x == 0) && (y == -1))  { direction = "DOWN"; }
	else if ((x == -1) && (y == -1)) { direction = "DOWNLEFT"; }
	else if ((x == -1) && (y == 0))  { direction = "LEFT"; }
	else if ((x == -1) && (y == 1))  { direction = "UPLEFT"; }
	else direction = "CENTERED";
#ifdef DEBUG
	fprintf(stderr, "[%d]:[DPAD]: %s\n", e->time, direction);
#endif
}

void parse_stick(struct js_event *e) {
	static float X = 0.0f, Y = 0.0f;
	static char *stick;
	int16_t value;
	if ((abs(e->value) < STICK_MIN_THRESHOLD) &&
			! ((e->number == RT) || (e->number == LT))) {
		value = 0;
	} else {
		value = e->value;
	}

	switch (e->number) {
		case LSTICK_X:
			value *= LSTICK_X_MULTIPLIER;
			X = value;
			move_cmd(X, Y);
			stick = "LEFT STICK X"; break;
		case LSTICK_Y:
			value *= LSTICK_Y_MULTIPLIER;
			Y = value;
			move_cmd(X, Y);
			stick = "LEFT STICK Y"; break;
		case RSTICK_Y:
			value *= RSTICK_X_MULTIPLIER;
			stick = "RIGHT STICK X"; break;
		case RSTICK_X:
			value *= RSTICK_Y_MULTIPLIER;
			stick = "RIGHT STICK Y"; break;
		case RT:
			value *= LT_MULTIPLIER;
			stick = "RIGHT TRIGGER"; break;
		case LT:
			value *= RT_MULTIPLIER;
			stick = "LEFT TRIGGER"; break;
	}
#ifdef DEBUG
	fprintf(stderr, "[%d]:[%s]: %d\n", e->time, stick, value);
#endif
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
		fprintf(stderr, "Can't open file: %s.\n", jsfile);
		return EXIT_FAILURE;
	}

	char number_of_axes, number_of_buttons;
	char gamepad_name[64];
	struct stat sb;
	fstat(fd, &sb);
	if (! S_ISCHR(sb.st_mode)) {
		fprintf(stderr, "Warning: %s is not a character file, "
				"is it even a joystick?\n", jsfile);
	} else {
		ioctl(fd, JSIOCGAXES, &number_of_axes);
		ioctl(fd, JSIOCGBUTTONS, &number_of_buttons);
		ioctl(fd, JSIOCGNAME(sizeof(gamepad_name)), &gamepad_name);
		printf("Listening for %s with %d axes and %d buttons.\n",
				gamepad_name,
				number_of_axes,
				number_of_buttons);
	}
	struct js_event e;
	while (read(fd, &e, sizeof(e)) == sizeof(e)) {
		parse_event(&e);
	}

	close(fd);
	return EXIT_SUCCESS;
}
