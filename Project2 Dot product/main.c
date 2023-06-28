#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define VECTOR_SIZE 10000
#define NUM_EXPERIMENTS 10000

int main() {
    int i, j;
    float dot_product;
    float *vector1 = malloc(sizeof(float) * VECTOR_SIZE);
    float *vector2 = malloc(sizeof(float) * VECTOR_SIZE);
    double total_time = 0;

    for (j = 0; j < NUM_EXPERIMENTS; j++) {
        // Initialize vectors with random values
        srand(time(NULL)+j); // Use j as an offset to the current time
        for (i = 0; i < VECTOR_SIZE; i++) {
            vector1[i] = rand() / (float)RAND_MAX;
            vector2[i] = rand() / (float)RAND_MAX;
        }

        // Compute dot product
        clock_t start_time = clock();
        dot_product = 0;
        for (i = 0; i < VECTOR_SIZE; i++) {
            dot_product += vector1[i] * vector2[i];
        }
        clock_t end_time = clock();
        double time_taken = (double)(end_time - start_time) / CLOCKS_PER_SEC;
        total_time += time_taken;

        // Print dot product and time taken
        printf("Experiment %d: Dot product: %f, Time taken: %f seconds\n", j + 1, dot_product, time_taken);
    }

    // Print average time taken
    printf("Average time taken: %f seconds\n", total_time / NUM_EXPERIMENTS);

    // Free memory
    free(vector1);
    free(vector2);
    return 0;
}

