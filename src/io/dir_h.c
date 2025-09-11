//
// Created by issbe on 02/09/2025.
//
#include "io/dir_h.h"

#include <dirent.h>
#include <stdio.h>
#include <sys/stat.h>
// ! WORK ON WT VERSION

extern int is_regular_file(const char *path);


dir_h* dir_h_create(char path[MAX_WORD_SIZE]) {
    dir_h *d = malloc(sizeof(dir_h));
    if (!d) {
        perror("malloc failed");
        exit(EXIT_FAILURE);
    }
    pthread_mutex_init(&d->lock, NULL);
    d->files_dict = dict_h_create(100000);
    strncpy(d->path, path, MAX_WORD_SIZE);
    d->path[MAX_WORD_SIZE - 1] = '\0';

    return d;
}



void dir_h_get_files(dir_h* d, char path[MAX_WORD_SIZE], bool check) {
    DIR* dir = opendir(path);
    if (dir == NULL) {
        printf("failed to open folder `%s`.\n", path);
        return;
    }

    struct dirent* fReaded = NULL;
    while ((fReaded = readdir(dir)) != NULL) {
        if (strcmp(fReaded->d_name, ".") == 0 || strcmp(fReaded->d_name, "..") == 0)
            continue;

        char full_path[MAX_WORD_SIZE];
        snprintf(full_path, MAX_WORD_SIZE, "%s/%s", path, fReaded->d_name);

        if (is_regular_file(full_path)) {
            // printf("FILE : %s\n", full_path);
            dict_h_add_word(d->files_dict, full_path, false);

        } else {
            // printf("DIR : %s\n", full_path);
            dir_h_get_files(d, full_path, check);
        }
    }
    closedir(dir);
}

void *dir_h_get_files_thread(void *arg) {
    search_thread_args_h *thread_args = (search_thread_args_h *)arg;
    dir_h *d = (dir_h*)thread_args->dict;
    char *path = thread_args->path;
    dir_h_get_files_wt(d, path, thread_args->check);

    free(thread_args);
    return NULL;
}

void dir_h_get_files_wt(dir_h* d, char path[MAX_WORD_SIZE], bool check) {
    DIR* dir = opendir(path);
    if (dir == NULL) {
        printf("failed to open folder `%s`.\n", path);
        return;
    }

    pthread_t threads[1024];
    int thread_count = 0;

    struct dirent* fReaded;
    while ((fReaded = readdir(dir)) != NULL) {
        if (strcmp(fReaded->d_name, ".") == 0 || strcmp(fReaded->d_name, "..") == 0)
            continue;

        char full_path[MAX_WORD_SIZE];
        snprintf(full_path, MAX_WORD_SIZE, "%s/%s", path, fReaded->d_name);

        if (is_regular_file(full_path)) {
            pthread_mutex_lock(&d->lock);
            dict_h_add_word(d->files_dict, full_path, check);
            pthread_mutex_unlock(&d->lock);
        } else {
            search_thread_args_h *thread_args = malloc(sizeof(search_thread_args_h));
            thread_args->check = check;
            snprintf(thread_args->path, MAX_WORD_SIZE, "%s/%s", path, fReaded->d_name);
            thread_args->dict = d;

            pthread_create(&threads[thread_count++], NULL,
                           dir_h_get_files_thread, thread_args);
        }
    }
    closedir(dir);


    for (int i = 0; i < thread_count; i++) {
        pthread_join(threads[i], NULL);
    }
}

void dir_h_write_all_files(dir_h* d, char* output_path) {
    if (!d || !d->files_dict) {
        return;
    }
    dict_h_save(d->files_dict, "\n", output_path);
}

void dir_h_destroy(dir_h* d) {
    if (d) {
        dict_h_destroy(d->files_dict);
        free(d);
    }
}