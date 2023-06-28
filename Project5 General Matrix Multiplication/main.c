#include "myGEMM.h"


int main() {
    int M = 2000;
    int N = 2000;
    int K = 2000;

    double *A = (double *)malloc(M * K * sizeof(double));
    double *B = (double *)malloc(K * N * sizeof(double));
    double *C1 = (double *)malloc(M * N * sizeof(double));
    double *C2 = (double *)malloc(M * N * sizeof(double));

    // Initialize A, B, and C with random values
    srand(time(NULL));
    for (int i = 0; i < M * K; i++) {
        A[i] = (double)rand() / RAND_MAX;
    }
    for (int i = 0; i < K * N; i++) {
        B[i] = (double)rand() / RAND_MAX;
    }
    for (int i = 0; i < M * N; i++) {
        C1[i] = (double)rand() / RAND_MAX;
    }
    memcpy(C2, C1, M * N * sizeof(double));


    double alpha = 1.0;
    double beta = 1.0;
    int numIterations = 1;


    // Test my implementation's performance
    clock_t start = clock();
    for (int i = 0; i < numIterations; i++) {
        cblas_dgemm_my(CblasRowMajor, CblasNoTrans, CblasTrans, M, N, K, alpha, A, K, B, N, beta, C1, N);
    }
    clock_t end = clock();
    double cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC / numIterations;
    printf("Time taken by my GEMM function: %f seconds\n", cpu_time_used);

    
    // Test OpenBLAS performance
    start = clock();
    for (int i = 0; i < numIterations; i++) {
        cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasTrans, M, N, K, alpha, A, K, B, N, beta, C2, N);
    }
    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC / numIterations;
    printf("Time taken by OpenBLAS GEMM function: %f seconds\n", cpu_time_used);


    // Test if our result is correct
    printf("The 1-norm of C1-C2 is %f.\n", calcAccuError(C1, C2, M, N));

    free(A);
    free(B);
    free(C1);
    free(C2);

    return 0;
}
