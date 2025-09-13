//
// Created by issbe on 29/08/2025.
//

#ifndef FILEREADER_H
#define FILEREADER_H
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <ctype.h>

#include "../data/dict_s.h"
#include "data/dict_h.h"
#include "data/index_matrix.h"


typedef struct {
    FILE* file;
    char* mot;
    char c;
} file_reader;

static file_reader* fr = NULL;
extern bool is_letter(char c);

FILE* fr_init(char path[]);
void fr_advance();
void fr_start();
bool fr_end();


char* fr_read(char path[]);
void fr_append_to_dict_s(char path[], dict_s *word_d);
void fr_append_to_dict_h(char path[], dict_h *word_d);
// void fr_append_to_dict_h_and_matrix(char path[], dict_h *word_d, index_matrix* matrix);

#endif // FILEREADER_H
