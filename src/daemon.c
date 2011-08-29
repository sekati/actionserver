#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include "daemon.h"

int daemonize() {
	pid_t pid;

	if ((pid = fork()) < 0) {
		return -1;
	} else if (pid != 0) {
		exit(0);
	}

	setsid();

	if (fork() != 0) {
		exit(0);
	}

	return 0;
}
