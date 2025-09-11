#include <stdio.h>
#include "io/dir_s.h"

#include <time.h>

#include "core/indexation.h"
#include "io/dir_h.h"
#include "data/index_matrix.h"
#include "io/file_reader_s.h"


int main(int argc, char** argv) {
    setlocale(LC_ALL, "en_US.UTF-8");

    /*
        ! ARGs
            - Arg [1] : Corpus Folder
            - Arg [2] : Filename for words dictionnary (loads if exists)
            - Arg [3] : Filename for index matrix 
            
    */

    if (argc < 2) {
        printf("Not enough arguments\n");
        getchar();
        return -1;
    }
    printf("Corpus Folder : %s\n", argv[1]);
    // dir_s *directory = index_directory_s(argv[1]);
    // dict_s_save(directory->files_dict, "\n", "Results/dic_ssss.txt");
    // dict_h* words = index_dictionnary_from_dir_s(directory);
    // dict_h_save(words,"\n", "dic_h.txt");

    // dict_s* dict = dict_s_load("\n", "dic_s.txt", false);;
    // dict_s_load("\n", "dic_s.txt", false);

    // dict_s_display(dict, "");

    // index_matrix* matrix;
    // matrix = matrix_create(directory_h->files_dict->size);

    dict_s *files = dict_s_load("\n", "Results/files.txt", false);
    dict_h *words = dict_h_load("\n", "Results/words.txt");


    getchar();



    return 0;
}
