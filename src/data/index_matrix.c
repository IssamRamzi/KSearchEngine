//
// Created by issbe on 07/09/2025.
//
#include "data/index_matrix.h"

#include <string.h>
#include <stdlib.h>
#include "data/hash_table.h"

index_matrix *matrix_create(int doc_number) {
    index_matrix *matrix = malloc(sizeof(index_matrix));
    if (matrix == NULL) {
        perror("create_matrix : ");
        return NULL;
    }
    matrix->size = doc_number;
    matrix->heads = calloc(matrix->size, sizeof(cell_t *));

    return matrix;
}

void matrix_seti(index_matrix* matrix, int ndoc, int nterm, int val) {

}

void matrix_setf(index_matrix* matrix, int ndoc, int nterm, float val) {

}

void matrix_sets(index_matrix* matrix, int ndoc, int nterm, char val[MAX_WORD_SIZE]) {

}

index_matrix *matrix_load(char *path) {
    FILE *f = fopen(path, "r");
    if (f == NULL) {
        perror("load_matrix");
        return NULL;
    }

    int size;
    if (fscanf(f, "%d\n", &size) != 1) {
        fprintf(stderr, "load_matrix: failed to read matrix size\n");
        fclose(f);
        return NULL;
    }

    index_matrix *matrix = matrix_create(size);
    if (matrix == NULL) {
        perror("load_matrix");
        fclose(f);
        return NULL;
    }

    int buffSize = 16384;
    char *line = malloc(buffSize * sizeof(char));
    if (line == NULL) {
        perror("load_matrix");
        matrix_free(matrix);
        fclose(f);
        free(line);
        return NULL;
    }

    for (int i = 0; i < size; i++) {
        if (fgets(line, buffSize, f)) {
            cell_t *head = NULL;
            cell_t *tail = NULL;
            char *token = strtok(line, " \n");
            int last_index = -1;

            while (token != NULL) {
                double value;
                int index;
                if (sscanf(token, "%lf:%d", &value, &index) == 2) {
                    if (index >= 0) {
                        if (index <= last_index) {
                            fprintf(stderr, "load_matrix: indices must be in ascending order in line %d\n", i + 2);
                            matrix_free(matrix);
                            free(line);
                            fclose(f);
                            return NULL;
                        }
                        cell_t *new_cell = cell_create_f((float)value, index, NULL);
                        if (new_cell == NULL) {
                            fprintf(stderr, "load_matrix: failed to create cell in line %d\n", i + 2);
                            matrix_free(matrix);
                            free(line);
                            fclose(f);
                            return NULL;
                        }
                        if (tail == NULL) {
                            matrix->heads[i] = new_cell;
                            head = new_cell;
                            tail = new_cell;
                        } else {
                            tail->next = new_cell;
                            tail = new_cell;
                        }
                        last_index = index;
                    } else {
                        fprintf(stderr, "load_matrix: invalid index %d in line %d\n", index, i + 2);
                        matrix_free(matrix);
                        free(line);
                        fclose(f);
                        return NULL;
                    }
                } else {
                    fprintf(stderr, "load_matrix: invalid format in line %d\n", i + 2);
                    matrix_free(matrix);
                    free(line);
                    fclose(f);
                    return NULL;
                }
                token = strtok(NULL, " \n");
            }
        } else {
            fprintf(stderr, "load_matrix: failed to read line %d\n", i + 2);
            matrix_free(matrix);
            free(line);
            fclose(f);
            return NULL;
        }
    }

    printf("Matrix Loaded successfully !\n");
    matrix_save(matrix, "test_load.txt");
    free(line);
    fclose(f);
    return matrix;
}


void matrix_free(index_matrix *matrix) {
    if (matrix == NULL) return;
    for (int i = 0; i < matrix->size; i++) {
        cell_t *cell = matrix->heads[i];
        while (cell) {
            cell_t *temp = cell;
            cell = cell->next;
            free(temp);
        }
    }
    free(matrix->heads);
    free(matrix);
}

void *matrix_val(index_matrix *matrix, int ndoc, int nterm) {
    if (matrix == NULL || ndoc < 0 || ndoc > matrix->size || nterm < 0) return NULL;
    cell_t *head = matrix->heads[ndoc];
    while (head && head->index <= nterm) {
        if (head->index == nterm) {
            switch (head->cell_type) {
                case FLOAT:
                    return &head->value.val_f;
                case INTEGER:
                    return &head->value.val_i;
                case STRING:
                    return &head->value.val_s;
                default:
                    return NULL;
            }
        }
        head = head->next;
    }
    return NULL;
}

void matrix_set(index_matrix *matrix, int ndoc, int nterm, float val) {
    if (matrix == NULL || ndoc < 0 || ndoc > matrix->size || nterm < 0) return;
    cell_t *prev = NULL;
    cell_t *head = matrix->heads[ndoc];

    while (head != NULL && head->index < nterm) {
        prev = head;
        head = head->next;
    }


    if (head->index == nterm) {
        switch (head->cell_type) {
            case FLOAT:
                head->value.val_f = val;
                break;
            case INTEGER:
                head->value.val_i = val;
                break;
            default:
                return;
        }
    }else {
        cell_t* new_cell = cell_create_f(val, nterm, head);
        if (prev == NULL)
            matrix->heads[ndoc] = new_cell;
        else
            prev->next = new_cell;
    }
}

void matrix_increment(index_matrix *matrix, int ndoc, int nterm) {
    if (matrix == NULL || ndoc < 0 || ndoc >= matrix->size || nterm < 0) return;

    cell_t *prev = NULL;
    cell_t *head = matrix->heads[ndoc];

    if (head == NULL) {
        matrix->heads[ndoc] = cell_create_f(1.0f, nterm, NULL);
        return;
    }

    while (head != NULL && head->index < nterm) {
        prev = head;
        head = head->next;
    }

    if (head != NULL && head->index == nterm) {
        switch (head->cell_type) {
            case FLOAT:
                head->value.val_f += 1.0f;
                break;
            case INTEGER:
                head->value.val_i += 1;
                break;
            default:
                return;
        }
    }
    else {
        cell_t* new_cell = cell_create_f(1.0f, nterm, head);
        if (prev == NULL)
            matrix->heads[ndoc] = new_cell;
        else
            prev->next = new_cell;
    }
}

void matrix_display(index_matrix* matrix, int max) {
    for (int i = 0; i < max; i++) {
        cell_t *cell = matrix->heads[i];
        while (cell) {
            printf("%.1f ", cell->value.val_f);
            cell = cell->next;
        }
        printf("\n");
    }
}

void matrix_save(index_matrix *matrix, char *path) {
    FILE *f = fopen(path, "w");
    if (f == NULL) {
        perror("save_matrix : ");
        return;
    }
    fprintf(f, "%d\n", matrix->size);
    for (int i = 0; i < matrix->size; i++) {
        cell_t *cell = matrix->heads[i];
        while (cell) {
            fprintf(f, "%.2f:%d ", cell->value.val_f, cell->index);
            cell = cell->next;
        }
        fprintf(f, "\n");
    }
    fflush(f);
}
