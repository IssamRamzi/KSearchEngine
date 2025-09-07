#ifndef DIR_S_H
#define DIR_S_H

#include <stdio.h>
#include "dict_s.h"
#include <dirent.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#define MAX_WORD_SIZE 256
#define MAX_THREADS 512

struct dir_s {
    dict_s *files_dict;
    char path[MAX_WORD_SIZE];
    pthread_mutex_t lock;
};
typedef struct dir_s dir_s;

typedef struct  {
    char path[MAX_WORD_SIZE];
    dir_s *dict;
    bool check;
} search_thread_args_s;

dir_s* dir_s_create(char path[MAX_WORD_SIZE]);
void dir_s_get_files(dir_s* d, char path[MAX_WORD_SIZE], bool check);
void dir_s_get_files_wt(dir_s* d, char path[MAX_WORD_SIZE], bool check);
void dir_s_destroy(dir_s*);

#endif //DIR_S_H