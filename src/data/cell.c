#include "data/cell.h"
cell_t *cell_create_i(int value, int index, struct cell *next) {
    cell_t* cell = malloc(sizeof(cell_t));
    if (cell == NULL) {
        perror("create_cell_i :");
        return NULL;
    }
    cell->cell_type = INTEGER;
    cell->index = index;
    cell->next = next;
    cell->value.val_i = value;
    return cell;

}
cell_t *cell_create_f(float value, int index, struct cell *next) {
    cell_t* cell = malloc(sizeof(cell_t));
    if (cell == NULL) {
        perror("create_cell_i :");
        return NULL;
    }
    cell->cell_type = INTEGER;
    cell->index = index;
    cell->next = next;
    cell->value.val_f = value;
    return cell;
}
cell_t *cell_create_s(char value[MAX_WORD_SIZE], int index, struct cell *next) {
    cell_t* cell = malloc(sizeof(cell_t));
    if (cell == NULL) {
        perror("create_cell_i :");
        return NULL;
    }
    cell->cell_type = INTEGER;
    cell->index = index;
    cell->next = next;
    strncpy(cell->value.val_s, value, MAX_WORD_SIZE - 1);
    cell->value.val_s[MAX_WORD_SIZE - 1] = '\0';
    return cell;
}

// cell_t *cell_create(void* value, int index, struct cell *next, DATA_TYPE type) {
//     cell_t* cell = malloc(sizeof(cell_t));
//     if (cell == NULL) {
//         perror("create_cell_i :");
//         return NULL;
//     }
//     cell->cell_type = type;
//     cell->index = index;
//     cell->next = next;
//
//     switch (cell->cell_type) {
//         case INTEGER:
//             cell->value.val_i = *(int*)value;
//             break;
//         case FLOAT:
//             cell->value.val_f = *(double*)value;
//             break;
//         case STRING:
//             strncpy(cell->value.val_s, (char*)value, MAX_WORD_SIZE - 1);
//             cell->value.val_s[MAX_WORD_SIZE - 1] = '\0';
//             break;
//         default:
//             free(cell);
//             return NULL;
//     }
//     return cell;
// }

void cell_free(cell_t *cell) {
    if (cell) {
        free(cell);
    }
}

void cell_print(cell_t* cell) {
    if (cell == NULL) {
        printf("Cell is NULL\n");
        return;
    }
    printf("Cell[index=%d]: ", cell->index);
    switch (cell->cell_type) {
        case INTEGER:
            printf("Integer = %d\n", cell->value.val_i);
            break;
        case FLOAT:
            printf("Float = %f\n", cell->value.val_f);
            break;
        case STRING:
            printf("String = %s\n", cell->value.val_s);
            break;
        default:
            printf("Unknown type\n");
    }
}