cmake_minimum_required(VERSION 3.16)

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>") #we want to use the static runtime
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Release>:Release>")

project(KSearchEngine C)

set(CMAKE_C_STANDARD 99)

include_directories(include)
add_executable(KSearchEngine src/main.c
        include/io/file_reader_s.h
        include/data/dict_s.h
        src/data/dict_s.c
        include/io/dir_s.h
        src/io/dir_s.c
        include/data/hash_table.h
        src/data/hash_table.c
        include/data/dict_h.h
        src/data/dict_h.c
        include/io/dir_h.h
        src/io/dir_h.c
        include/core/utils.h
        include/data/cell.h
        include/data/index_matrix.h
        src/data/index_matrix.c
        src/core/utils.c
        src/data/cell.c
        include/core/logger.h
        src/core/logger.c
        src/core/indexation.c
        include/core/indexation.h
        src/io/file_reader_s.c)

