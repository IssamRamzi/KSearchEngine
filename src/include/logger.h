//
// Created by issbe on 17/08/2025.
//
#pragma once

#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>
#include <time.h>

#define MAX_LOG_SIZE 2048

typedef enum  {
    DEBUG,
    INFO,
    ERROR,
    WARNING
} LOG_LEVEL;

typedef struct {
    FILE* file;
} Logger;

static Logger logger;


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
    if (logger.file)
        printf("%s", message);
        fprintf(logger.file, message);
}



#endif //LOGGER_H
