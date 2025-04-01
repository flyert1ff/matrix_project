#ifndef OUTPUT_H
#define OUTPUT_H

#include <stdio.h>
#include <stdarg.h>
#include <pthread.h>

void set_output_stream(FILE *stream);
void output(const char *format, ...);
int open_output_file(const char *filename, const char *mode);
int close_output_file();
int output_to_file(const char *format, ...);

#endif