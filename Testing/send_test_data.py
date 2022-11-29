import serial
import numpy as np
import time
import pickle
from sklearn.metrics import f1_score, confusion_matrix
import seaborn as sn
import pandas as pd
import matplotlib.pyplot as plt
import threading
import sys

def recieve_message():
    # read data comming from microcontroller
    while(True):
        if (channel.inWaiting() > 0):
            data_str = channel.readline().decode('utf-8')

            if "prediction" in data_str:
                print(data_str)
                output = round(float(data_str[12:])) 
                predictions.append(output)
            if "HR" in data_str:
                print(data_str)

        if testing_done:
            break

        time.sleep(0.01)


def send_data(test_len):
    with open('testdata5s500Hzraw.pkl', 'rb') as handle:
        test_dic = pickle.load(handle)

    test_ecgs = test_dic['ECG']

    test_labels = test_dic['label'][:test_len]

    print("Sending data!")

    for sample in range(test_len):
        test_ecg = test_ecgs[sample]
        # this is neccesarry to guarantee the fixed message lenght
        test_ecg = np.round(test_ecg, 5)

        for j in range(len(test_ecg)):
            send_data = str(test_ecg[j])

            while len(send_data) != 9:
                send_data += '0';
            send_data += 'x' # append separating character

            channel.write(send_data.encode())

            # time.sleep(0.002)

        print(f"sample {sample+1} sent!")

    return test_labels
    
def calculate_metrics(test_labels, predictions):
    cmtx = confusion_matrix(test_labels, predictions)
    tn = cmtx[0][0]
    fp = cmtx[0][1]
    fn = cmtx[1][0]
    tp = cmtx[1][1]

    accuracy = (tn + tp) / (tn + tp + fn + fp)
    sensitivity = tp / (tp + fn)
    specificity = tn / (tn + fp)
    precision = tp / (tp + fp)

    print('accuracy: ', accuracy)
    print('sensitivity: ', sensitivity)
    print('specificity: ', specificity)
    print('precision: ', precision)

    df_cm = pd.DataFrame(cmtx, range(2), range(2))
    plt.figure(figsize=(10,7))
    sn.set(font_scale=1.4) # for label size
    sn.heatmap(df_cm, annot=True, annot_kws={"size": 16}, fmt = 'd') # font size
    plt.xlabel('predictions')
    plt.ylabel('ground truth')
    plt.savefig('confusion_matrix.png')


if __name__ == "__main__":

    '''settings'''
    test_len = 5
    # port = 'COM11' # COM port example, for windows
    # port = '/dev/ttyS11' # windows subsytem for linux
    port = "/dev/ttyACM0" # linux

    for i in sys.argv:
        if "--samples=" in i:
            test_len = int(i[10:])
        if "--port=" in i:
            port = i[7:]

    print(f"Sending {test_len} samples!")

    try:
        channel = serial.Serial(port, baudrate = 115200, bytesize = serial.EIGHTBITS, parity = serial.PARITY_NONE, stopbits = serial.STOPBITS_ONE,timeout=None)
    except serial.serialutil.SerialException:
        print(f"ERROR: CANNOT OPEN SERIAL PORT {port}!")
        sys.exit()

    print(f"Succesfully connected to {port}!")

    testing_done = False
    predictions = []

    read_thread = threading.Thread(target=recieve_message)

    read_thread.start()
    ground_truth = send_data(test_len)
    time.sleep(3)
    testing_done = True
    read_thread.join()

    calculate_metrics(ground_truth, predictions)
