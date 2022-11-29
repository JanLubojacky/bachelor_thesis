import config as cf
import os
import pandas as pd
import numpy as np
import neurokit2 as nk
import pickle
import ecg_preprocess as ep
from scipy.interpolate import interp1d
import re

def process_biomech_data(paths_to_data, window_length, filtering):
    '''
    processes the biomech lab dataset data
    input: path to data, length of signal windows, filtering flag
    output: none, processed data is saved to output folder
    '''
    cf.info('BIOMECH LAB', window_length, filtering)

    data_out = pd.DataFrame()

    for path_to_data in paths_to_data:
        files = os.listdir(path_to_data)

        if path_to_data == cf.BIOMECH_SOURCE_NEG:
                label = 0
        elif path_to_data == cf.BIOMECH_SOURCE_POS:
            label = 1

        for i in range(len(files)):
            file_to_read = os.path.join(path_to_data,files[i])
            ecg_orig = pd.read_csv(file_to_read)
            ecg_orig = ecg_orig.values.flatten()

            if filtering: # filter signal
                ecg_orig = ep.filter_ecg(ecg_orig, cf.BIOMECH_SAMPLE_RATE, cf.BIOMECH_PWL)

            ecg_down = ep.resample_signal(ecg_orig, cf.BIOMECH_SAMPLE_RATE, cf.TARGET_SAMPLE_RATE)

            if len(ecg_down) > (cf.TARGET_SAMPLE_RATE * window_length):
                
                ecg_win = ep.create_windows(ecg_down,
                                        cf.TARGET_SAMPLE_RATE, window_length)

                if ecg_win != 1:

                    df_tmp = pd.DataFrame({
                                    'ECG': ecg_win,
                                    'label': label
                    })

                    data_out = pd.concat([data_out, df_tmp], ignore_index=True)

            cf.progress_bar(path_to_data, i, len(files)-1)

    dst = os.path.join(cf.OUTPUT_DIR, f'BIOMECH_{window_length}s_{cf.TARGET_SAMPLE_RATE}Hz_prepro_{filtering}.pkl')
    # print(data_out)
    data_out.to_pickle(dst)
    print(f'\nBIOMECH processed, output in {cf.OUTPUT_DIR}')
        


def process_wesad_data(path_to_data, window_length, filtering):
    '''
    processed the wesad dataset data
    input: path to data, length of signal windows, filtering flag
    output: none, processed data is saved to output folder
    '''
    def load_wesad_dict(path):
        f = open(path, 'rb')
        p = pickle.load(f, encoding='latin1')
        f.close()
        return p

    def preprocess_wesad_dict(subject_dict, window_length):
        '''
        chops signal into windows, decimates the signal to target sample rate
        '''
        chest_sensor = subject_dict['signal']['chest']

        df_formated = pd.DataFrame({
            'ECG': chest_sensor['ECG'].flatten(),
            'label': subject_dict['label']
        })
        # resample signal

        ecg_orig = df_formated['ECG'].to_numpy()

        if filtering: # filter signal
            ecg_orig = ep.filter_ecg(ecg_orig, cf.RESPIBAN_SAMPLE_RATE, cf.WESAD_PWL)

        ecg_down = ep.resample_signal(ecg_orig, cf.RESPIBAN_SAMPLE_RATE, cf.TARGET_SAMPLE_RATE)

        # resample lables
        labels = df_formated['label'].to_numpy()

        xp = np.arange(0, len(labels),
                   cf.RESPIBAN_SAMPLE_RATE / cf.TARGET_SAMPLE_RATE)
        labels_down = interp1d(np.arange(len(labels)), labels, kind='nearest')(xp)

        df_prep = pd.DataFrame({'ECG': ecg_down, 'label': labels_down})

        df_neg = df_prep[df_prep['label'] == cf.WESAD_NEGATIVE[0]]
        df_pos = df_prep[df_prep['label'] == cf.WESAD_POSITIVE[0]]

        df_grouped = [df_neg, df_pos]

        data_windows = pd.DataFrame()

        for df in df_grouped:
            label = df['label'].values[0]

            if label in cf.WESAD_POSITIVE:
                model_label = 1
            elif label in cf.WESAD_NEGATIVE:
                model_label = 0

            if len(df['ECG']) > cf.TARGET_SAMPLE_RATE * window_length:

                ecg_win = ep.create_windows(df['ECG'].to_numpy(),
                                     cf.TARGET_SAMPLE_RATE, window_length)

                df_tmp = pd.DataFrame({
                    'ECG': ecg_win,
                    'label': model_label
                })

                data_windows = pd.concat([data_windows, df_tmp], ignore_index=True)

        return data_windows

    cf.info(path_to_data, window_length, filtering)

    parts = [
        i for i in os.listdir(path_to_data)
        if os.path.isdir(os.path.join(path_to_data, i))
    ]
    
    data_out = pd.DataFrame()

    for i, p in enumerate(parts):
        cf.progress_bar(p, i, len(parts)-1)

        src = os.path.join(path_to_data, p, f'{p}.pkl')

        subject_dict = load_wesad_dict(src)
        data_part = preprocess_wesad_dict(subject_dict, window_length)
        data_out = pd.concat([data_out, data_part], ignore_index=True)

    dst = os.path.join(cf.OUTPUT_DIR, f'WESAD_{window_length}s_{cf.TARGET_SAMPLE_RATE}Hz_prepro_{filtering}.pkl')
    data_out.to_pickle(dst)
    print(f'\nWESAD processed, output in {cf.OUTPUT_DIR}')


