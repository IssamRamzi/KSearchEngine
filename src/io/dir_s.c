//
// Created by issbe on 29/08/2025.
//
#include "io/dir_s.h"

#include "core/utils.h"


dir_s* dir_s_create(char path[MAX_WORD_SIZE]){
    dir_s *d = malloc(sizeof(dir_s));
    if (!d) {
        perror("malloc failed");
        exit(EXIT_FAILURE);
    }
    d->files_dict = dict_s_create(300000);
    pthread_mutex_init(&d->lock, NULL);

    strncpy(d->path, path, MAX_WORD_SIZE - 1);
    d->path[MAX_WORD_SIZE - 1] = '\0';

    return d;
}


void *dir_s_get_files_thread(void *arg) {
    search_thread_args_s *thread_args = (search_thread_args_s *)arg;
    dir_s *d = thread_args->dict;
    char *path = thread_args->path;

    dir_s_get_files_wt(d, path, thread_args->check);

    free(thread_args);
    pthread_exit(NULL);
}

void dir_s_get_files_wt(dir_s* d, char path[MAX_WORD_SIZE], bool check) {
    DIR* dir = opendir(path);
    if (dir == NULL) {
        fprintf(stderr, "Failed to open folder `%s`.\n", path);
        perror("opendir");
        return;
    }

    pthread_t threads[MAX_THREADS];
    int thread_count = 0;

    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        char full_path[MAX_WORD_SIZE];
        int written = snprintf(full_path, MAX_WORD_SIZE, "%s/%s", path, entry->d_name);
        if (written >= MAX_WORD_SIZE) {
            fprintf(stderr, "Path too long: %s/%s\n", path, entry->d_name);
            continue;
        }

        if (is_regular_file(full_path)) {
            dict_s_add_word(d->files_dict, full_path, check);
        }
        else if (!is_regular_file(full_path)) {
            if (thread_count >= MAX_THREADS) {
                fprintf(stderr, "Max threads reached, skipping directory: %s\n", full_path);
                continue;
            }

            search_thread_args_s *thread_args = malloc(sizeof(search_thread_args_s));
            if (!thread_args) {
                perror("malloc");
                continue;
            }

            strncpy(thread_args->path, full_path, MAX_WORD_SIZE - 1);
            thread_args->path[MAX_WORD_SIZE - 1] = '\0';
            thread_args->dict = d;
            thread_args->check = check;

            int result = pthread_create(&threads[thread_count], NULL, dir_s_get_files_thread, thread_args);
            if (result != 0) {
                perror("pthread_create");
                free(thread_args);
                continue;
            }

            thread_count++;
        }
    }

    closedir(dir);

    for (int i = 0; i < thread_count; i++) {
        pthread_join(threads[i], NULL);
    }
}


void dir_s_get_files(dir_s* d, char path[MAX_WORD_SIZE], bool check) {
    DIR* dir = opendir(path);
    if (dir == NULL) {
        perror("DEBUG: opendir error");
        return;
    }

    struct dirent* fReaded = NULL;
    int file_count = 0, dir_count = 0;

    while ((fReaded = readdir(dir)) != NULL) {
        if (strcmp(fReaded->d_name, ".") == 0 || strcmp(fReaded->d_name, "..") == 0)
            continue;

        char full_path[MAX_WORD_SIZE];
        snprintf(full_path, MAX_WORD_SIZE, "%s/%s", path, fReaded->d_name);

        if (is_regular_file(full_path)) {
            dict_s_add_word(d->files_dict, full_path, check);
            file_count++;
        } else {
            dir_s_get_files(d, full_path, check);
            dir_count++;
        }
    }

    closedir(dir);
}


void dir_s_destroy(dir_s* d){
    if (d) {
        dict_s_destroy(d->files_dict);
        free(d);
    }
}


