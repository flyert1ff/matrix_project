#include "output.h"
#include <stdio.h>
#include <stdarg.h>
#include <pthread.h>

static FILE *output_stream;
static FILE *file_stream = NULL;
static pthread_mutex_t output_mutex = PTHREAD_MUTEX_INITIALIZER;

void set_output_stream(FILE *stream) {
    pthread_mutex_lock(&output_mutex);
    output_stream = stream;
    pthread_mutex_unlock(&output_mutex);
}

void output(const char *format, ...) {
    pthread_mutex_lock(&output_mutex);
    va_list args;
    va_start(args, format);
    vfprintf(output_stream, format, args);
    va_end(args);
    pthread_mutex_unlock(&output_mutex);
}

int open_output_file(const char *filename, const char *mode) {
    pthread_mutex_lock(&output_mutex);
    if (file_stream)fclose(file_stream);
    file_stream = fopen(filename, mode);
    pthread_mutex_unlock(&output_mutex);
    return file_stream ? 0 : -1;
}

int close_output_file() {
    pthread_mutex_lock(&output_mutex);
    int ret = 0;
    if (file_stream) {
        ret = fclose(file_stream);
        file_stream = NULL;
    }
    pthread_mutex_unlock(&output_mutex);
    return ret;
}

int output_to_file(const char *format, ...) {
    pthread_mutex_lock(&output_mutex);
    if (!file_stream) {
        pthread_mutex_unlock(&output_mutex);
        return -1;
    }
    
    va_list args;
    va_start(args, format);
    int ret = vfprintf(file_stream, format, args);
    va_end(args);
    
    pthread_mutex_unlock(&output_mutex);
    return (ret >= 0) ? 0 : -1;
}
