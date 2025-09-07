//
// Created by issbe on 07/09/2025.
//
#include "index_matrix.h"

#include <string.h>

index_matrix *matrix_create(int doc_number) {
    index_matrix *matrix = malloc(sizeof(index_matrix));
    if (matrix == NULL) {
        perror("create_matrix : ");
        return NULL;
    }
    matrix->size = doc_number + 1;
    matrix->heads = calloc(matrix->size, sizeof(cell_t *));
    for (int i = 0; i < matrix->size; i++) {
        matrix->heads[i] = cell_create(0, 0, NULL, FLOAT);
    }
    return matrix;
}

index_matrix *matrix_load(char *path) {
    // TODO
    FILE *f = fopen(path, "r");
    if (f == NULL) {
        perror("load_matrix : ");
        return NULL;
    }

    int size;
    if (fscanf(f, "%d\n", &size) != 1) {
        fprintf(stderr, "load_matrix: failed to read matrix size\n");
        fclose(f);
        return NULL;
    }

    index_matrix *matrix = matrix_create(size - 1);
    if (matrix == NULL) {
        perror("load_matrix :");
        return NULL;
    }

    int bufSize = 2048;
    char line[bufSize];
    for (int i = 0; i < matrix->size; i++) {
        if (fgets(line, bufSize, f) == NULL) {
            fprintf(stderr, "load_matrix : falied to read row\n");
            matrix_free(matrix);
            fclose(f);
            return NULL;
        }

        char *token = strtok(line, ' \n');

        cell_t *prev;
        cell_t *head;

        while (token != NULL) {
            void *value;
            int idx;

            if (sscanf(token, "%f ", &value) != 1) {
                fprintf(stderr, "load_matrix : falied to read values\n");
                matrix_free(matrix);
                fclose(f);
                return NULL;
            }

            if (sscanf(token, "%d ", &idx) != 1) {
                fprintf(stderr, "load_matrix : falied to read index\n");
                matrix_free(matrix);
                fclose(f);
                return NULL;
            }


            cell_t *cell = cell_create(value, idx, NULL, FLOAT);
        }
    }
}

void matrix_save(index_matrix *matrix, char *path) {
    // TODO : others DATA_TYPES
    FILE *f = fopen(path, "w");
    if (f == NULL) {
        perror("save_matrix : ");
        return;
    }
    fprintf(f, "%d\n", matrix->size);
    for (int i = 0; i < matrix->size; i++) {
        cell_t *cell = matrix->heads[i];
        while (cell) {
            fprintf(f, "%.2f:%d ", cell->value, cell->index);
            cell = cell->next;
        }
        fprintf(f, "\n");
    }
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

void matrix_set(index_matrix *matrix, int ndoc, int nterm, int val) {
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
        cell_t* new_cell = cell_create(val, nterm, head, head->cell_type);
        if (prev == NULL)
            matrix->heads[ndoc] = new_cell;
        else
            prev->next = new_cell;
    }
}

void matrix_increment(index_matrix *matrix, int ndoc, int nterm) {
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
                head->value.val_f += 1.0f;
                break;
            case INTEGER:
                head->value.val_i += 1;
                break;
            default:
                return;
        }
    }else {
        cell_t* new_cell = cell_create(1, nterm, head, head->cell_type);
        if (prev == NULL)
            matrix->heads[ndoc] = new_cell;
        else
            prev->next = new_cell;
    }
}
