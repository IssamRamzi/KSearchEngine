#include "data/dict_s.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static pthread_mutex_t dict_mutex = PTHREAD_MUTEX_INITIALIZER;

dict_s* dict_s_create(int max_dict_size) {
    dict_s *dict = malloc(sizeof(dict_s));
    if (!dict) {
        perror("dict_s_create: malloc failed");
        return NULL;
    }

    dict->max_size = max_dict_size;
    dict->size = 0;
    dict->dict = malloc(sizeof(char*) * dict->max_size);
    if (!dict->dict) {
        perror("dict_s_create: malloc failed for dict array");
        free(dict);
        return NULL;
    }

    for (int i = 0; i < dict->max_size; i++) {
        dict->dict[i] = malloc(sizeof(char) * MAX_WORD_SIZE);
        if (!dict->dict[i]) {
            perror("dict_s_create: malloc failed for word");

            for (int j = 0; j < i; j++) {
                free(dict->dict[j]);
            }
            free(dict->dict);
            free(dict);
            return NULL;
        }
    }
    return dict;
}

void dict_s_empty(dict_s* d) {
    if (!d) return;
    pthread_mutex_lock(&dict_mutex);
    for (int i = 0; i < d->size; i++) {
        d->dict[i][0] = '\0';
    }
    d->size = 0;
    pthread_mutex_unlock(&dict_mutex);
}

void dict_s_add_word(dict_s* d, char word[MAX_WORD_SIZE], bool check) {
    if (!d || !word || word[0] == '\0') return;

    pthread_mutex_lock(&dict_mutex);

    if (check && dict_s_contains(d, word)) {
        pthread_mutex_unlock(&dict_mutex);
        return;
    }

    if (d->size < d->max_size) {
        strncpy(d->dict[d->size], word, MAX_WORD_SIZE - 1);
        d->dict[d->size][MAX_WORD_SIZE - 1] = '\0';
        d->size++;
    }

    pthread_mutex_unlock(&dict_mutex);
}


bool dict_s_contains(dict_s* d, char word[MAX_WORD_SIZE]) {
    if (!d || !word) return false;

    for (int i = 0; i < d->size; i++) {
        if (strncmp(word, d->dict[i], MAX_WORD_SIZE) == 0)
            return true;
    }
    return false;
}

char* dict_s_get_word(dict_s* d, int index) {
    if (!d || index >= d->size || index < 0) return NULL;
    return d->dict[index];
}

int dict_s_get_index(dict_s* d, char word[MAX_WORD_SIZE]) {
    if (!d || !word) return -1;

    for (int i = 0; i < d->size; i++) {
        if (strncmp(word, d->dict[i], MAX_WORD_SIZE) == 0) {
            return i;
        }
    }
    return -1;
}

void dict_s_destroy(dict_s* d) {
    if (d == NULL) return;

    for (int i = 0; i < d->max_size; i++) {
        free(d->dict[i]);
    }
    free(d->dict);
    free(d);
}

void dict_s_display(dict_s* d, char* end) {
    if (!d) return;
    for (int i = 0; i < d->size; i++) {
        printf("%s%s", d->dict[i], end ? end : "");
    }
    printf("\n");
}

void dict_s_save(dict_s* d, char* end, char* path) {
    if (!d || !path) return;

    FILE* f = fopen(path, "w");
    if (f == NULL) {
        perror("dict_s_save: failed to open file");
        return;
    }
    fprintf(f, "%d\n", d->size);
    for (int i = 0; i < d->size; i++) {
        fprintf(f, "%s%s", d->dict[i], end ? end : "");
    }
    printf("dict_s_save done\n");
    fclose(f);
}

dict_s* dict_s_load(char* end, char* path, bool check) {
    clock_t start_time = clock(), end_time;
    if (!path) return;

    FILE* f = fopen(path, "r");

    if (f == NULL) {
        perror("dict_s_load: failed to open file");
        return;
    }

    char buffer[MAX_WORD_SIZE];
    fgets(buffer, MAX_WORD_SIZE, f);
    char *endptr;

    long size = strtol(buffer, &endptr, 10); // str_int, endptr, base
    dict_s* d = dict_s_create((int)size);
    // printf("Loading dict_s with a size of %d\n",size);

    while (fgets(buffer, MAX_WORD_SIZE, f) != NULL) {
        dict_s_add_word(d, buffer, check);
    }
    end_time = clock();
    printf("Takes %ldms to load the dict_s and its size is %d\n",(end_time - start_time) * 1000 / CLOCKS_PER_SEC, d->size);
    fclose(f);
    return d;
}