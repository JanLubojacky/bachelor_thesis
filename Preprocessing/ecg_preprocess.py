import neurokit2 as nk
import numpy as np
from scipy.signal import resample, medfilt
import sys
import os
import config


def create_windows(ecg_signal, sample_rate, window_len):
    """Creates windows on ecg signal. Removes first elements, that do not fit into window

    Parameters
    ----------
    ecg_signal : np.array
        ECG signal
    sample_rate : int
        Sampling frequency of `ecg_signal`
    window_len : int
        Length of windows in seconds

    """
    split_num = int(len(ecg_signal) / (sample_rate * window_len))
    short = ecg_signal[len(ecg_signal) % int(sample_rate * window_len):]
    window = np.split(short, split_num)
    if np.isnan(np.sum(window)):
        return 1
        
    return window


def resample_signal(signal, old_sample_rate, new_sample_rate):
    new_num_samples = int(len(signal) / old_sample_rate) * new_sample_rate
    return resample(signal, new_num_samples)


def filter_drift(ecg_signal, sampling_rate):
    """
    high pass filter, cutoff frequency 1 Hz
    bw order 5
    """
    ecg_filter = nk.signal_filter(ecg_signal,
                                  lowcut=1,
                                  sampling_rate=sampling_rate,
                                  order=5)

    return ecg_filter


def filter_low_pass(ecg_signal, sampling_rate):
    """
    low pass filter, cutoff frequency (sampling_rate / 2) Hz
    bw order 5
    """

    ecg_filter = nk.signal_filter(ecg_signal,
                                  highcut=25,
                                  sampling_rate=sampling_rate,
                                  order=5)

    return ecg_filter


def filter_ecg_powerline(ecg_signal, sampling_rate, pwl_frequency):
    """
    powerline filtering for both europe and us specified by pwl_frequency
    """
    if pwl_frequency == 50:
        return(nk.signal_filter(ecg_signal,
                                sampling_rate,
                                method='powerline',
                                powerline=pwl_frequency,
                                order=5))
    elif pwl_frequency == 60:
        return nk.signal_filter(ecg_signal,
                                sampling_rate,
                                method='powerline',
                                powerline=pwl_frequency,
                                order=5)


def standatrdize_ecg(ecg_signal):
    return (ecg_signal - ecg_signal.mean()) / (ecg_signal.std())


def filter_ecg(ecg_signal, sampling_rate, frequency):
    ecg_signal = filter_ecg_powerline(ecg_signal, sampling_rate, frequency)
    ecg_signal = filter_low_pass(ecg_signal, sampling_rate)
    ecg_signal = filter_drift(ecg_signal, sampling_rate)

    return ecg_signal

