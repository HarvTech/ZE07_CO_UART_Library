    ZE07_CO_UART_Library

 The Winsen ZE07_CO Electrochemical CO Module provides a UART I/O interface that yields
 Carbon Monoxide(CO) concentration levels ranging from 0.5 ppm to 500 ppm with a resolution of 
 0.1 ppm. It also provides an analog DAC output ranging from 0.4 to 2.0 Volts linearly
 proportional to CO concentration levels ranging from 0 to 500ppm. However, to make use of
 this analog output with the same resolution will require the use of a high resolution ADC with 
 a precision voltage reference. Considering that this module's onboard electronics already 
 provides this hardware it makes little sense to duplicate it if a serial port is available.
 The DAC output is most suited for applications that may simply employ a discrete analog
 comparator to trigger an alarm at a threshold level, and/or possibly a Voltmeter scaled in ppm.
 
 UART I/O operation is well documented in:
 User Manual Version:1.7 Valid from 10/19/2020, available at
 https://www.winsen-sensor.com/sensors/co-sensor/ze07-co.htm
 
 There are two modes of UART communication. A default mode that runs in a new
 device that has not previously been reconfigured is called "initiative upload".
 In this mode UART data is automatically transmitted periodically at one second intervals.
 I will refer to this as Mode 1. The second UART communication mode is called
 "question and answer (Q&A)" which I will refer to as Mode 2. In this mode a request for data
 must be received by the device before data is transmitted. Additional commands must be sent 
 to set the mode to be used. All UART communication takes place using unique nine byte data blocks.
 
 This arduino library provides the code required to retrive CO concentration levels
 from this device via. a software serial port and two or three simple function calls. 
 Much of this code is patterned after that found in ZE07-CO_Sensor library Developed by 
 Valentin - 2018/01/03, https://github.com/tweenietomatoes/ZE07CO#start-of-content.

 Included examples ZE7_UART_Mode1, and ZE7_UART_Mode2 demonstrate the use of both modes. 
 These examples were setup to run on a NodeMcu development board. Since all pin numbers are 
 defined it should not be difficult to modify them to run on most any Aduino compatible board.
 Install this arduino library in "libraries" and give the examples a try.
