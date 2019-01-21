#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/joystick.h>

#include "buttons.h"

char parse_button(struct js_event *e) {
	printf("[%d]:[", e->time);
	switch (e->number) {
		case A_BTN:
			printf("A"); break;
		case B_BTN:
			printf("B"); break;
		case X_BTN:
			printf("X"); break;
		case Y_BTN:
			printf("Y"); break;
		case L_BTN:
			printf("L"); break;
		case R_BTN:
			printf("R"); break;
		case BACK_BTN:
			printf("BACK"); break;
		case START_BTN:
			printf("START"); break;
		case XBOX_BTN:
			printf("XBOX"); break;
	}
	printf("] %s.\n", (e->value == 0x01) ? "pressed" : "released");

	return EXIT_SUCCESS;
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
		if (e.type == JS_EVENT_BUTTON) {
			parse_button(&e);
		} else {
			printf("[%d]: input 0x%08x type of 0x%02x, button: 0x%02x.\n",
					e.time, e.value, e.type, e.number);
		}
	}

	close(fd);
	return EXIT_SUCCESS;
}
