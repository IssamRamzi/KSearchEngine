#ifndef CELL_H
#define CELL_H
#include <stdio.h>
#include <stdlib.h>

#include "dict_s.h"
#include "utils.h"

typedef struct cell{
    union content{
        int val_i;
        double val_f;
        char val_s[MAX_WORD_SIZE];
    } value;
    DATA_TYPE cell_type;
    int index;
    struct cell *next;
} cell_t;

// Declare functions but don't define them in header
cell_t *cell_create(void* value, int index, struct cell *next, DATA_TYPE type);
void cell_free(cell_t *cell);
void cell_print(cell_t* cell);

#endif //CELL_H