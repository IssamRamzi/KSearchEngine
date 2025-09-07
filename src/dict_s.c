#include "dict_s.h"  // Chemin corrigé
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
            // Cleanup already allocated memory
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
    if (!d) return;

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
    for (int i = 0; i < d->size; i++) {
        fprintf(f, "%s%s", d->dict[i], end ? end : "");
    }
    fclose(f);
}