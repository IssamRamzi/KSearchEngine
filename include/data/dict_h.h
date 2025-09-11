//
// Created by issbe on 01/09/2025.
//

#ifndef DICT_H_H
#define DICT_H_H
#include <stdbool.h>

#include "hash_table.h"

typedef hash_table_s dict_h;

dict_h*     dict_h_create(int max_dict_size);
void        dict_h_empty(dict_h* d);
char*       dict_h_get_word(dict_h* d, int index);
int         dict_h_get_index(dict_h* d, char word[MAX_WORD_SIZE]);
void        dict_h_add_word(dict_h* d, char word[MAX_WORD_SIZE], bool check);
bool        dict_h_contains(dict_h* d, char word[MAX_WORD_SIZE]);
void        dict_h_destroy(dict_h* d);

void        dict_h_write(dict_h* d, char* end, int max);
void        dict_h_save(dict_h* d, char* end, char* path);
dict_h*     dict_h_load(char* end, char* path);

#endif //DICT_H_H
