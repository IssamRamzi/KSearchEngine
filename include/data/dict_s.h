#ifndef DICT_S_H
#define DICT_S_H

#define MAX_WORD_SIZE 256
#include <stdbool.h>

struct dict_s {
    int max_size;
    int size;
    char **dict;
};
typedef struct dict_s dict_s;

dict_s*     dict_s_create(int max_dict_size);
void        dict_s_empty(dict_s* d);
char*       dict_s_get_word(dict_s* d, int index);
int         dict_s_get_index(dict_s* d, char word[MAX_WORD_SIZE]);
void        dict_s_add_word(dict_s* d, char word[MAX_WORD_SIZE], bool check);
bool        dict_s_contains(dict_s* d, char word[MAX_WORD_SIZE]);
void        dict_s_destroy(dict_s* d);

void        dict_s_display(dict_s* d, char* end);
void        dict_s_save(dict_s* d, char* end, char* path);
dict_s*     dict_s_load(char* end, char* path, bool check);
#endif //DICT_S_H