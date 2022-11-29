from enum import Enum
import os

'''serial port for connection to the board'''
# port = 'COM11' # windows
port = '/dev/ttyS11' # windows subsytem for linux
# port = "/dev/ttyACM0" # linux

RESPIBAN_SAMPLE_RATE = 700
CLAS_SAMPLING_RATE = 256
BIOMECH_SAMPLE_RATE = 500
TARGET_SAMPLE_RATE = 500
WINDOW_LEN = 5
FILTER_FLAG = False
# Powerline noise for each signal
# Europe: biomech(czech rep.), clas(bulgaria) - 50Hz
# US: WESAD - 60Hz
BIOMECH_PWL = 50
CLAS_PWL = 50
WESAD_PWL = 60

CLAS_ACCEPTABLE_QUALITY = 1.0

CLAS_SOURCE = 'CLAS'
BIOMECH_SOURCE_NEG = os.path.join('ecg_data_lubojacky','data','rest')
BIOMECH_SOURCE_POS = os.path.join('ecg_data_lubojacky','data','stroop')
BIOMECH_SOURCE = [BIOMECH_SOURCE_NEG, BIOMECH_SOURCE_POS]
WESAD_SOURCE = 'WESAD'

OUTPUT_DIR = f'./output_data/{WINDOW_LEN}s_{TARGET_SAMPLE_RATE}Hz_{FILTER_FLAG}filt_new'

class WesadLabel(Enum):
    NOT_DEFINED = 0
    BASELINE = 1
    STRESS = 2
    FUN = 3
    MEDI = 4
    IGNORE_1 = 5
    IGNORE_2 = 6
    IGNORE_3 = 7

WESAD_NEGATIVE = [WesadLabel.BASELINE.value]
WESAD_POSITIVE = [WesadLabel.STRESS.value]

class ClasLabel(Enum):
    BASELINE = 0
    NEUTRAL = 1
    VIDEO_CLIP = 2
    MATH_TEST = 3
    MATH_TEST_RESPONSE = 4
    PICTURES = 5
    STROOP_TEST = 6
    STROOP_TEST_RESPONSE = 7
    IQ_TEST = 8
    IQ_TEST_RESPONSE = 9

CLAS_LABEL_STRINGS = {
    'Baseline': 0,
    'Neutral': 1,
    'Video clip': 2,
    'Math Test': 3,
    'Math Test Response': 4,
    'Pictures': 5,
    'Stroop Test': 6,
    'Stroop Test Response': 7,
    'IQ Test': 8,
    'IQ Test Response': 9
}

# CLAS_POSITIVE = [
#     ClasLabel.MATH_TEST.value, ClasLabel.STROOP_TEST.value,
#     ClasLabel.IQ_TEST.value
# ]
CLAS_POSITIVE = [ClasLabel.STROOP_TEST.value, ClasLabel.MATH_TEST.value]
# CLAS_NEGATIVE = [ClasLabel.BASELINE.value, ClasLabel.NEUTRAL.value]
CLAS_NEGATIVE = [ClasLabel.BASELINE.value]

def info(paths_to_data, window_len, filtering):
    '''
    logs information about preprocessing progress
    '''
    print(f'Processing dataset {paths_to_data}')
    print(f'Sample rate {TARGET_SAMPLE_RATE}')
    print(f'Window length {window_len}')
    if filtering: print('Filtering on')
    else: print('Filtering off')

def progress_bar(name, count, total, suffix=''):
    '''
    adapted from: https://stackoverflow.com/questions/3173320/text-progress-bar-in-terminal-with-block-characters
    '''
    bar_len = 40
    filled_len = round(bar_len * count / float(total))

    percents = round(100.0 * count / float(total), 1)
    bar = '#' * filled_len + ('-' * (bar_len - filled_len))

    print('%s [%s] %s%s ...%s' % (name, bar, percents, '%', suffix), end='\r')