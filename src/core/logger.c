//
// Created by issbe on 08/09/2025.
//
#include "core/logger.h"

#include <stdlib.h>

char* get_string_log_level(LOG_LEVEL level) {
    switch (level) {
        case DEBUG: return "[ DEBUG ]";
        case INFO: return "[ INFO ]";
        case ERROR: return "[ ERROR ]";
        case WARNING: return "[ WARNING ]";
        default: return "Unknown";
    }
}


void init_logger(const char* path) {
    logger.file = fopen(path, "w+");
    if (logger.file == NULL) {
        perror("init_logger : ");
    }
}

void log_(LOG_LEVEL level, char* message) {
    // time - level - message
    time_t now = time(0);
    struct tm* timeinfo = localtime(&now);
    char timestamp[20];
    strftime(timestamp, sizeof(timestamp),
             "%Y-%m-%d %H:%M:%S", timeinfo);
    char* result = malloc(sizeof(char) * MAX_LOG_SIZE);
    snprintf(result, MAX_LOG_SIZE, "[ %s ] - %s : %s\n", timestamp, get_string_log_level(level), message);
    printf("%s", message);
    if (logger.file) {
        fprintf(logger.file, message);
    }
    free(result);
}
