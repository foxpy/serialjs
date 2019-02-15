#include <stdio.h>
#include <stdlib.h>
#include <termios.h>

int setup_serial_interface(int fd, int speed, int parity)
{
	struct termios tty;
	if (tcgetattr(fd, &tty) != EXIT_SUCCESS) {
		perror("tcgetattr");
		exit(EXIT_FAILURE);
	}

	cfsetospeed (&tty, speed);
	cfsetispeed (&tty, speed);

	tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;
	tty.c_iflag &= ~IGNBRK;
	tty.c_lflag = 0;
	tty.c_oflag = 0;
	tty.c_cc[VMIN] = 0;
	tty.c_iflag &= ~(IXON | IXOFF | IXANY);
	tty.c_cflag |= (CLOCAL | CREAD);
	tty.c_cflag &= ~(PARENB | PARODD);
	tty.c_cflag |= parity;
	tty.c_cflag &= ~CSTOPB;
	tty.c_cflag &= ~CRTSCTS;

	if (tcsetattr(fd, TCSANOW, &tty) != 0) {
		perror("tcsetattr");
		exit(EXIT_FAILURE);
	}
	return EXIT_SUCCESS;
}
