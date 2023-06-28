#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <cblas.h>
#include <omp.h>
#include <immintrin.h>
#include <string.h>
#include<math.h>

void cblas_dgemm_my(const enum CBLAS_ORDER Order, const enum CBLAS_TRANSPOSE TransA, const enum CBLAS_TRANSPOSE TransB,
                 const int M, const int N, const int K, const double alpha, const double *A, const int lda,
                 const double *B, const int ldb, const double beta, double *C, const int ldc);

const int testCorrectness(double* C1, double* C2, int M, int N);

void printMatrix(const double* A, const int ROW, const int COL, const enum CBLAS_ORDER Order);

const double calcAccuError(double* C1, double* C2,int M, int N);
