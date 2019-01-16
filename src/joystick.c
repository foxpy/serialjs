#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/joystick.h>

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
		printf("[%d]: input 0x%08x type of 0x%02x, button: 0x%02x.\n",
				e.time, e.value, e.type, e.number);
	}

	close(fd);
	return EXIT_SUCCESS;
}
