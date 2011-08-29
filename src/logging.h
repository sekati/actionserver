#ifndef __LOGGING_H__
#define __LOGGING_H__

int log_start(char *filename);
void log_write(char *format, ...);
char *log_fmt_time();
void log_stop();

#endif
