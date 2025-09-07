cmake_minimum_required(VERSION 3.16)

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>") #we want to use the static runtime
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Release>:Release>")

project(KSearchEngine C)

set(CMAKE_C_STANDARD 99)

add_executable(KSearchEngine main.c
        src/include/file_reader_s.h
        src/include/dict_s.h
        src/dict_s.c
        src/include/dir_s.h
        src/dir_s.c
        src/include/hash_table.h
        src/hash_table.c
        src/include/dict_h.h
        src/dict_h.c
        src/include/dir_h.h
        src/dir_h.c
        src/include/utils.h
        src/include/cell.h
        src/include/index_matrix.h
        src/index_matrix.c
        src/include/include.h
        src/utils.c
        src/cell.c)

include_directories(src/include)
