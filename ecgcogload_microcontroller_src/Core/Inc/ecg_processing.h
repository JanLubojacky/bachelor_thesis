#ifndef ecg_processing_h
#define ecg_processing_h

#if __cplusplus
extern "C"{
#endif

#include "stm32f3xx_hal.h"
#include "filter.h"
#include <math.h>
#include <stdio.h>

#define False 0
#define True 1
#define high_pass_cutoff 0.5
#define low_pass_cutoff 40

void filt_and_downsample(float* arr_downsample, float* arr_oversample, int arr_oversample_len, int sample_freq, int downsample_freq);
/* filters and down samples the signal to the target sampling rate */

float mean_rr_interval(int* peaks_indicies, int peak_count);
/* calculates the mean rr interval */

float hear_rate(int sampling_frequency, float mean_rr_int);
/* estimates the heart rate from the mean rr interval and sampling rate */

float SDNN(int* peaks_sample_num, int peaks_count, float mean_rr_int);

float RMSSD(int* peaks_sample_num, int peaks_count);

int r_peak_detection(float* signal, int signal_lenght, int sampling_freq, UART_HandleTypeDef huart2);
/* performs the detection of R peaks and computes all the parameters */

float min_f(float* arr, int arr_len);
/* finds the minimum of an array */

float max_f(float* arr, int arr_len);
/* 
    finds the maximum of an array 
*/

void min_max_normalization(float* arr, int arr_len);
/* 
    performs the min-max normalization of an array
*/

void standardization(float* arr, int arr_len);
/*
    standardizes the array
*/

#if __cplusplus
}
#endif

#endif /* ecg_processing_h */
