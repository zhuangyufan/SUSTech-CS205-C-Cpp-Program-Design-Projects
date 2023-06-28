#pragma once

typedef struct {
    float* data; // data is a 1D array, where width is contiguous
    int width;
    int height;
    int channels;
} Image;

typedef struct {
    float* data; // data is a 1D array, where width*height is contiguous
    int width;
    int height;
    int channels_in;
    int channels_out;
} Kernel;

void convolve(Image* input, Kernel* kernel, Image* output, int padding, int stride);
void convolve_avx2(Image* input, Kernel* kernel, Image* output, int padding, int stride);
void convolve_avx2_omp(Image* input, Kernel* kernel, Image* output, int padding, int stride);
