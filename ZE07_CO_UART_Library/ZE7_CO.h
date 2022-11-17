
  /*
    Copyright (C) 2022  HarvTech
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    https://www.gnu.org/licenses    
  */

/*  ZE7_CO Library  ZE7_CO.h
 *  This library facilitates the use of both UART I/O Modes provided by the the Winsen ZE07_CO Electrochemical
 *  CO Module as documented in User Manual Version:1.7 Valid from 10/19/2020, available at
 *  https://www.winsen-sensor.com/sensors/co-sensor/ze07-co.htm.
 *  
 * Much of this code is patterned after that found in ZE07-CO_Sensor library created by 
 * Valentin - 2018/01/03, https://github.com/tweenietomatoes/ZE07CO#start-of-content.
 * 
 * Created by HarvTech 10/17/2022
 *
*/

#ifndef __ZE7_CO_H_
#define __ZE7_CO_H_

#include <SoftwareSerial.h>
#include <arduino.h>

class ZE7_CO
{ 
  public:

//-------------------Public Functions-------------------------------------------------------------------- 
  ZE7_CO(SoftwareSerial* Serial); //Use the designated SoftwareSerial port.
  ZE7_CO(HardwareSerial* Serial); //Use the designated HardwarwwareSerial port.
  void beginMode1(); //Setuo Mode1.
  void beginMode2(); //Setup Mode2.
  void Request_CO_data(); //Request Mode2 data
  void updateMode1_data(float &CON, bool &data_ready); //Update Mode1 data and reference local params.  
  void updateMode2_data(float &CON, bool &data_ready); //Update Mode2 data and reference local params.
  
//-------------------Public Variables-----------------------------------------------------------------------
  float _concentration; //CO concentration value in ppm.
  bool read_flag = false; //Toggles when UART data received

  
  private:
  
//----------------Private Variables-------------------------------------------------------------------------  
  Stream *mySerial; //Points myRerial to a Serial port.
  byte switch_init[9] = {255, 1, 120, 64, 0, 0, 0, 0, 71}; //"inititive upload" mode switch data block.
  byte switch_QA[9] = {255, 1,120, 65, 0, 0, 0, 0, 70}; //"Q&A" mode switch data block.
  byte question[9] = {255, 1, 134, 0, 0, 0, 0, 0, 121}; //"Question" command data block.
  byte init_data[9]; //Establish receive data buffer for "initiative upload" Mode1.
  byte answer_data[9]; //Establish receive data buffer for "Q&A" Mode2.
  byte This_Byte; //data byte read from serial buffer.
  uint8_t _j = 0; //data array index.
  float CON;
  bool ready_flag;
  byte sum;
};

#endif