def process_clas_data(paths_to_data, window_length, filtering):
    cf.info(paths_to_data, window_length, filtering)

    bd_dir = os.path.join(paths_to_data, 'Block_details')
    part_dir = os.path.join(paths_to_data, 'Participants')

    bd_parts = os.listdir(bd_dir)
    bd_parts = sorted(bd_parts, key=lambda a: int(a.split('_')[0][4:]))

    data_out = pd.DataFrame()

    for i, b in enumerate(bd_parts):
        cf.progress_bar(b, i, len(bd_parts)-1)

        part_df = pd.DataFrame()
        part = b.split('_')[0]

        block_df = pd.read_csv(os.path.join(bd_dir, b))
        file_list = os.listdir(os.path.join(part_dir, part, 'by_block'))

        for _, row in block_df.iterrows():

            label = cf.CLAS_LABEL_STRINGS[row['Block Type']]
            # check if this is the ECG data we want
            if label in cf.CLAS_POSITIVE or label in cf.CLAS_NEGATIVE:
                # check the quality of the file
                quality = row['ECG Quality']

                if quality == cf.CLAS_ACCEPTABLE_QUALITY:
                    # find filename
                    block = row['Block']
                    r = re.compile(f'^{block}_ecg_')
                    file_name = list(filter(r.match, file_list))[0]

                    ecg_raw = pd.read_csv(
                        os.path.join(part_dir, part, 'by_block',
                                    file_name))['ecg2'].to_numpy()

                    if filtering: # filter signal
                        ecg_raw = ep.filter_ecg(ecg_raw, cf.CLAS_SAMPLING_RATE, cf.CLAS_PWL)

                    ecg_resample = ep.resample_signal(ecg_raw, cf.CLAS_SAMPLING_RATE,
                                                cf.TARGET_SAMPLE_RATE)


                    if len(ecg_resample) > window_length * cf.TARGET_SAMPLE_RATE:

                        ecg_win = ep.create_windows(ecg_resample,
                                                cf.TARGET_SAMPLE_RATE,
                                                window_length)

                        model_label = -1
                        if label in cf.CLAS_POSITIVE:
                            model_label = 1
                        elif label in cf.CLAS_NEGATIVE:
                            model_label = 0

                        if model_label != -1:
                            df_tmp = pd.DataFrame({
                                'ECG': ecg_win,
                                'label': model_label
                            })

                            part_df = pd.concat([part_df, df_tmp], ignore_index=True)

        data_out = pd.concat([data_out, part_df], ignore_index = True)

    dst = os.path.join(cf.OUTPUT_DIR, f'CLAS_{window_length}s_{cf.TARGET_SAMPLE_RATE}Hz_prepro_{filtering}.pkl')
    data_out.to_pickle(dst)
    print(f'\nCLAS processed, output in {cf.OUTPUT_DIR}')


if __name__ == "__main__":
    '''
    processed the wesad dataset data
    input: path to data, length of signal windows, filtering flag
    output: none, processed data is saved to output folder
    '''

    if not os.path.exists(cf.OUTPUT_DIR): os.makedirs(cf.OUTPUT_DIR)

    process_biomech_data(cf.BIOMECH_SOURCE, cf.WINDOW_LEN, cf.FILTER_FLAG)
    process_wesad_data(cf.WESAD_SOURCE, cf.WINDOW_LEN, cf.FILTER_FLAG)
    process_clas_data(cf.CLAS_SOURCE, cf.WINDOW_LEN, cf.FILTER_FLAG)
   