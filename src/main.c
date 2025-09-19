#include <locale.h>

#include "core/indexation.h"
#include "core/utils.h"
#include "data/index_matrix.h"


int main(int argc, char** argv) {
    setlocale(LC_ALL, "en_US.UTF-8");

    if (argc < 5) {
        printf("Not enough arguments :\n"
            "\t - Arg [1] : Corpus Folder\n"
            "\t - Arg [2] : Filename for files dictionnary (loads if exists or save if it doesn't)\n"
            "\t - Arg [3] : Filename for words dictionnary (loads if exists or save if it doesn't)\n"
            "\t - Arg [4] : Filename for index matrix  (loads if exists or save if it doesn't)\n");
        getchar();
        return -1;
    }
    printf("--- Corpus Folder : %s ---\n", argv[1]);
    dict_s* files = NULL;
    dict_h* words = NULL;
    index_matrix* matrix = NULL;
    //load if we already indexed
    if (does_file_exist(argv[2]) && does_file_exist(argv[3]) && does_file_exist(argv[4])) {
        printf("Loading files...\n");
        files = dict_s_load("\n", argv[2], false);
        words = dict_h_load("\n", argv[3]);
        matrix = matrix_load(argv[4]);
        printf("All files loaded succesfully !\n");
    }
    // if only the file that contains the filenames exists (index)
    else if (does_file_exist(argv[2])) {
        printf("Loading %s and indexing...\n", argv[2]);
        files = dict_s_load("\n", argv[2], false);
        words = dict_h_create(300000);
        matrix = matrix_create(files->size);
        index(files, words, matrix);
        dict_h_save(words, "\n", argv[3]);
        matrix_save(matrix, argv[4]);
        printf("Word 0 : %s\n", dict_h_get_word(words, 0));

    }
    // No filenames file
    // Need to reindex
    else {
        // filenames dict
        dir_s* directory = dir_s_create(argv[1]);
        dir_s_get_files_wt(directory, directory->path, false);
        dict_s_save(directory->files_dict, "\n", argv[2]);
        files = directory->files_dict;
        // indexation
        words = dict_h_create(300000);
        matrix = matrix_create(files->size);
        index(files, words, matrix);
        dict_h_save(words, "\n", argv[3]);
        matrix_save(matrix, argv[4]);

        printf("Word 0 : %s\n", dict_h_get_word(words, 0));
    }
    // matrix_display(matrix, 10);


    getchar();



    return 0;
}
