#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "io/file_reader_s.h"
#include "core/utils.h"

file_reader* fr_init(char path[]) {
    file_reader* fr = malloc(sizeof(file_reader));
    if (!fr) {
        perror("fr_init: malloc failed");
        return NULL;
    }

    fr->file = fopen(path, "rb");
    if (fr->file == NULL) {
        printf("Failed to open: %s\n", path);
        perror("fopen error");
        free(fr);
        return NULL;
    }

    fr->mot = malloc(MAX_WORD_SIZE);
    if (!fr->mot) {
        perror("fr_init: malloc failed for mot");
        fclose(fr->file);
        free(fr);
        return NULL;
    }

    fr->c = '\0';
    fr->mot[0] = '\0';

    return fr;
}

void fr_start(file_reader* fr) {
    if (fr == NULL || fr->file == NULL) {
        fprintf(stderr, "fr_start: file_reader or file is not initialized\n");
        return;
    }

    fr->c = fgetc(fr->file);
    while (fr->c != EOF && !isalpha((unsigned char)fr->c)) {
        fr->c = fgetc(fr->file);
    }
}

void fr_advance(file_reader* fr) {
    if (fr == NULL || fr->file == NULL) {
        fprintf(stderr, "fr_advance: file_reader not initialized\n");
        return;
    }

    int i = 0;
    fr->mot[0] = '\0';

    // Lire le mot actuel
    while (fr->c != EOF && isalpha((unsigned char)fr->c) && i < MAX_WORD_SIZE - 1) {
        fr->mot[i++] = (char)tolower((unsigned char)fr->c);
        fr->c = fgetc(fr->file);
    }
    fr->mot[i] = '\0';
}

bool fr_end(file_reader* fr) {
    return fr == NULL || fr->c == EOF;
}

void fr_free(file_reader* fr) {
    if (fr == NULL) return;

    if (fr->file != NULL) {
        fclose(fr->file);
        fr->file = NULL;
    }

    if (fr->mot != NULL) {
        free(fr->mot);
        fr->mot = NULL;
    }

    free(fr);
}

char* fr_read(char path[]) {
    FILE *file = fopen(path, "r");
    if (file == NULL) {
        perror("fr_read: opening file");
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    rewind(file);

    char *content = malloc(size + 1);
    if (!content) {
        perror("fr_read: malloc");
        fclose(file);
        return NULL;
    }

    size_t read_size = fread(content, 1, size, file);
    content[read_size] = '\0';
    fclose(file);

    return content;
}

void fr_append_to_dict_s(char path[], dict_s *word_d) {
    file_reader* fr = fr_init(path);
    if (fr == NULL) return;

    fr_start(fr);

    while (!fr_end(fr)) {
        fr_advance(fr);
        if (strlen(fr->mot) > 0) {
            dict_s_add_word(word_d, fr->mot, true);
        }
        while (!fr_end(fr) && !isalpha((unsigned char)fr->c)) {
            fr->c = fgetc(fr->file);
        }
    }

    fr_free(fr);
}

void fr_append_to_dict_h(char path[], dict_h *word_d) {
    char clean_path[MAX_WORD_SIZE];
    strncpy(clean_path, path, MAX_WORD_SIZE - 1);
    clean_path[MAX_WORD_SIZE - 1] = '\0';
    clean_path[strcspn(clean_path, "\n\r")] = '\0';

    file_reader* fr = fr_init(clean_path);
    if (fr == NULL) return;

    fr_start(fr);

    while (!fr_end(fr)) {
        fr_advance(fr);
        if (strlen(fr->mot) > 0) {
            dict_h_add_word(word_d, fr->mot, true);
        }
        while (!fr_end(fr) && !isalpha((unsigned char)fr->c)) {
            fr->c = fgetc(fr->file);
        }
    }

    fr_free(fr);
}