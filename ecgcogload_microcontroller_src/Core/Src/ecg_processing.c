#include "ecg_processing.h"

void filt_and_downsample(float* arr_downsample, float* arr_oversample, int arr_oversample_len, int sample_freq, int downsample_freq)
// filters the signal with a low pass filter for a frequency safely below the
// nyquist frequency for the downsampled signal and then performs the downsampling
{
    BWHighPass* bw_hp_1 = create_bw_high_pass_filter(2, sample_freq, high_pass_cutoff);
    BWHighPass* bw_hp_2 = create_bw_high_pass_filter(2, sample_freq, high_pass_cutoff);
    CHELowPass* che_lp_1 = create_che_low_pass_filter(4, 0.5, sample_freq, low_pass_cutoff);
    CHELowPass* che_lp_2 = create_che_low_pass_filter(4, 0.5, sample_freq, low_pass_cutoff);

    int sample_counter = 0;
    int sample_num = 0;
    int downsample_idx = sample_freq/downsample_freq;

    for(int i=0;i<arr_oversample_len;i++){
        // low pass filtering
        arr_oversample[i] = che_low_pass(che_lp_1, arr_oversample[i]);
        arr_oversample[i] = che_low_pass(che_lp_2, arr_oversample[i]);
        arr_oversample[i] = bw_high_pass(bw_hp_1, arr_oversample[i]);
        arr_oversample[i] = bw_high_pass(bw_hp_2, arr_oversample[i]);
        sample_counter++;

        // SAMPLE
        if(sample_counter == downsample_idx){
            sample_counter = 0;
            arr_downsample[sample_num] = arr_oversample[i];
            sample_num++;
        }
    }

    free_che_low_pass(che_lp_1);
    free_che_low_pass(che_lp_2);
    free_bw_high_pass(bw_hp_1);
    free_bw_high_pass(bw_hp_2);
}

float mean_rr_interval(int* peaks_indicies, int peak_count)
{
    float diff = 0;

    for(int i=0;i<peak_count-1;i++){
        diff += peaks_indicies[i+1] - peaks_indicies[i]; 
    }

    float mean_rr_int = diff / (peak_count - 1);

    return mean_rr_int;
}

float hear_rate(int sampling_frequency, float mean_rr_int)
{
	float hr_ = 60 * sampling_frequency/mean_rr_int;
    return hr_;
}

float SDNN(int* peaks_sample_num, int peaks_count, float mean_rr_int)
{
    float sum = 0;

    for(int i=0;i<peaks_count;i++){
        sum += pow(((float) peaks_sample_num[i] - mean_rr_int), 2);
    }

    return sqrt(sum / (float) peaks_count);
}

float RMSSD(int* peaks_sample_num, int peaks_count)
{
    float sum = 0;

    for(int i=0;i< (peaks_count - 1);i++){
        sum += pow((peaks_sample_num[i+1] - peaks_sample_num[i]), 2);
    }

    return sqrt(sum / (float) (peaks_count - 1));
}

float min_f(float* arr, int arr_oversamplelen)
{
    float min_num = arr[0];

    for(int i=1; i<arr_oversamplelen;i++){
        if(arr[i] < min_num){
            min_num = arr[i];
        }
    }

    return min_num;
}

float max_f(float* arr, int arr_oversamplelen)
{
    float max_num = arr[0];

    for(int i=1; i<arr_oversamplelen;i++){
        if(arr[i] > max_num){
            max_num = arr[i];
        }
    }
    
    return max_num;
}

void min_max_normalization(float* arr, int arr_oversamplelen)
{
    float arr_oversamplemin = min_f(arr, arr_oversamplelen);
    float arr_oversamplemax = max_f(arr, arr_oversamplelen);
    float minmax_diff = arr_oversamplemax - arr_oversamplemin;

    for(int i=0; i<arr_oversamplelen;i++){
        arr[i] = (arr[i] - arr_oversamplemin)/(minmax_diff);
    }
}

void standardization(float* arr, int arr_oversamplelen)
{
    float mean = 0.0    ;
    float sigma = 0.0;

    for(int i=0;i<arr_oversamplelen;i++){
        mean += arr[i];
    }

    mean /= arr_oversamplelen;

    for(int i=0;i<arr_oversamplelen;i++){
        sigma += pow(arr[i] - mean,2);
    }

    sigma /= arr_oversamplelen;

    for(int i=0;i<arr_oversamplelen;i++){
        arr[i] = (arr[i] - mean)/sigma;
    }
}

int r_peak_detection(float* signal, int signal_lenght, int sampling_freq, UART_HandleTypeDef huart2)
{
    int peaks_sample_number[20] = {0};
    int peak_count = 0;
    int last_sample_peak = False;
    int repolarization = False;
    int repolarization_count = 40;
    float detection_treshold = 0.713;

    char buf[100];
    int buf_len = 0;

    for(int i=0;i<signal_lenght;i++){
        if(repolarization == True){
            repolarization_count--;

            if(repolarization_count == 0){
                repolarization = False;
                repolarization_count = 40;
            }
        }
        else if(repolarization == False){
            if((signal[i] > detection_treshold) && (last_sample_peak == False)){
                last_sample_peak = True;
                peaks_sample_number[peak_count] = i;
                repolarization = True;
                peak_count++;
            }
            else if(signal[i] < detection_treshold){
                last_sample_peak = False;
            }
        }
    }

    if(peak_count == 0 || peak_count == 1){
        // UNSUCCESFUL DETECTION
    	buf_len = sprintf(buf, "R PEAK DETECTION FAILED!\n");
    	HAL_UART_Transmit(&huart2, (uint8_t *)buf, buf_len, 100);
        return 1;
    }

    float mean_rr_int = mean_rr_interval(peaks_sample_number, peak_count);
    float hr = hear_rate(sampling_freq, mean_rr_int);
    float sdnn = SDNN(peaks_sample_number, peak_count, mean_rr_int);
    float rmssd = RMSSD(peaks_sample_number, peak_count);

    // TADY SE DA VSECHNO ROVNOU POSLAT DO UARTU ASI;
    // mean rr interval is converted to seconds here
    buf_len = sprintf(buf, "HR: %.2f bpm, MEAN_RR_INT %.2f ms, SDNN %.2f ms, RMSSD %.2f ms\n", hr, mean_rr_int, sdnn, rmssd);
    HAL_UART_Transmit(&huart2, (uint8_t *)buf, buf_len, 100);



    return 0;
}
