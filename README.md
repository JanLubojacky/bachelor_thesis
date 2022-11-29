# ECG Cognitive Load Analyzer with AI for a microcontroller

Developed for the bachelor thesis of Jan Lubojacky at CTU FEL, 2022

**Setup**
   - Create virtual enviroment with the name cog_load_analyzer
   
         python -m venv cog_load_analyzer
   - Activate it
   
         source cog_load_analyzer/bin/activate
   - Install requirements

         pip install -r requirements.txt
         
**Preprocessing**

   - unpack_data.py 
      - creates a directory output_data and places a directory with preprocessed data inside, the directories are named according to the specified parameters
    
   - config.py
      - settings for preprocessing the data, we can set the window lenght, sample rate, preprocessing flag

   - ecg_preprocess.py
      - functions for preprocessing the ECG signal
   
   - requires the training datasets to be present
****

**Neural network training**
   - ecg_neural_net_prototype.ipybn -> jupyter notebook from google colab for training the data, 
   
      - run it toghether with the mobilenet.py file in google colab
****
**Tf lite models**

   - Exported neural network models in .tflite format
****

**Testing**
    
  **send_test_data.py**
  
   - Containts a script, that sends test data to the microcontroller, recieves the output and calculates the accuracy, sensitivity, specificity, precision and saves a confusion matrix.
  
  **Application GUI - linux only**
  
  contents: 
  
            send_data.py -> ECG simulator, that sends data over usart, optional arguments --samples=NUM_OF_SAMPLES_TO_SEND, --port=SERIAL_PORT
            receive_data.py -> Reciever application that plots data outputted bu the microcontroller, optional argument --port=SERIAL_PORT
            run_gui.sh -> shell script that runs both send_data.py and recieve_data.py
    
  Running the GUI application
  
  1.) Run the shell script run_gui.sh.

  2.) Press the recieve data and the send data buttons.

  3.) The ECG sender window displays number of samples sent, default number is 10, and the ECG reciever window displays predictions from the microcontroller.

  4.) If there is no communication for 5 seconds the reciever goes inactive and the button needs to be pressed again.
  
  **gui_app_one_window.py - linux + windows**
  
  - Has the same function as send_data.py and recieve_data.py integrated into one window. This is necessary for the app to work on Windows os too because on Windows two applications cant access one serial port at the same time (:
  
  - Takes two optional command line arguments --samples=NUM_OF_SAMPLES_TO_SEND, --port=SERIAL_PORT, the serial port has to be the same one the microcontroller is connected to. If connected to an empty serial port the application will simply show an error message and has to be restarted with the correct port.
  
****
**Microcontroller application**

The application is written for the STM32F303RE nucleo board.

ecg_cogload_microcontroller

Contents
   - Core 
      - Inc - headers - my code -> ecg_preprocessing.h, input_parser.h
      - Src - my code -> main.c, ecg_preprocessing.c, input_parser.c
      - Startup
   - Drivers - STM drivers
   - Middlewares - STM drivers
   - X-CUBE-AI - STM drivers, code for the neural network

Details:

- The application defaultly works with a 5s windows sampled at 500 Hz (5*500 = 2500 samples) which are then filtered and downsampled to 100 Hz (500 samples), which is what the neural network expects.
- The initial sampling rate can be configured in the defines at the beginning of main.c, however the buffer sizes for the input signal have to be adjusted accordingly.
- It is a good idea to press the black reset button on the microcontroller before running the sending application to clean the buffers and reinitialize everything.

Microcontroller signals with the onboard green led:

   - Successfull startup: The green led (LED2) blinks twice.

   - Working: Input is recieved trough UART 
      - Output is sent trough UART in the form ("prediction: %.2f", prediction) 
      and ("HR: %2.f, MEAN_RR_INT %2.f, SDNN %.2f, RMSSD %.2f", hr, mean_rr_int, sdnn, rmsssd), green led (LED2) is off.

   - Error: If a critical errror occurs a corresponding error message is sent trough UART and the green led (LED2) turns on.
 
