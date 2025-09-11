//
// Created by issbe on 01/09/2025.
//

#ifndef HASH_TABLE_H
#define HASH_TABLE_H
#include <stdlib.h>
#include <string.h>
#define MAX_WORD_SIZE 256

typedef struct hash_node{
    char value[MAX_WORD_SIZE];
    int index;
    struct hash_node* next; // pr les conflits

} hash_node_s;

typedef struct {
    hash_node_s** nodes;
    int nodes_count;
    int size;
} hash_table_s;

unsigned int hash_code(char* str, int nodes_count);
hash_table_s* hash_table_s_create(int nodes_count);
void hash_insert(hash_table_s *ht,char* value);

#endif //HASH_TABLE_H
