#ifndef UTILS_H
#define UTILS_H

#include <sys/stat.h>
#include <stdbool.h>

typedef enum {
    INTEGER,
    FLOAT,
    STRING
} DATA_TYPE;

int is_regular_file(const char *path);
bool is_letter(const char c);

#endif // UTILS_H