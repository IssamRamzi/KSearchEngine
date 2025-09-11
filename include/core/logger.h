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

char* get_string_log_level(LOG_LEVEL level);
void init_logger(const char* path);
void log(LOG_LEVEL level, char* message);




#endif //LOGGER_H
