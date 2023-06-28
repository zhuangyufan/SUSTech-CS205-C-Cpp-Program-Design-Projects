#include <stdio.h>
#include <stdlib.h>
#include "conv.h"
#include <string.h>

#ifdef _OPENMP
#include <omp.h>
#endif

#ifdef WITH_AVX2
#include <immintrin.h>
#endif 

void convolve(Image* input, Kernel* kernel, Image* output, int padding, int stride) {
    if (input == NULL || kernel == NULL || output == NULL){
        printf("NULL Pointer!");
    }
    // Compute output dimensions
    int output_width = (input->width + 2 * padding - kernel->width) / stride + 1;       
    int output_height = (input->height + 2 * padding - kernel->height) / stride + 1;       
    // Loop over output feature map
    for (int out_channel = 0; out_channel < kernel->channels_out; out_channel++) {
        for (int out_y = 0; out_y < output_height; out_y++) {
            for (int out_x = 0; out_x < output_width; out_x++) {
                // Compute input region corresponding to output element
                int in_x_origin = out_x * stride - padding;
                int in_y_origin = out_y * stride - padding;
                // Loop over input region and kernel
                float sum = 0.0f;
                for (int in_channel = 0; in_channel < kernel->channels_in; in_channel++) {
                    for (int k_y = 0; k_y < kernel->height; k_y++) {
                        for (int k_x = 0; k_x < kernel->width; k_x++) {
                            int in_x = in_x_origin + k_x;
                            int in_y = in_y_origin + k_y;
                            if (in_x >= 0 && in_x < input->width && in_y >= 0 && in_y < input->height) {
                                float input_value = input->data[(in_y * input->width + in_x) * input->channels + in_channel];
                                float kernel_value = kernel->data[((kernel->height - k_y - 1) * kernel->width + kernel->width - k_x - 1) * kernel->channels_out + out_channel];
                                sum += input_value * kernel_value;
                            }
                        }
                    }
                }
                output->data[(out_y * output->width + out_x) * output->channels + out_channel] = sum;
            }
        }
    }
}

void convolve_avx2(Image* input, Kernel* kernel, Image* output, int padding, int stride) {
    #ifdef WITH_AVX2
    if (input == NULL || kernel == NULL || output == NULL){
        printf("NULL Pointer!");
    }
    // Compute output dimensions
    int output_width = (input->width + 2 * padding - kernel->width) / stride + 1;       
    int output_height = (input->height + 2 * padding - kernel->height) / stride + 1;    
    // Initialize registers and buffers
    float sum[8] = {0};
    __m256 a, b, c;
    float temp_buffer1[8] = {0};
    float temp_buffer2[8] = {0};
    // Loop over output feature map
    for (int out_channel = 0; out_channel < kernel->channels_out; out_channel++) {
        for (int out_y = 0; out_y < output_height; out_y++) {
            for (int out_x = 0; out_x < output_width; out_x++) {
                // Compute input region corresponding to output element
                int in_x_origin = out_x * stride - padding;
                int in_y_origin = out_y * stride - padding;
                // Loop over input region and kernel
                int count = 0;
                c = _mm256_setzero_ps();
                for (int in_channel = 0; in_channel < kernel->channels_in; in_channel++) {
                    for (int k_y = 0; k_y < kernel->height; k_y++) {
                        for (int k_x = 0; k_x < kernel->width; k_x++) {
                            int in_x = in_x_origin + k_x;
                            int in_y = in_y_origin + k_y;
                            if (in_x >= 0 && in_x < input->width && in_y >= 0 && in_y < input->height) {
                                // load values to temporary buffer
                                memcpy(&temp_buffer1[count % 8], &input->data[(in_y * input->width + in_x) * input->channels + in_channel], sizeof(float));
                                memcpy(&temp_buffer2[count % 8], &kernel->data[((kernel->height - k_y - 1) * kernel->width + kernel->width - k_x - 1) * kernel->channels_out + out_channel], sizeof(float));                              
                                count++;
                                // if the buffer has already been loaded with 8 values
                                if (count % 8 == 0){
                                    a = _mm256_loadu_ps(temp_buffer1);   // load the values in beffer to register
                                    b = _mm256_loadu_ps(temp_buffer2);
                                    c = _mm256_add_ps(c, _mm256_mul_ps(a, b));   // calculate and add to register c
                                }
                            }
                        }
                    }
                }
                // For the last buffer that may be not full
                if (count % 8 != 0){
                for (int i = 0; i < 8 - count % 8; i++){
                    temp_buffer1[7-i] = 0;
                    temp_buffer2[7-i] = 0;
                }
                a = _mm256_loadu_ps(temp_buffer1);   // load the values in beffer to register
                b = _mm256_loadu_ps(temp_buffer2);
                c = _mm256_add_ps(c, _mm256_mul_ps(a, b));   // calculate and add to register c 
                }
                _mm256_storeu_ps(sum, c);     // load to memory from register
                output->data[(out_y * output->width + out_x) * output->channels + out_channel] = sum[0]+sum[1]+sum[2]+sum[3]+sum[4]+sum[5]+sum[6]+sum[7];
            }
        }
    }
    #else
        printf("AVX2 is not supported");
    #endif
}

