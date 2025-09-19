#ifndef UTILS_H
#define UTILS_H

#include <sys/stat.h>
#include <stdbool.h>
#include <stdint.h>

typedef enum {
    INTEGER,
    FLOAT,
    STRING
} DATA_TYPE;

int is_regular_file(const char *path);
bool does_file_exist(const char* path);
bool is_letter(const char c);
int to_lower(uint8_t c);

#endif // UTILS_H