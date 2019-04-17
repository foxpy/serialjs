#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <linux/joystick.h>

#include "config.h"
#include "packet.h"
#include "serial.h"
#include "joystick.h"

void log_js_data(int js_fd, char *jsfile)
{
	uint8_t number_of_axes, number_of_buttons;
	char gamepad_name[64];
	struct stat sb;
	fstat(js_fd, &sb);
	if (!S_ISCHR(sb.st_mode)) {
		fprintf(stderr, "Warning: %s is not a character file, "
		        "is it even a joystick?\n", jsfile);
	} else {
		ioctl(js_fd, JSIOCGAXES, &number_of_axes);
		ioctl(js_fd, JSIOCGBUTTONS, &number_of_buttons);
		ioctl(js_fd, JSIOCGNAME(sizeof(gamepad_name)), &gamepad_name);
		printf("Listening for %s with %d axes and %d buttons.\n",
		       gamepad_name,
		       number_of_axes,
		       number_of_buttons);
	}
}

int main(int argc, char *argv[])
{
	char *jsfile, *acmfile;
	if (argc >= 2)
		jsfile = argv[1];
	else
		jsfile = JSFILE_DEFAULT;

	if (argc >= 3)
		acmfile = argv[2];
	else
		acmfile = ACMFILE_DEFAULT;

	int js_fd = open(jsfile, O_RDONLY);
	if (js_fd < 0) {
		fprintf(stderr, "Can't open file: %s (%s).\n",
			jsfile, strerror(errno));
		return EXIT_FAILURE;
	}

	acm_fd = open(acmfile, O_RDWR | O_NOCTTY | O_SYNC);
	if (acm_fd < 0) {
		fprintf(stderr, "Can't open file: %s (%s).\n",
			acmfile, strerror(errno));
		return EXIT_FAILURE;
	}
	if (setup_serial_interface(acm_fd, BAUDRATE, 0) == -1) {
		fputs("Failed to setup ACM device\n", stderr);
#ifndef DEBUG
		// do not write packets to arbitrary (non character)
		// files in release mode
		return EXIT_FAILURE;
#endif
	}

	log_js_data(js_fd, jsfile);

	struct js_event e;
	while (read(js_fd, &e, sizeof(e)) == sizeof(e))
		process_event(&e);

	close(js_fd);
	close(acm_fd);
	return EXIT_SUCCESS;
}
