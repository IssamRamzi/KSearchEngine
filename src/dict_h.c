//
// Created by issbe on 01/09/2025.
//
//
// Created by issbe on 29/08/2025.
//
#include "include/dict_h.h"

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// ! DONE

dict_h* dict_h_create(int max_dict_size) {
    dict_h* d = hash_table_s_create(max_dict_size);
    if (d == NULL) {
        perror("dict_h_create : ");
    }
    return d;
}

void dict_h_empty(dict_h* d) {
    for (int i = 0; i < d->nodes_count; i++) {
        hash_node_s *node = d->nodes[i];
        while (node) {
            hash_node_s *next = node->next;
            free(node);
            node = next;
        }
    }
    free(d->nodes);
    d->nodes = calloc(d->nodes_count, sizeof(hash_node_s*));
    d->size = 0;
}

char* dict_h_get_word(dict_h* d, int index) {
    if (index >= d->nodes_count) return NULL;
    for (int i = 0; i < d->nodes_count; i++) {
        struct hash_node* node = d->nodes[i];
        while (node) {
            if (node->index == index) return node->value;
            node = node->next;
        }
    }
    return NULL;
}

void dict_h_add_word(dict_h* d, char word[MAX_WORD_SIZE], bool check) {
    hash_insert(d, word);
}

int dict_h_get_index(dict_h* d, char word[MAX_WORD_SIZE]) {
    unsigned int idx = hash_code(word, d->nodes_count);
    hash_node_s* node = d->nodes[idx];
    while (node) {
        if (strcmp(node->value, word) == 0) {
            return node->index;
        }
        node = node->next;
    }
    return -1;
}

bool dict_h_contains(dict_h* d, char word[MAX_WORD_SIZE]) {
    return dict_h_get_index(d, word) != -1;
}

void dict_h_destroy(dict_h* d) {
    for (int i = 0; i < d->nodes_count; i++) {
        hash_node_s *node = d->nodes[i];
        while (node) {
            hash_node_s *next = node->next;
            free(node);
            node = next;
        }
    }
    free(d->nodes);
    free(d);
}

void dict_h_write(dict_h* d, char* end, int max) {
    if (max == -1) max = d->nodes_count;
    for (int i = 0; i < max; i++) {
        hash_node_s* node = d->nodes[i];
        while (node) {
            fprintf(stdout, "%s%s", node->value, end);
            node = node->next;
        }
    }
}


void dict_h_save(dict_h* d, char* end, char* path) {
    FILE* f = fopen(path, "w");
    for (int i = 0; i < d->nodes_count; i++) {
        hash_node_s* node = d->nodes[i];
        while (node) {
            fprintf(f, "%s%s", node->value, end);
            node = node->next;
        }
    }
    fclose(f);
}
