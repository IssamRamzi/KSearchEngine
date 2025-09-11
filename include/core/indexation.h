//
// Created by issbe on 09/09/2025.
//
#pragma once
#ifndef INDEXATION_H
#define INDEXATION_H
#include "io/dir_h.h"
#include "io/dir_s.h"
#include "io/dir_s.h"

dir_h *index_directory_h(char[MAX_WORD_SIZE]);
dir_s *index_directory_s(char[MAX_WORD_SIZE]);

dict_h *index_dictionnary_from_dir_h(dir_h* directory);
dict_h *index_dictionnary_from_dir_s(dir_s* directory);


#endif //INDEXATION_H