void convolve_avx2_omp(Image* input, Kernel* kernel, Image* output, int padding, int stride) {
    #ifdef WITH_AVX2
    if (input == NULL || kernel == NULL || output == NULL){
        printf("NULL Pointer!");
    }
    // Compute output dimensions
    int output_width = (input->width + 2 * padding - kernel->width) / stride + 1;       
    int output_height = (input->height + 2 * padding - kernel->height) / stride + 1;    
    // Initialize registers and buffers
    float sum[8] = {0};
    __m256 a, b, c;
    float temp_buffer1[8] = {0};
    float temp_buffer2[8] = {0};
    // Loop over output feature map
    #pragma omp parallel for
    for (int out_channel = 0; out_channel < kernel->channels_out; out_channel++) {
        for (int out_y = 0; out_y < output_height; out_y++) {
            for (int out_x = 0; out_x < output_width; out_x++) {
                // Compute input region corresponding to output element
                int in_x_origin = out_x * stride - padding;
                int in_y_origin = out_y * stride - padding;
                // Loop over input region and kernel
                int count = 0;
                c = _mm256_setzero_ps();
                for (int in_channel = 0; in_channel < kernel->channels_in; in_channel++) {
                    for (int k_y = 0; k_y < kernel->height; k_y++) {
                        for (int k_x = 0; k_x < kernel->width; k_x++) {
                            int in_x = in_x_origin + k_x;
                            int in_y = in_y_origin + k_y;
                            if (in_x >= 0 && in_x < input->width && in_y >= 0 && in_y < input->height) {
                                // load values to temporary buffer
                                memcpy(&temp_buffer1[count % 8], &input->data[(in_y * input->width + in_x) * input->channels + in_channel], sizeof(float));
                                memcpy(&temp_buffer2[count % 8], &kernel->data[((kernel->height - k_y - 1) * kernel->width + kernel->width - k_x - 1) * kernel->channels_out + out_channel], sizeof(float));                              
                                count++;
                                // if the buffer has already been loaded with 8 values
                                if (count % 8 == 0){
                                    a = _mm256_loadu_ps(temp_buffer1);   // load the values in beffer to register
                                    b = _mm256_loadu_ps(temp_buffer2);
                                    c = _mm256_add_ps(c, _mm256_mul_ps(a, b));   // calculate and add to register c
                                }
                            }
                        }
                    }
                }
                // For the last buffer that may be not full
                if (count % 8 != 0){
                for (int i = 0; i < 8 - count % 8; i++){
                    temp_buffer1[7-i] = 0;
                    temp_buffer2[7-i] = 0;
                }
                a = _mm256_loadu_ps(temp_buffer1);   // load the values in beffer to register
                b = _mm256_loadu_ps(temp_buffer2);
                c = _mm256_add_ps(c, _mm256_mul_ps(a, b));   // calculate and add to register c 
                }
                _mm256_storeu_ps(sum, c);     // load to memory from register
                output->data[(out_y * output->width + out_x) * output->channels + out_channel] = sum[0]+sum[1]+sum[2]+sum[3]+sum[4]+sum[5]+sum[6]+sum[7];
            }
        }
    }
    #else
        printf("AVX2 is not supported");
    #endif
}