
//-------------------Sketch Description---------------------------------------------------------------------------------------------------------


/* This sketch demonstrates the use of ZE07-CO Carbon Monoxide Module UART I/O Mode 2. In this mode a request for UART data
 * must first be transmitted to the module. The module responds by transmitting back UART data which then must be recovered. 
 * In this example mode 2 UART data is requested at 3 second intervals. Mode 2 UART data is recovered as it arrives as indicated 
 * by a toggled read_flag state as well as a LED. Recovered CO concentration level in ppm is updated, and its value is printed out 
 * when read_flag state is toggled. There is also an option to use a board pin as a test point to observe read_flag state on an 
 * oscilloscope along with UART I/O data.
 *  
 * Declaration ZE7_CO ZE7_CO(&COSerial), at line 55, refers the Software Serial port created at line 52 to library file ZE7_CO.h 
 * included at line 37.
 * 
 * Three function calls are required to retrive concentration levels in Mode 2:
 * 1) ZE7_CO.beginMode2(), at line 77, sets UART I/O to Mode 2.
 * 2) ZE7_CO.Request_CO_data(), at line 108, requests UART Mode 2 data.
 * 3) ZE7_CO.updateMode2_data(concentration, read_flag), at line 87, recovers Mode 2 data as it is received,
 *    updates concentration level, and read_flag state which is toggled following data recovery. 
 *    This function  is non-blocking so other uperations may take place within the loop. However, it is most important
 *    that they must also be non-blocking for this function to work. 
 *   
 * Be advised that the pin spacing on this device makes it impossible to plug it into a standard 
 * breadboard with 0.1 inch (2.54mm) spacing. An adaptor PCB that remedies this problem is available
 * at https://www.tindie.com/products/ga_mecatronics/pcb-breakout-boards-for-winsen-ze07-co-modules/.
 * Close pin spacing does require good soldering skills, and a good soldering iron or soldering
 * station.
 * 
 * 
 * 
 * 
 * Created by HarvTech 10/17/2022
 * 
*/
//-------------------Declarations-------------------------------------------------------------------------------------

#include <ZE7_CO.h>
#include <SoftwareSerial.h>

//Software serial pins used for board in use.
//For NodeMcu 1.0 ESP-12E Module use:
#define Softserial_RX_pin D5
#define Softserial_TX_Pin D6

//Option to define pin as a test point for observing the read_flag variable waveform on an oscilloscope.
#define read_flag_Pin D1

//LED pin to be used for indicating UART data reception. 
#define LED_Pin D4 //Onboard blue LED.

//Create a Software Serial Port.
SoftwareSerial COSerial(Softserial_RX_pin, Softserial_TX_Pin ); 

//Refer the Software Serial port to the ZE7_CO library.
ZE7_CO ZE7_CO(&COSerial);

//Reference use of a HardwareSerial port, i.e.use of Hardware Serial2.
//ZE7_CO ZE7_CO(&Serial2);

//This should work with boards supporting multiple HardwareSerial ports. It does not work with
//the NodeMcu board due to the following issues.
// 1) I found the ZE07-C0 UART output unable to drive the NodeMCU HardwareSerial 
//    RX pin directly. It seems unable to sink enough current and a buffer is required. 
// 2) The HardwareSerial port selected should not be that used to upload code or receive other serial input.



//-------------------Global Variables-----------------------------------------------------------------------------------

unsigned long previousMillis = 0; 
unsigned long currentMillis;

float concentration = 0; //CO concentration value in ppm.
bool read_flag = false; //Toggles when UART data is received
bool last_read_flag;

//---------------------Setup----------------------------------------------------------------------------------------------
void setup() {
 
  Serial.begin(9600);
  delay(1000);
  COSerial.begin(9600);
  delay(1000);
  pinMode(LED_Pin, OUTPUT);
  pinMode(read_flag_Pin, OUTPUT);
  
 //Set Mode2 UART I/O
  ZE7_CO.beginMode2(); 
}
//---------------------Main Loop-----------------------------------------------------------------------------------
void loop() {

 //Request UART data at 3 second intervals.  
  Request_UART_data(3000);  
   
 //Recover Mode 2 UART data as it arrives, update concentration level, and read_flag state.
 //Note: read_flag state will toggle after concentration has been updated.
  ZE7_CO.updateMode2_data(concentration, read_flag);
  
  //Toggle the state of LED_Pin and read_flag_ Pin when UART data has been recovered
   Show_dataready();
   
  //Print updated concentration level.
   Print_Concentration();
       

//--------------------End of Main Loop---------------------------------------------------------------------------------

}

//--------------------My Functions-------------------------------------------------------------------------------------

void Request_UART_data(int interval) 
 {
    currentMillis = millis();
    if (currentMillis - previousMillis >= interval) 
    {
    previousMillis = currentMillis;
    ZE7_CO.Request_CO_data(); 
   }
 }

void Show_dataready()
{
 if(read_flag) 
  {
    digitalWrite(read_flag_Pin, HIGH);
    digitalWrite(LED_Pin, LOW);
  }
  else 
  {
    digitalWrite(read_flag_Pin, LOW);
    digitalWrite(LED_Pin, HIGH);
  }  
}

void Print_Concentration()
{
  if(read_flag != last_read_flag)
   {
     last_read_flag = read_flag;
     
   //Print data
    Serial.println("UARTI/O Mode2 Concentration Level:"); 
    Serial.print(concentration, 1);
    Serial.println(" ppm CO");   
    Serial.println("--------------------------------");
    Serial.println("");    
   } 
}





//---------------End of Sketch------------------------------------------------------------------------------------------------
