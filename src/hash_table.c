//
// Created by issbe on 01/09/2025.
//
#include "include/hash_table.h"

unsigned int hash_code(char* str, int nodes_count) {
    unsigned int hash = 5381;
    int c;
    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c;
    }
    return hash % nodes_count;
}

hash_table_s* hash_table_s_create(int nodes_count) {
    hash_table_s* ht = malloc(sizeof(hash_table_s));
    ht->nodes = calloc(nodes_count, sizeof(hash_node_s*));
    ht->nodes_count = nodes_count;
    ht->size = 0;

    return ht;
}

void hash_insert(hash_table_s *ht,char* value) {
    unsigned int idx = hash_code(value, ht->nodes_count);
    hash_node_s* node = ht->nodes[idx];

    while (node) { // contains ?
        if (strcmp(node->value, value) == 0) return;
        node = node->next;
    }
    // printf("Debug: About to malloc\n");
    node = malloc(sizeof(hash_node_s)); // MEM LEAK
    if (!node) return;
    strncpy(node->value, value, MAX_WORD_SIZE);
    node->value[MAX_WORD_SIZE - 1] = '\0';
    node->next = ht->nodes[idx];
    ht->nodes[idx] = node;
    ht->nodes[idx]->index = ht->size;
    ht->size++;
}