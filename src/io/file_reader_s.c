#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "io/file_reader_s.h"
#include "core/utils.h"

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
        printf("Failed to open: %s\n", path);
        free(fr);
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
    while (fr->c != EOF && !isalpha((unsigned char)fr->c)) {
        fr->c = fgetc(fr->file);
    }
}

void advance() {
    int i = 0;
    fr->mot[0] = '\0';
    while (fr->c != EOF && isalpha((unsigned char)fr->c) && i < MAX_WORD_SIZE - 1) {
        fr->mot[i++] = (char)tolower((unsigned char)fr->c);
        fr->c = fgetc(fr->file);
    }
    fr->mot[i] = '\0';
}

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

void fr_append_to_dict_s(char path[], dict_s *word_d) {
    if (init(path) == NULL) return;
    start();

    while (fr->c != EOF) {
        advance();
        if (strlen(fr->mot) > 0) {
            // printf("Adding word: '%s' from pos %ld\n", fr->mot, ftell(fr->file));
            dict_s_add_word(word_d, fr->mot, true);
        }
        while (fr->c != EOF && !isalpha((unsigned char)fr->c)) {
            fr->c = fgetc(fr->file);
        }
    }

    fclose(fr->file);
    free(fr->mot);
    free(fr);
    fr = NULL;
}

void fr_append_to_dict_h(char path[], dict_h *word_d) {
    if (init(path) == NULL) return;
    start();

    while (fr->c != EOF) {
        advance();
        if (strlen(fr->mot) > 0) {
            // printf("Adding word: '%s' from pos %ld\n", fr->mot, ftell(fr->file));
            dict_h_add_word(word_d, fr->mot, true);
        }
        while (fr->c != EOF && !isalpha((unsigned char)fr->c)) {
            fr->c = fgetc(fr->file);
        }
    }

    fclose(fr->file);
    free(fr->mot);
    free(fr);
    fr = NULL;
}

