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

#include "dict_s.h"


typedef struct {
    FILE* file;
    char* mot;
    char c;
} file_reader;

static file_reader* fr = NULL;

extern bool is_letter(char c);

char* fr_read(char path[]) {
    FILE *file = fopen(path, "r");
    if (file == NULL) {
        perror("opening file");
        exit(EXIT_FAILURE);
    }

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    rewind(file);

    char *content = malloc(size + 1);
    if (!content) {
        perror("malloc");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    fread(content, 1, size, file);
    content[size] = '\0';
    fclose(file);
    return content;
}

FILE* init(char path[]) {
    if (fr != NULL) {
        fclose(fr->file);
        free(fr->mot);
        free(fr);
    }

    fr = malloc(sizeof(file_reader));
    if (!fr) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    fr->file = fopen(path, "r");
    if (fr->file == NULL) {
        printf("%s\n",path);
        //perror("opening file");
        // exit(EXIT_FAILURE);
        return NULL;
    }

    fr->mot = malloc(MAX_WORD_SIZE);
    if (!fr->mot) {
        perror("malloc");
        fclose(fr->file);
        free(fr);
        exit(EXIT_FAILURE);
    }
    fr->c = '\0';
    return fr->file;
}

void start() {
    fr->c = fgetc(fr->file);
    while (fr->c != EOF && !is_letter(fr->c)) {
        fr->c = fgetc(fr->file);
    }
}

void advance() {
    int i = 0;
    fr->mot[0] = '\0';
    while (fr->c != EOF && is_letter(fr->c)) {
        fr->mot[i++] = (char)tolower(fr->c);
        fr->c = fgetc(fr->file);
    }
    fr->mot[i] = '\0';
}

void fr_append_to_dict_s(char path[], dict_s *word_d) {
    if (init(path) == NULL) return;
    start();

    while (fr->c != EOF) {
        advance();
        if (strlen(fr->mot) > 0) {
            dict_s_add_word(word_d, fr->mot, true);
        }
        while (fr->c != EOF && !is_letter(fr->c)) {
            fr->c = fgetc(fr->file);
        }
    }

    fclose(fr->file);
    free(fr->mot);
    free(fr);
    fr = NULL;
}

void fr_append_to_dict_h(char path[], dict_h *word_d) {
    init(path);
    start();

    while (fr->c != EOF) {
        advance();
        if (strlen(fr->mot) > 0) {
            dict_h_add_word(word_d, fr->mot, true);
        }
        while (fr->c != EOF && !is_letter(fr->c)) {
            fr->c = fgetc(fr->file);
        }
    }
    fclose(fr->file);
    // snprintf(fr->mot, MAX_WORD_SIZE, "\0");
    //snprintf(fr->mot, MAX_WORD_SIZE, "");
    //free(fr->mot);

    free(fr);

    fr = NULL;
}

#endif // FILEREADER_H
