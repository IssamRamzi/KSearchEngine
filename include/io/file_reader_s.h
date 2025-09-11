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


typedef struct {
    FILE* file;
    char* mot;
    char c;
} file_reader;

static file_reader* fr = NULL;
extern bool is_letter(char c);

char* fr_read(char path[]);
void fr_append_to_dict_s(char path[], dict_s *word_d);
void fr_append_to_dict_h(char path[], dict_h *word_d);

#endif // FILEREADER_H
