cmake_minimum_required(VERSION 3.12)

add_definitions(-DWITH_AVX2)
add_definitions(-mavx2)
set(CMAKE_CXX_STANDARD 11)

project(dotp)

add_executable(dotp main.c myGEMM.c myGEMM.h)

find_library(OPENBLAS_LIBRARY openblas)
target_link_libraries(dotp PRIVATE ${OPENBLAS_LIBRARY})

find_package(OpenMP)
if(OpenMP_CXX_FOUND)
    message("OpenMP found.")
    target_link_libraries(dotp PRIVATE OpenMP::OpenMP_CXX)
endif()


