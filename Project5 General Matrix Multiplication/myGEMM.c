#include "myGEMM.h"

void cblas_dgemm_my(const enum CBLAS_ORDER Order, const enum CBLAS_TRANSPOSE TransA, const enum CBLAS_TRANSPOSE TransB,
                 const int M, const int N, const int K, const double alpha, const double *A, const int lda,
                 const double *B, const int ldb, const double beta, double *C, const int ldc) {
    int i, j, k;

    if (Order == CblasRowMajor) {
        if (TransA == CblasNoTrans) {
            if (TransB == CblasNoTrans) {
                #pragma omp parallel for
                for (i = 0; i < M; i++) {
                    for (j = 0; j < N; j++) {
                        double temp = 0.0;
                        for (k = 0; k < K; k++) {
                            temp += alpha * A[i * lda + k] * B[k * ldb + j];
                        }
                        C[i * ldc + j] = temp + beta * C[i * ldc + j];
                    }
                }
            } else {
                #pragma omp parallel for
                for (i = 0; i < M; i++) {
                    for (j = 0; j < N; j++) {
                        if (K % 4 == 0){
                            __m256d a, b;
                            __m256d c = _mm256_setzero_pd();
                            double sum[4] = {0};
                            const double* pa = &A[i * lda];
                            const double* pb = &B[j * ldb];                          
                            for (size_t r = 0; r < K; r += 4) {
                                a = _mm256_loadu_pd(pa + r);      // load to register
                                b = _mm256_loadu_pd(pb + r);
                                c = _mm256_add_pd(c, _mm256_mul_pd(a, b));  // calculate and add to register c
                            }
                            _mm256_storeu_pd(sum, c);     // load to memory from register 
                            C[i * ldc + j] = alpha * (sum[0]+sum[1]+sum[2]+sum[3]) + beta * C[i * ldc + j];
                        }
                        else{
                            printf("K can not divided by 4.");
                        }
                    }
                }
            }
        } else {
            if (TransB == CblasNoTrans) {
                #pragma omp parallel for
                for (i = 0; i < M; i++) {
                    for (j = 0; j < N; j++) {
                        double temp = 0.0;
                        for (k = 0; k < K; k++) {
                            temp += alpha * A[k * lda + i] * B[k * ldb + j];
                        }
                        C[i * ldc + j] = temp + beta * C[i * ldc + j];
                    }
                }
            } else {
                #pragma omp parallel for
                for (i = 0; i < M; i++) {
                    for (j = 0; j < N; j++) {
                        double temp = 0.0;
                        for (k = 0; k < K; k++) {
                            temp += alpha * A[k * lda + i] * B[j * ldb + k];
                        }
                        C[i * ldc + j] = temp + beta * C[i * ldc + j];
                    }
                }
            }
        }
    } else {
        if (TransA == CblasNoTrans) {
            if (TransB == CblasNoTrans) {
                #pragma omp parallel for
                for (j = 0; j < N; j++) {
                    for (i = 0; i < M; i++) {
                        double temp = 0.0;
                        for (k = 0; k < K; k++) {
                            temp += alpha * A[k * lda + i] * B[j * ldb + k];
                        }
                        C[j * ldc + i] = temp + beta * C[j * ldc + i];
                    }
                }
            } else {
                #pragma omp parallel for
                for (j = 0; j < N; j++) {
                    for (i = 0; i < M; i++) {
                        double temp = 0.0;
                        for (k = 0; k < K; k++) {
                            temp += alpha * A[i * lda + k] * B[j * ldb + k];
                        }
                        C[j * ldc + i] = temp + beta * C[j * ldc + i];
                    }
                }
            }
        } else {
            if (TransB == CblasNoTrans) {
                #pragma omp parallel for
                for (j = 0; j < N; j++) {
                    for (i = 0; i < M; i++) {
                        if (K % 4 == 0){
                            __m256d a, b;
                            __m256d c = _mm256_setzero_pd();
                            double sum[4] = {0};
                            const double* pa = &A[i * lda];
                            const double* pb = &B[j * ldb];                          
                            for (size_t r = 0; r < K; r += 4) {
                                a = _mm256_loadu_pd(pa + r);      // load to register
                                b = _mm256_loadu_pd(pb + r);
                                c = _mm256_add_pd(c, _mm256_mul_pd(a, b));  // calculate and add to register c
                            }
                            _mm256_storeu_pd(sum, c);     // load to memory from register 
                            C[j * ldc + i] = alpha * (sum[0]+sum[1]+sum[2]+sum[3]) + beta * C[j * ldc + i];
                        }
                        else{
                            printf("K can not divided by 4.");
                        }
                    }
                }
            } else {
                #pragma omp parallel for
                for (j = 0; j < N; j++) {
                    for (i = 0; i < M; i++) {
                        double temp = 0.0;
                        for (k = 0; k < K; k++) {
                            temp += alpha * A[i * lda + k] * B[k * ldb + j];
                        }
                        C[j * ldc + i] = temp + beta * C[j * ldc + i];
                    }
                }
            }
        }
    }
}

const double calcAccuError(double* C1, double* C2,int M, int N){
    double accuError = 0.0;
    for (int i = 0; i < M * N; i++){
        accuError += fabs(C1[i] - C2[i]);
    }
    return accuError;
}

const int testCorrectness(double* C1, double* C2, int M, int N){
    for (int i = 0; i < M * N; i++){
        if(C1[i] != C2[i]){
            return 0;
        }
    }
    return 1;
}

void printMatrix(const double* A, const int ROW, const int COL, const enum CBLAS_ORDER Order){
    if (Order == CblasRowMajor){
        for (int y = 0; y < ROW; y++) {
            for (int x = 0; x < COL; x++) {
                printf("%f ", A[y * ROW + x]);
            }
            printf("\n");
        }
    }
    else{
        for (int x = 0; x < ROW; x++) {
            for (int y = 0; y < COL; y++) {
                printf("%f ", A[y * ROW + x]);
            }
            printf("\n");
        }
    }
}