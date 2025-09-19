//
// Created by issbe on 09/09/2025.
//

#include "core/indexation.h"

#include "data/index_matrix.h"
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

void normalize_path(char* path) {
    for (int i = 0; path[i]; i++) {
        if (path[i] == '/') {
            path[i] = '\\';
        }
    }
}

void index(dict_s *files, dict_h *words, index_matrix *matrix) {
    printf("Number of files: %d\n", files->size);

    for (int i = 0; i < 10 && i < files->size; i++) {
        printf("File %d: %s\n", i, files->dict[i]);
    }

    clock_t start_time = clock();
    int processed_files = 0;
    int skipped_files = 0;

    for (int i = 0; i < files->size; i++) {
        char* file_path = files->dict[i];

        file_path[strcspn(file_path, "\n\r")] = '\0';

        file_reader* fr = fr_init(file_path);
        if (fr == NULL) {
            printf("Skipping file %d (failed to open): %s\n", i, file_path);
            skipped_files++;
            continue;
        }

        fr_start(fr);

        while (!fr_end(fr)) {
            fr_advance(fr);

            if (strlen(fr->mot) > 0) {
                dict_h_add_word(words, fr->mot, false);
                int word_idx = dict_h_get_index(words, fr->mot);
                matrix_increment(matrix, i, word_idx);
            }

            while (!fr_end(fr) && !isalpha((unsigned char)fr->c)) {
                fr->c = fgetc(fr->file);
            }
        }

        fr_free(fr);
        processed_files++;

        if ((i + 1) % 100 == 0) {
            printf("Processed %d/%d files...\n", i + 1, files->size);
        }
    }

    clock_t end_time = clock();

    printf("\n=== Indexing completed ===\n");
    printf("Total files: %d\n", files->size);
    printf("Processed files: %d\n", processed_files);
    printf("Skipped files: %d\n", skipped_files);
    printf("Dictionary size: %d words\n", words->size);
    printf("Processing time: %ldms\n", (end_time - start_time) * 1000 / CLOCKS_PER_SEC);

}