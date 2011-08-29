#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>
#include "logging.h"

FILE *fp;

int log_start(char *filename) {
	fp = fopen(filename, "at");

	if (fp == NULL) {
		return -1;
	}

	return 0;
}

void log_write(char *format, ...) {
	va_list ap;

	va_start(ap, format);
	vfprintf(fp, format, ap);
	va_end(ap);

	fflush(fp);
}

static char strtime[100];

char *log_fmt_time() {
	time_t t = time(NULL);

	struct tm *t2 = localtime(&t);

	strftime(strtime, sizeof(strtime), "%Y-%m-%d %H:%M:%S", t2);

	return strtime;
}

void log_stop() {
	fclose(fp);
}

