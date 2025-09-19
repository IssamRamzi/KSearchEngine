#include "core/utils.h"
#include <unistd.h>


int is_regular_file(const char *path)
{
    struct stat path_stat;
    stat(path, &path_stat);
    return S_ISREG(path_stat.st_mode);
}

bool does_file_exist(const char* path) {
    if (access(path, F_OK) == 0) {
        return true;
    }
    return false;
}

bool is_letter(const char c) {
    return (c >= '0' && c <= '9') ||
           (c >= 'a' && c <= 'z') ||
           (c >= 'A' && c <= 'Z') ||
           (c >= 192);
}

int to_lower(uint8_t c) {
    if (c >= 'A' && c <= 'Z') return c - 'a';
    return c;
}
