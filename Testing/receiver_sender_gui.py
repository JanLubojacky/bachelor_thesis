import serial
import time
import sys
import matplotlib
import pickle
matplotlib.use('Qt5Agg')
from matplotlib.backends.backend_qt5agg import FigureCanvasQTAgg as FigureCanvas
from matplotlib.figure import Figure
from PyQt5.QtCore import Qt, QObject, pyqtSignal, QThreadPool
from PyQt5.QtGui import QFont
from PyQt5.QtWidgets import (
    QApplication,
    QLabel,
    QMainWindow,
    QPushButton,
    QVBoxLayout,
    QWidget,
)

# Create a matplotlib window
class MplCanvas(FigureCanvas):

    def __init__(self, parent=None, width=5, height=4, dpi=100):
        fig = Figure(figsize=(width, height), dpi=dpi)
        fig.subplots_adjust(hspace=1)
        self.axes1 = fig.add_subplot(211)
        self.axes2 = fig.add_subplot(212)
        super(MplCanvas, self).__init__(fig)

# Create a worker class
class Worker_send(QObject):
    progress_samples = pyqtSignal(int)
    finished = pyqtSignal()
    message = pyqtSignal(str)

    def send_test_data(self):
        '''file with test data'''
        with open('testdata5s500Hzraw.pkl', 'rb') as handle:
            test_dic = pickle.load(handle)

        test_ecgs = test_dic['ECG']

        self.message.emit("SENDING DATA!")
        
        for i in range(test_len):

            test_ecg = test_ecgs[i]

            # send test data to microcontroller
            for j in range(len(test_ecg)):

                send_data = round(test_ecg[j],4)
                send_data = str(send_data)

                while len(send_data) != 9:
                    send_data += '0';
                send_data += 'x' # append separating character

                channel.write(send_data.encode())
                
            self.progress_samples.emit(i+1)

        self.message.emit("Run testing")
        self.finished.emit()

class Worker_receive(QObject):
    '''class for the receiving thread'''
    finished = pyqtSignal()
    progress = pyqtSignal(str)
    hrparams = pyqtSignal(str)
    prediction = pyqtSignal(str)
    add_pred_point = pyqtSignal(float)
    add_hr_point = pyqtSignal(float)
    message = pyqtSignal(str)


    def receive_message(self):
        ''' read data comming from microcontroller '''

        no_activity = 0
        self.progress.emit("Receiving data!")

        while(1):
            if (channel.inWaiting() > 0):
                no_activity = 0
                data_str = channel.readline().decode('utf-8')

                # We can also print every message in terminal for debuggin purposes,
                # uncomment if necessary

                # print("************")
                # print(data_str)
                # print("************")

                if "prediction" in data_str:
                    try:
                        pred = float(data_str[12:])
                        output = round(pred)

                        if output == 1:
                            self.prediction.emit(f"Cognitive load HIGH")

                        elif output == 0:
                            self.prediction.emit(f"Cognitive load LOW")

                        self.add_pred_point.emit(output)

                    except ValueError:
                        pass

                elif "HR:" in data_str:
                    self.hrparams.emit(data_str)
                    hr = float(data_str[4:10])
                    self.add_hr_point.emit(hr)
                else:
                    self.progress.emit("Unexpected message:" + data_str)

            elif no_activity == 700:
                self.progress.emit(f"7 seconds of no messages, receiveing stopped!")
                self.prediction.emit("prediction:")
                self.hrparams.emit("HR: , MEAN_RR_INT: , SDNN: , RMSSD: ")
                break
            else:
                no_activity += 1
                time.sleep(0.01)
                
        self.finished.emit()
    

