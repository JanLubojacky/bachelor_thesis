import sys
from numpy import size
import serial
import pickle
from time import sleep
from PyQt5.QtCore import QThread, Qt, QObject, pyqtSignal
from PyQt5.QtGui import QFont
from PyQt5.QtWidgets import (
    QApplication,
    QLabel,
    QMainWindow,
    QPushButton,
    QVBoxLayout,
    QWidget,
)
# Step 1: Create a worker class
class Worker(QObject):
    finished = pyqtSignal()
    progress = pyqtSignal(int)
    message = pyqtSignal(str)

    def send_test_data(self):
        '''check whether the port works'''
        try:
            channel = serial.Serial(port, baudrate = 115200, bytesize = serial.EIGHTBITS, parity = serial.PARITY_NONE, stopbits = serial.STOPBITS_ONE,timeout=None)
        except serial.serialutil.SerialException:
            self.message.emit(f"ERROR: CANNOT OPEN SERIAL PORT {port}!")
            self.finished.emit()
            return

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
            
            self.progress.emit(i+1)

        self.message.emit("Run testing")
        self.finished.emit()


class Window(QMainWindow):
    '''
    Graphical window class
    '''
    def __init__(self, parent=None):
        super().__init__(parent)
        self.setupUi()

    def setupUi(self):
        self.setWindowTitle("ECG APP SENDER")
        self.resize(400, 200)
        self.centralWidget = QWidget()
        self.setCentralWidget(self.centralWidget)

        self.send_label = QLabel("Run testing")
        self.send_label.setAlignment(Qt.AlignHCenter | Qt.AlignVCenter)
        self.send_label.setFont(QFont('Ubuntu',16))
        self.send_button = QPushButton("Send test data", self)
        self.send_button.setFont(QFont('Ubuntu',12))
        self.send_button.clicked.connect(self.ecg_simulator)
        # Set the layout
        layout = QVBoxLayout()
        layout.addWidget(self.send_label)
        layout.addWidget(self.send_button)
        self.centralWidget.setLayout(layout)

    def show_message(self, str):
        self.send_label.setText(str)

    def report_progress(self, n):
        self.send_label.setText(f"Sent {n} samples")

    def ecg_simulator(self):
        self.thread = QThread()
        # Create a worker object
        self.worker = Worker()
        # Move worker to the thread
        self.worker.moveToThread(self.thread)
        # Connect signals and slots
        self.thread.started.connect(self.worker.send_test_data)
        self.worker.finished.connect(self.thread.quit)
        self.worker.finished.connect(self.worker.deleteLater)
        self.thread.finished.connect(self.thread.deleteLater)
        self.worker.message.connect(self.show_message)
        self.worker.progress.connect(self.report_progress)
        # Start the thread
        self.thread.start()

        # Final resets
        self.send_button.setEnabled(False)
        self.thread.finished.connect(
            lambda: self.send_button.setEnabled(True)
        )

        self.thread.exit()


if __name__ == '__main__':
    '''number of samples to send'''
    test_len = 10
    '''
        channel settings, 
        the port has to be set here
    '''
    port = '/dev/ttyACM0'
    
    for i in sys.argv:
        if "--samples=" in i:
            test_len = int(i[10:])
        if "--port=" in i:
            port = i[7:]
    app = QApplication(sys.argv)

    main = Window()
    main.show()

    sys.exit(app.exec_())
