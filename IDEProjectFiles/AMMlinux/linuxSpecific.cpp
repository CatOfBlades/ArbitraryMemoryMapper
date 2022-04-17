
#include "Defines.h"

#include <fcntl.h>
#include <getopt.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <linux/kd.h>

#ifndef CLOCK_TICK_RATE
#define CLOCK_TICK_RATE 1193180
#endif

int console_fd = -1;

void Beep(int freq, int dur) //defined in defines.h
{
	int i; /* loop counter */

	/* try to snag the console */
	if((console_fd = open("/dev/console", O_WRONLY)) == -1)
	{
		printf("\a");  /* Output the only beep we can, in an effort to fall back on usefulness */
		perror("open");
		exit(1);
	}

	/* Beep */
	if(ioctl(console_fd, KIOCSOUND, (int)(CLOCK_TICK_RATE/freq)) < 0)
	{
	  printf("\a");  /* Output the only beep we can, in an effort to fall back on usefulness */
	  perror("ioctl");
	}
	/* Look ma, I'm not ansi C compatible! */
	usleep(1000*dur);                          /* wait...    */
	ioctl(console_fd, KIOCSOUND, 0);                    /* stop beep  */

	close(console_fd);
}