class Window(QMainWindow):
    '''
        Main window of the GUI
    '''
    def __init__(self, parent=None):
        super().__init__(parent)
        # basic setup
        self.setWindowTitle("ECG APP sender + receiver")
        self.resize(1000, 700)
        self.centralWidget = QWidget()
        self.setCentralWidget(self.centralWidget)
        self.setup_gui()
        self.threadpool = QThreadPool()
        

    def setup_gui(self):
        '''setup for the GUI'''

        # define labels and buttons
        self.receive_label = QLabel("Run receiving!")
        self.receive_label.setAlignment(Qt.AlignHCenter | Qt.AlignVCenter)
        self.receive_label.setFont(QFont('Ubuntu',20))
        self.prediction_label = QLabel("prediction:", self)
        self.prediction_label.setAlignment(Qt.AlignHCenter | Qt.AlignVCenter)
        self.prediction_label.setFont(QFont('Ubuntu',16))
        self.hr_params_label = QLabel("HR: , MEAN_RR_INT: , SDNN: , RMSSD: ", self)
        self.hr_params_label.setAlignment(Qt.AlignHCenter | Qt.AlignVCenter)
        self.hr_params_label.setFont(QFont('Ubuntu',16))
        self.receive_button = QPushButton("Receive data", self)
        self.receive_button.clicked.connect(self.receive_thread)
        self.receive_button.setFont(QFont('Ubuntu',12))
        self.send_label = QLabel(f"Run testing with {test_len} samples")
        self.send_label.setAlignment(Qt.AlignHCenter | Qt.AlignVCenter)
        self.send_label.setFont(QFont('Ubuntu',16))
        self.send_button = QPushButton("Send test data", self)
        self.send_button.setFont(QFont('Ubuntu',12))
        self.send_button.clicked.connect(self.ecg_simulator)

        # init graph canvas
        self.canvas = MplCanvas(self, width=5, height=4, dpi=100)
        n_data = 20 # num of points for the graphs

        # create graph for cog load predictions
        self.xdata1 = list(range(n_data))
        self.ydata1 = [-0.2]*n_data
        self.canvas.axes1.set_title("Cognitive load  0->negative 1->positive")
        self.canvas.axes1.set_ylim((-0.1,1.1))
        self.canvas.axes1.set_ylabel('Class prediction')
        self.canvas.axes1.set_xlabel('Samples')
        self._plot_ref1 = None
        self.update_pred_plot(-1)

        # create graph for heart rate
        self.xdata2 = list(range(n_data))
        self.ydata2 = [-0.2]*n_data
        self.canvas.axes2.set_ylim((40,155))
        self.canvas.axes2.set_title("Heart rate")
        self.canvas.axes2.set_ylabel('bpm')
        self.canvas.axes2.set_xlabel('Samples')
        self._plot_ref2 = None
        self.update_hr_plot(35)
        self.show()

        # Set the layout
        layout = QVBoxLayout()
        # Set the layout
        layout = QVBoxLayout()
        layout.addWidget(self.canvas)
        layout.addWidget(self.receive_label)
        layout.addStretch(1)
        layout.addWidget(self.prediction_label)
        layout.addStretch(1)
        layout.addWidget(self.hr_params_label)
        layout.addStretch(1)
        layout.addWidget(self.receive_button)
        layout.addStretch(1)
        layout.addWidget(self.send_label)
        layout.addStretch(1)
        layout.addWidget(self.send_button)
        self.centralWidget.setLayout(layout)


    def show_message(self, str):
        self.send_label.setText(str)


    def report_sending_progress(self, n):
        self.send_label.setText(f"Sent {n} samples")


    # functions for displaying output
    def report_progress(self, message):
        self.receive_label.setText(message)


    def update_hrparams(self, message):
        self.hr_params_label.setText(message)


    def update_prediction(self, message):
        self.prediction_label.setText(message)


    def update_hr_plot(self,hr):
        # Drop off the first y element, append a new one.
        self.ydata2 = self.ydata2[1:] + [hr]
        # self.xdata += 1
        # Note: we no longer need to clear the axis.
        if self._plot_ref2 is None:
            # First time we have no plot reference, so do a normal plot.
            # .plot returns a list of line <reference>s, as we're
            # only getting one we can take the first element.
            plot_refs2 = self.canvas.axes2.plot(self.xdata2, self.ydata2, color='r',linestyle='--', marker='o')
            self._plot_ref2 = plot_refs2[0]
        else:
            # We have a reference, we can use it to update the data for that line.
            self._plot_ref2.set_ydata(self.ydata2)
        # Trigger the canvas to update and redraw.
        self.canvas.draw()


    def update_pred_plot(self,prediction):
        # Drop off the first y element, append a new one.
        self.ydata1 = self.ydata1[1:] + [prediction]
        # self.xdata += 1
        # Note: we no longer need to clear the axis.
        if self._plot_ref1 is None:
            # First time we have no plot reference, so do a normal plot.
            # .plot returns a list of line <reference>s, as we're
            # only getting one we can take the first element.
            plot_refs1 = self.canvas.axes1.plot(self.xdata1, self.ydata1, color='b',linestyle='--', marker='o')
            self._plot_ref1 = plot_refs1[0]
        else:
            # We have a reference, we can use it to update the data for that line.
            self._plot_ref1.set_ydata(self.ydata1)
        # Trigger the canvas to update and redraw.
        self.canvas.draw()


    def receive_thread(self):

        worker = Worker_receive()
        # init thread
        worker.progress.connect(self.report_progress)
        worker.hrparams.connect(self.update_hrparams)
        worker.prediction.connect(self.update_prediction)
        worker.add_pred_point.connect(self.update_pred_plot)
        worker.add_hr_point.connect(self.update_hr_plot)

        # Final resets
        self.receive_button.setEnabled(False)
        worker.finished.connect(
            lambda: self.receive_button.setEnabled(True)
        )

        self.threadpool.start(worker.receive_message)

    def ecg_simulator(self):

        worker = Worker_send()
        # init thread
        worker.message.connect(self.show_message)
        worker.progress_samples.connect(self.report_sending_progress)

        # Final resets
        self.send_button.setEnabled(False)
        worker.finished.connect(
            lambda: self.send_button.setEnabled(True)
        )

        self.threadpool.start(worker.send_test_data)


if __name__ == '__main__':
    '''
        channel settings, 
        the port has to be set here
    '''
    test_len = 10

    port = '/dev/ttyACM0'

    for i in sys.argv:
        if "--samples=" in i:
            test_len = int(i[10:])
            print(f"sending {test_len} samples!")
        if "--port=" in i:
            port = i[7:]

    app = QApplication(sys.argv)

    main = Window()
    main.show()

    """
        Try openning the channel
    # """
    # port_connected = False
    # while port_connected == False:
    try:
        channel = serial.Serial(port, baudrate = 115200, bytesize = serial.EIGHTBITS, parity = serial.PARITY_NONE, stopbits = serial.STOPBITS_ONE,timeout=None)
        main.send_label.setText("Port connected!")
        # port_connected == True
    except serial.serialutil.SerialException:
        main.receive_label.setText(f"Error: CANNOT OPEN SERIAL PORT {port}!")
        main.send_label.setText("The microcontroller is disconnected or you didnt specify the correct port.")
        main.receive_button.setEnabled(False)
        main.send_button.setEnabled(False)

    sys.exit(app.exec_())
