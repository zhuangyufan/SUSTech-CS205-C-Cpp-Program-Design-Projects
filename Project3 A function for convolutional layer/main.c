#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "conv.h"

#define TIME_START clock_t start, end; start = clock();
#define TIME_END(NAME) end = clock(); \
             double duration = ((double) (end - start)) / CLOCKS_PER_SEC * 1000.0; \
             printf("%s: output.data[0]=%f, duration = %.2fms\n", NAME, output.data[0], duration);

// Input parameters
#define INPUT_WIDTH 5
#define INPUT_HEIGHT 5
#define CHALLELS 3

// Kernal parameters
#define KERNEL_WIDTH 3      // should >= 3
#define KERNEL_HEIGHT 3     // should >= 3
#define CHANNELS_IN 3
#define CHANNELS_OUT 1

// Convolution parameters
#define PADDING 1
#define STRIDE 1

int main() {
    size_t output_width = (INPUT_WIDTH + 2 * PADDING - KERNEL_WIDTH) / STRIDE + 1;
    size_t output_height = (INPUT_HEIGHT + 2 * PADDING - KERNEL_HEIGHT) / STRIDE + 1;
    
    Image input = { malloc(INPUT_WIDTH * INPUT_HEIGHT * CHALLELS * sizeof(float)), INPUT_WIDTH, INPUT_HEIGHT, CHALLELS};
    for (int i = 0; i < INPUT_WIDTH * INPUT_HEIGHT * CHALLELS; i++) {
        input.data[i] = 1.0f;
        // input.data[i] = (float) rand() / RAND_MAX * 255;
    }
    Kernel kernel = { malloc(KERNEL_WIDTH * KERNEL_HEIGHT * CHANNELS_IN * sizeof(float)), KERNEL_WIDTH, KERNEL_HEIGHT, CHANNELS_IN, CHANNELS_OUT};
    for (int i = 0; i < 3 * 3 * 3; i++) {
        // kernel.data[i] = (float) rand() / RAND_MAX;
        kernel.data[i] = 1.0f;
    }
    Image output = { malloc(output_width * output_height * sizeof(float)), output_width, output_height, CHANNELS_OUT};   
    
    // convolve(&input, &kernel, &output, 1, 1);
    // convolve(&input, &kernel, &output, 1, 1);

    // convolve_avx2(&input, &kernel, &output, 1, 1);
    // convolve_avx2(&input, &kernel, &output, 1, 1);

    // convolve_avx2_omp(&input, &kernel, &output, 1, 1);
    // convolve_avx2_omp(&input, &kernel, &output, 1, 1);

    // TIME_START
    // convolve(&input, &kernel, &output, 1, 1);
    // TIME_END("Normal")

    // TIME_START
    // convolve_avx2(&input, &kernel, &output, 1, 1);
    // TIME_END("SIMD")

    TIME_START
    convolve_avx2_omp(&input, &kernel, &output, 1, 1);
    TIME_END("SIMD+openMP")
    
    // Print some statistics
    printf("Input:\n");
    for (int y = 0; y < input.height; y++) {
        for (int x = 0; x < input.width; x++) {
            printf("(%f, %f, %f) ", input.data[(y * input.width + x) * input.channels], input.data[(y * input.width + x) * input.channels + 1], input.data[(y * input.width + x) * input.channels + 2]);
        }
        printf("\n");
    }
    printf("Kernel:\n");
    for (int y = 0; y < kernel.height; y++) {
        for (int x = 0; x < kernel.width; x++) {
            printf("(%f, %f, %f) ", kernel.data[(y * kernel.width + x) * kernel.channels_out], kernel.data[(y * kernel.width + x) * kernel.channels_out + 1], kernel.data[(y * kernel.width + x) * kernel.channels_out + 2]);
        }
        printf("\n");
    }
    printf("Output:\n");
    for (int y = 0; y < output.height; y++) {
        for (int x = 0; x < output.width; x++) {
            printf("%f ", output.data[(y * output.width + x) * output.channels]);
        }
        printf("\n");
    }

    // Free memory
    free(input.data);
    free(kernel.data);
    free(output.data);
    return 0;
}