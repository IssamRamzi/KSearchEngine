//
// Created by issbe on 02/09/2025.
//

#ifndef DIR_H_H
#define DIR_H_H
#include <pthread.h>

#include "dict_h.h"

struct dir_h {
    dict_h *files_dict;
    char path[MAX_WORD_SIZE];
    pthread_mutex_t lock;
};
typedef struct dir_h dir_h;

typedef struct  {
    char path[MAX_WORD_SIZE];
    dict_h *dict;
}search_thread_args_h;

dir_h* dir_h_create(char path[MAX_WORD_SIZE]);
void dir_h_get_files(dir_h* d, char path[MAX_WORD_SIZE]);
void dir_h_get_files_wt(dir_h* d, char path[MAX_WORD_SIZE]);
void dir_h_destroy(dir_h*);


#endif //DIR_H_H
