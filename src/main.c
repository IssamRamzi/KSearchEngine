#include <locale.h>

#include "core/indexation.h"
#include "core/utils.h"
#include "data/index_matrix.h"


int main(int argc, char** argv) {
    setlocale(LC_ALL, "en_US.UTF-8");

    /*
        ! ARGs
            - Arg [1] : Corpus Folder
            - Arg [2] : Filename for files dictionnary (loads if exists)
            - Arg [3] : Filename for words dictionnary (loads if exists)
            - Arg [4] : Filename for index matrix 
            
    */

    if (argc < 4) {
        printf("Not enough arguments\n");
        getchar();
        return -1;
    }
    printf("Corpus Folder : %s\n", argv[1]);
    // dir_s* dir = dir_s_create(argv[1]);
    // dir_s_get_files_wt(dir, argv[1], false);
    // dict_s_save(dir->files_dict, "\n", "../Results/files_small.txt");


    // loading dicts
    dict_s *files = dict_s_load("\n", "../Results/files_small.txt", false);
    // dict_h *words = dict_h_load("\n", argv[3]);


    // matrix
    dict_h* words = dict_h_create(300000);
    index_matrix* matrix = matrix_create(files->size);
    index(files, words, matrix);

    getchar();



    return 0;
}
