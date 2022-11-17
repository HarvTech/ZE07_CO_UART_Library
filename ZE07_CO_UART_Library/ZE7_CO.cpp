
 /*
    ZE07_CO_UART_Library
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

/*  ZE7_CO Library ZE7_CO.cpp
 *  Facilitates the use of both UART I/O Modes provided by the the Winsen ZE07_CO Electrochemical
 *  CO Module as documented in User Manual Version:1.7 Valid from 10/19/2020, available at
 *  https://www.winsen-sensor.com/sensors/co-sensor/ze07-co.htm
 *  
 * Much of this code is patterned after that found in ZE07-CO_Sensor library created by 
 * Valentin - 2018/01/03, https://github.com/tweenietomatoes/ZE07CO#start-of-content.
 * 
 * Created by HarvTech 10/17/2022
 */

#include "ZE7_CO.h"


//Point mySerial to the Software Serial port.
ZE7_CO::ZE7_CO(SoftwareSerial* Serial) 
{
  mySerial = Serial;
}

//Point mySerial to the Hardwareware Serial port.
ZE7_CO::ZE7_CO(HardwareSerial* Serial) 
{
  mySerial = Serial;
}

//Setup Mode1.
void ZE7_CO::beginMode1()
{
 for(int i = 0; i <=8; i++) {
 mySerial->write(switch_init[i]);
 }   
}

//Setup Mode2.
void ZE7_CO::beginMode2()
{
  for(int i = 0; i <=8; i++) {
  mySerial->write(switch_QA[i]);;
 }       
}

//Request Mode2 data.
void ZE7_CO::Request_CO_data() 
{
  for(int i = 0; i <=8; i++) {
   mySerial->write(question[i]);
 }  
}  

//Update Mode1 data and reference params.
 void ZE7_CO::updateMode1_data(float &CON, bool &data_ready)
 {
      if (mySerial ->available() >0 ) 
   {          
      This_Byte = mySerial->read(); //Read a data Byte.
      //When Start Byte0 is received.
      if(This_Byte == 255) 
      { 
        _j = 0; //Assign it as Byte number 0. 
        sum = 0; //Initialize Sum check. 
      }      
       init_data[_j] = This_Byte; //Enter byte in data array in Byte order.
       
          if((_j != 0) && (_j != 8)) //Exclude Byte0 and Byte8.
          { 
            sum = sum + init_data[_j]; //Accumulate Sum check.
          }
       _j++; //Increment Byte number.
       
       //When last data Byte has been received.
       if(_j >8) 
       { 
        sum = ~sum +1;//Sum check complete.
          if(sum == init_data[8]) //Sum check = Check sum?
          {          
          CON = (float(init_data[4]<<8 | init_data[5]))/10; 
          data_ready = !data_ready;
          }
        }
    } 
 }           
 
   
 
 
//Update Mode2 data and reference params.
void ZE7_CO::updateMode2_data(float &CON, bool &data_ready) 
 {
  if (mySerial->available() >0 ) 
  {          
      This_Byte = mySerial ->read(); //Read a data Byte.
      //When Start Byte0 is received.
      if(This_Byte == 255) 
      { 
        _j = 0; //Assign it as byte index 0.
        sum = 0; //Initialize Sum check.
      }      
       answer_data[_j] = This_Byte; //Enter byte in data array in Byte order.
          if((_j != 0) && (_j != 8)) //Exclude Byte0 and Byte8.
          { 
            sum = sum + answer_data[_j]; //Accumulate Sum check.
          }             
       _j++; //Increment byte index.

       //When last data Byte has been received.
       if(_j >8) 
       { 
        sum = ~sum +1;//Sum check complete.
          if(sum == answer_data[8]) //Sum check = Check sum?
          {         
           CON = float(float(answer_data[2]<<8 | answer_data[3]))/10;
           data_ready = !data_ready; 
          }                        
      }
   }    
 }

//-------------------------------------------------------------------------------------------------------------------------------------
