//
// Created by issbe on 07/09/2025.
//

#ifndef INDEX_MATRIX_H
#define INDEX_MATRIX_H
#include "cell.h"

typedef struct {
    cell_t **heads;
    int size;
} index_matrix;

index_matrix* matrix_create(int doc_number);
index_matrix* matrix_load(char* path);
void matrix_save(index_matrix* matrix, char* path);
void matrix_free(index_matrix* matrix);

void* matrix_val(index_matrix* matrix, int ndoc, int nterm);
void matrix_increment(index_matrix* matrix, int ndoc, int nterm);
void matrix_seti(index_matrix* matrix, int ndoc, int nterm, int val);
void matrix_setf(index_matrix* matrix, int ndoc, int nterm, float val);
void matrix_sets(index_matrix* matrix, int ndoc, int nterm, char val[MAX_WORD_SIZE]);

void display_matrix(index_matrix* matrix);



#endif //INDEX_MATRIX_H
