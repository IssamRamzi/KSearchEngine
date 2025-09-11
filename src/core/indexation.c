//
// Created by issbe on 09/09/2025.
//

#include "core/indexation.h"

#include "io/dir_s.h"
#include "io/file_reader_s.h"

pthread_mutex_t mutex;

dir_h *index_directory_h(char path[MAX_WORD_SIZE]) {
    clock_t start_time, end_time;
    start_time = clock();
    dir_h* directory_h = dir_h_create(path);
    if (directory_h == NULL) {
        perror("index_directory_h : ");
        return NULL;
    }
    dir_h_get_files_wt(directory_h, directory_h->path, false);
    end_time = clock();
    printf("Takes %ldms to read all files for dir_s and the size is %d\n",
           (end_time - start_time) * 1000 / CLOCKS_PER_SEC,
           directory_h->files_dict->size);

    return directory_h;
}

dir_s *index_directory_s(char path[MAX_WORD_SIZE]) {
    clock_t start_time, end_time;
    start_time = clock();
    dir_s* directory_s = dir_s_create(path);
    if (directory_s == NULL) {
        perror("idnex_directory_s : ");
        return NULL;

    }
    dir_s_get_files_wt(directory_s, directory_s->path, false);
    end_time = clock();
    printf("Takes %ldms to read all files for dir_s and the size is %d\n",
           (end_time - start_time) * 1000 / CLOCKS_PER_SEC,
           directory_s->files_dict->size);

    return directory_s;
}

dict_h *index_dictionnary_from_dir_h(dir_h* directory) {
    clock_t start_time, end_time;
    start_time = clock();
    dict_h* words = dict_h_create(300000);
    for (int i = 0; i < directory->files_dict->nodes_count; i++) {
        hash_node_s *node = directory->files_dict->nodes[i];
        while (node) {
            fr_append_to_dict_h(node->value, words);
        }
    }
    end_time = clock();
    printf("Words : %d, indexed in %ld\n", words->size, (end_time - start_time) * 1000 / CLOCKS_PER_SEC);
    return words;
}

typedef struct {
    dict_h* words;
    char** dict_chunk;
    int chunk_size;
    pthread_mutex_t* mutex;
} thread_data_t;

void* thread_worker(void* arg) {
    thread_data_t* data = (thread_data_t*)arg;
    for (int i = 0; i < data->chunk_size; i++) {
        if (data->dict_chunk[i]) {
            pthread_mutex_lock(data->mutex);
            fr_append_to_dict_h(data->dict_chunk[i], data->words);
            pthread_mutex_unlock(data->mutex);
        }
    }
    return NULL;
}

dict_h* index_dictionnary_from_dir_s(dir_s* directory) {
    clock_t start_time, end_time;
    start_time = clock();
    dict_h* words = dict_h_create(300000);


    const int num_threads = 2048;
    int chunk_size = (directory->files_dict->size + num_threads - 1) / num_threads;
    pthread_t threads[num_threads];
    thread_data_t thread_data[num_threads];
    pthread_mutex_init(&mutex, NULL);


    for (int i = 0; i < num_threads; i++) {
        int start = i * chunk_size;
        int end = (i + 1) * chunk_size;
        if (end > directory->files_dict->size) end = directory->files_dict->size;

        thread_data[i].words = words;
        thread_data[i].dict_chunk = &directory->files_dict->dict[start];
        thread_data[i].chunk_size = end - start;
        thread_data[i].mutex = &mutex;

        if (pthread_create(&threads[i], NULL, thread_worker, &thread_data[i]) != 0) {
            perror("pthread_create failed");
            pthread_mutex_destroy(&mutex);
            return NULL;
        }
    }

    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    end_time = clock();
    printf("Words : %d, indexed in %ldms\n", words->size, (end_time - start_time) * 1000 / CLOCKS_PER_SEC);
    return words;
}

// dict_h *index_dictionnary_from_dir_s(dir_s* directory) {
//     clock_t start_time, end_time;
//     start_time = clock();
//     dict_h* words = dict_h_create(300000);
//     for (int i = 0; i < directory->files_dict->size; i++) {
//         char *str = directory->files_dict->dict[i];
//         fr_append_to_dict_h(str, words);
//     }
//     end_time = clock();
//     printf("Words : %d, indexed in %ld\n", words->size, (end_time - start_time) * 1000 / CLOCKS_PER_SEC);
//     return words;
// }

