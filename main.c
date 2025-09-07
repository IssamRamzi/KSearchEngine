#include <stdio.h>
#include "dir_s.h"

#include <time.h>

#include "dir_h.h"
#include "file_reader_s.h"
#include "index_matrix.h"


int main(void) {
    clock_t start, end;
    dir_s* directory_s = dir_s_create("H:/french");
    start = clock();
    dir_s_get_files_wt(directory_s, directory_s->path, true);
    end = clock();
    printf("Takes %ldms to read all files for dir_s and the size is %d\n",
           (end - start) * 1000 / CLOCKS_PER_SEC,
           directory_s->files_dict->size);


    for (int i = 0; i < 10 && i < directory_s->files_dict->size; i++) {
        printf("File %d: %s\n", i, directory_s->files_dict->dict[i]);
    }

    // dir_s* directory_s2 = dir_s_create("H:/french");
    // start = clock();
    // dir_s_get_files_wt(directory_s2, directory_s2->path, true);
    // end = clock();
    // printf("Takes %ldms to read all files for dir_s and the size is %d\n",
    //        (end - start) * 1000 / CLOCKS_PER_SEC,
    //        directory_s2->files_dict->size);

    //
    // for (int i = 0; i < 10 && i < directory_s2->files_dict->size; i++) {
    //     printf("File %d: %s\n", i, directory_s2->files_dict->dict[i]);
    // }



    // dir_s* directory_s = dir_s_create("H:/corpus/files");
    // start = clock();
    // dir_s_get_files_wt(directory_s, directory_s->path);
    // end = clock();
    // printf("Takes %ldms to read all files for dir_h and the size is %d\n", (end - start) * 1000 / CLOCKS_PER_SEC, directory_s->files_dict->size);

    // dict_h_display(directory_h->files_dict, "\n", 10);


    // start = clock();
    // dict_h *words = dict_h_create(3000);
    // for (int i = 0; i < directory_h->files_dict->nodes_count; i++ ) {
    //     if (directory_h->files_dict->nodes[i]->value != NULL) {
    //         // printf("Fichier : %s\n", directory_h->files_dict->nodes[i]->value);
    //         fr_append_to_dict_h(directory_h->files_dict->nodes[i]->value, words);
    //     }
    // }
    // end = clock();


    // start = clock();
    // dict_h *words = dict_h_create(300000);
    // for (int i = 0; i < directory_h->files_dict->nodes_count; i++ ) {
    //     if (directory_h->files_dict->nodes[i]->value != NULL) {
    //         // printf("Fichier : %s\n", directory_h->files_dict->nodes[i]->value);
    //         fr_append_to_dict_h(directory_h->files_dict->nodes[i]->value, words);
    //     }
    // }
    // end = clock();
    //
    // //dict_h_save(words, "\n", "dictionnaire.txt");
    // printf("Elements : %d, %ldo\n", words->size, sizeof(words));
    // printf("Takes %ldms to create the dictionnary\n", (end - start) * 1000 / CLOCKS_PER_SEC);
    //
    // index_matrix* matrix = matrix_create(directory_h->files_dict->size);
    // for (int i = 0; i < directory_h->files_dict->nodes_count;i++) {
    //     di
    // }

    getchar();



    return 0;
}
