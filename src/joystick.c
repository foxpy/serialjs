#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <linux/joystick.h>

#include "config.h"
#include "command.h"

#define UNUSED(var) (void)(var)

void process_button(struct js_event *e, int fd)
{
	static uint8_t s = 0;
	switch (e->number) {
	case A_BTN:
		if (e->value == 0x00) {
			s = (s) ? 0 : 1;
			toggle_cmd(s, fd);
		}
		break;
	case L_BTN:
		if (e->value == 0x01)
			action_cmd(-1, fd);
		else
			action_cmd(0, fd);
		break;
	case R_BTN:
		if (e->value == 0x01)
			action_cmd(1, fd);
		else
			action_cmd(0, fd);
		break;
	case START_BTN:
		if (e->value == 0x01)
			abort_cmd(fd);
		break;
	}

#ifdef DEBUG
	fprintf(stderr, "[%d]:[", e->time);
	switch(e->number) {
	case A_BTN:     fputs("A", stderr);     break;
	case B_BTN:     fputs("B", stderr);     break;
	case X_BTN:     fputs("X", stderr);     break;
	case Y_BTN:     fputs("Y", stderr);     break;
	case L_BTN:     fputs("R", stderr);     break;
	case R_BTN:     fputs("L", stderr);     break;
	case BACK_BTN:  fputs("BACK", stderr);  break;
	case START_BTN: fputs("START", stderr); break;
	case XBOX_BTN:  fputs("XBOX", stderr);  break;
	}
	fprintf(stderr, "] %s.\n", e->value == 0x01 ? "pressed" : "released");
#endif
}

void process_dpad(struct js_event *e, int fd)
{
	UNUSED(fd);
#ifdef DEBUG
	static int8_t x, y; // DPAD state
	const char *direction;

	if      ((e->number == DPAD_Y) && (e->value == DPAD_UP))       y = 1;
	else if ((e->number == DPAD_Y) && (e->value == DPAD_DOWN))     y = -1;
	else if ((e->number == DPAD_Y) && (e->value == DPAD_CENTERED)) y = 0;
	else if ((e->number == DPAD_X) && (e->value == DPAD_RIGHT))    x = 1;
	else if ((e->number == DPAD_X) && (e->value == DPAD_LEFT))     x = -1;
	else if ((e->number == DPAD_X) && (e->value == DPAD_CENTERED)) x = 0;

	if      ((x == 0) && (y == 1))   direction = "UP";
	else if ((x == 1) && (y == 1))   direction = "UPRIGHT";
	else if ((x == 1) && (y == 0))   direction = "RIGHT";
	else if ((x == 1) && (y == -1))  direction = "DOWNRIGHT";
	else if ((x == 0) && (y == -1))  direction = "DOWN";
	else if ((x == -1) && (y == -1)) direction = "DOWNLEFT";
	else if ((x == -1) && (y == 0))  direction = "LEFT";
	else if ((x == -1) && (y == 1))  direction = "UPLEFT";
	else                             direction = "CENTERED";
	fprintf(stderr, "[%d]:[DPAD]: %s\n", e->time, direction);
#else
	UNUSED(e);
#endif
}

void process_stick(struct js_event *e, int fd)
{
	int16_t value;
	static float X = 0.0f, Y = 0.0f;
	if ((abs(e->value) < STICK_MIN_THRESHOLD) &&
			!((e->number == RT) || (e->number == LT)))
		value = 0;
	else
		value = e->value;

	switch (e->number) {
	case LSTICK_X:
		value *= LSTICK_X_MULTIPLIER;
		X = value;
		move_cmd(X, Y, fd);
		break;
	case LSTICK_Y:
		value *= LSTICK_Y_MULTIPLIER;
		Y = value;
		move_cmd(X, Y, fd);
		break;
	case RSTICK_Y:
		value *= RSTICK_X_MULTIPLIER;
		break;
	case RSTICK_X:
		value *= RSTICK_Y_MULTIPLIER;
		break;
	case RT:
		value *= LT_MULTIPLIER;
		break;
	case LT:
		value *= RT_MULTIPLIER;
		break;
	}
#ifdef DEBUG
	fprintf(stderr, "[%d]:[", e->time);
	switch(e->number) {
	case LSTICK_X: fputs("LEFT STICK X", stderr);  break;
	case LSTICK_Y: fputs("LEFT STICK Y", stderr);  break;
	case RSTICK_X: fputs("RIGHT STICK X", stderr); break;
	case RSTICK_Y: fputs("RIGHT STICK Y", stderr); break;
	case RT:       fputs("RIGHT TRIGGER", stderr); break;
	case LT:       fputs("LEFT TRIGGER", stderr);  break;
	}
	fprintf(stderr, "]: %d\n", value);
#endif
}

void process_event(struct js_event *e, int fd)
{
	if (e->type == JS_EVENT_BUTTON) {
		process_button(e, fd);
	} else if (e->type == JS_EVENT_AXIS) {
		if ((e->number == DPAD_X) || (e->number == DPAD_Y)) {
			process_dpad(e, fd);
		} else {
			process_stick(e, fd);
		}
	}
}
