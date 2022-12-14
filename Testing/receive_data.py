import serial
import time
import sys
import matplotlib
matplotlib.use('Qt5Agg')
from matplotlib.backends.backend_qt5agg import FigureCanvasQTAgg as FigureCanvas
from matplotlib.figure import Figure
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

# Create a matplotlib window
class MplCanvas(FigureCanvas):

    def __init__(self, parent=None, width=5, height=4, dpi=100):
        fig = Figure(figsize=(width, height), dpi=dpi)
        fig.subplots_adjust(hspace=1)
        self.axes1 = fig.add_subplot(211)
        self.axes2 = fig.add_subplot(212)
        super(MplCanvas, self).__init__(fig)

# Create a worker class
class Worker(QObject):
    finished = pyqtSignal()
    progress = pyqtSignal(str)
    hrparams = pyqtSignal(str)
    prediction = pyqtSignal(str)
    add_pred_point = pyqtSignal(float)
    add_hr_point = pyqtSignal(float)

    def receive_message(self):
    # read data comming from microcontroller
        no_activity = 0
        self.progress.emit("Receiving!")

        try:
            channel = serial.Serial(port, baudrate = 115200, bytesize = serial.EIGHTBITS, parity = serial.PARITY_NONE, stopbits = serial.STOPBITS_ONE,timeout=None)
        except serial.serialutil.SerialException:
            self.progress.emit(f"ERROR: CANNOT OPEN SERIAL PORT {port}!")
            self.finished.emit()
            return

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
                            self.prediction.emit(f"Cognitive load HIGH!")
                        elif output == 0:
                            self.prediction.emit(f"Cognitive load LOW!")
                        self.add_pred_point.emit(output)
                    except ValueError:
                        pass

                elif "HR:" in data_str:
                    self.hrparams.emit(data_str)
                    hr = float(data_str[4:10])
                    self.add_hr_point.emit(hr)
                else:
                    self.progress.emit("Unexpected message:" + data_str)
                    self.finished.emit()

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
        self.setupUi()

    def setupUi(self):
        self.setWindowTitle("ECG APP RECEIVER")
        self.resize(1000, 600)
        self.centralWidget = QWidget()
        self.setCentralWidget(self.centralWidget)
        self.receive_label = QLabel("Run receiving!")
        self.receive_label.setAlignment(Qt.AlignHCenter | Qt.AlignVCenter)
        self.receive_label.setFont(QFont('Ubuntu',20))
        self.prediction_label = QLabel("prediction:", self)
        self.prediction_label.setAlignment(Qt.AlignHCenter | Qt.AlignVCenter)
        self.prediction_label.setFont(QFont('Ubuntu',16))
        self.hr_params_label = QLabel("HR: , MEAN_RR_INT: , SDNN: , RMSSD: ", self)
        self.hr_params_label.setAlignment(Qt.AlignHCenter | Qt.AlignVCenter)
        self.hr_params_label.setFont(QFont('Ubuntu',16))
        self.receive_button = QPushButton("receive data", self)
        self.receive_button.clicked.connect(self.receive_thread)
        self.receive_button.setFont(QFont('Ubuntu',12))
        self.canvas = MplCanvas(self, width=5, height=4, dpi=100)
        n_data = 20
        self.xdata1 = list(range(n_data))
        self.ydata1 = [-0.2]*n_data
        self.canvas.axes1.set_title("Cognitive load")
        self.canvas.axes1.set_ylim((-0.1,1.1))
        self.canvas.axes1.set_ylabel('Class prediction')
        self.canvas.axes1.set_xlabel('Samples')

        self.xdata2 = list(range(n_data))
        self.ydata2 = [-0.2]*n_data
        self.canvas.axes2.set_ylim((40,155))
        self.canvas.axes2.set_title("Heart rate")
        self.canvas.axes2.set_ylabel('bpm')
        self.canvas.axes2.set_xlabel('Samples')

        self._plot_ref1 = None
        self.update_pred_plot(-1)
        self._plot_ref2 = None
        self.update_hr_plot(35)
        self.show()

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
        self.centralWidget.setLayout(layout)


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
        """Long-running task in 5 steps."""
        # Step 2: Create a QThread object
        self.thread = QThread()
        # Step 3: Create a worker object
        self.worker = Worker()
        # Step 4: Move worker to the thread
        self.worker.moveToThread(self.thread)
        # Step 5: Connect signals and slots
        self.thread.started.connect(self.worker.receive_message)
        self.worker.finished.connect(self.thread.quit)
        self.worker.finished.connect(self.worker.deleteLater)
        self.thread.finished.connect(self.thread.deleteLater)
        self.worker.progress.connect(self.report_progress)
        self.worker.hrparams.connect(self.update_hrparams)
        self.worker.prediction.connect(self.update_prediction)
        self.worker.add_pred_point.connect(self.update_pred_plot)
        self.worker.add_hr_point.connect(self.update_hr_plot)
        # Step 6: Start the thread
        self.thread.start()

        # Final resets
        self.receive_button.setEnabled(False)
        self.thread.finished.connect(
            lambda: self.receive_button.setEnabled(True)
        )

        self.thread.exit()


if __name__ == '__main__':
    '''
        channel settings, 
        the port has to be set here
    '''
    port = '/dev/ttyACM0'

    for i in sys.argv:
        if "--port=" in i:
            port = port = i[7:]

    app = QApplication(sys.argv)

    main = Window()
    main.show()

    sys.exit(app.exec_())
