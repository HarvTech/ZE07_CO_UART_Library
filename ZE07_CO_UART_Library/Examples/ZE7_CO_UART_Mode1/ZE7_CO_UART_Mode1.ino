
/*-------------------Desription-----------------------------------------------------------------------------------
 * 
 * This sketch demonstrates the use of ZE07-CO Carbon Monoxide Module Mode 1 UART I/O. In this mode UART data is automatically
 * transmitted from the module periodically at one second time intervals. In this example mode 1 UART data is recovered as it 
 * arrives as indicated by a toggled read_flag state as well as a LED. Recovered CO concentration level in ppm is updated, and
 * its value is printed out when read_flag state is toggled. There is also an option to use a board pin as a test point to observe 
 * read_flag state on an oscilloscope along with UART I/O data signals..
 * 
 * Declaration ZE7_CO ZE7_CO(&COSerial), at line 49, refers the Software Serial port created in line 46 to library file ZE7_CO.h 
 * included at line 32.
 * 
 * Two function calls are required to retrive concentration levels in Mode 1:
 * 1) ZE7_CO.beginMode1(), at line 72 sets Mode 1 UART I/O Mode 1 if it had previously been set to Mode 2.
 * 2) ZE7_CO.updateMode1_data(concentration, read_flag), at line 80, continuously recovers Mode 1 data as it arrives,
 *    updates concentration level, and read_flag state which is toggled after concentration has been updated.
 *    This function  is non-blocking so other uperations may take place withinin the loop. However, it is most important
 *    that they must also be not block the receptioon of softwre serial data for this function to work. 
 *    
 * Be advised that the pin spacing on this device makes it impossible to plug it into a standard 
 * breadboard with 0.1 inch (2.54mm) spacing. An adaptor PCB that remedies this problem is available
 * at https://www.tindie.com/products/ga_mecatronics/pcb-breakout-boards-for-winsen-ze07-co-modules/.
 * Close pin spacing does require good soldering skills, and a good soldering iron or soldering
 * station.   
 *    
 * Created by HarvTech 10/17/2022
 *
*/

//-------------------Declarations------------------------------------------------------------------------------------------------

#include <ZE7_CO.h>
#include <SoftwareSerial.h>

//Software serial pins used for board in use.
//For NodeMcu 1.0 ESP-12E Module use:
#define Softserial_RX_pin D5
#define Softserial_TX_Pin D6

//LED pin to be used for indicating UART data reception. 
#define LED_Pin D4
//pin to monitor read_flag state on an oscilloscope along with UART I/O signals.
#define read_flag_Pin D1

//Create a Software Serial Port.
SoftwareSerial COSerial(Softserial_RX_pin, Softserial_TX_Pin ); 

//Reference library use of the Software Serial port.
ZE7_CO ZE7_CO(&COSerial);

//Reference use of a HardwareSerial port, i.e.use of Hardware Serial2.
//ZE7_CO ZE7_CO(&Serial2);

//This should work with boards supporting multiple HardwareSerial ports. It does not work with
//the NodeMcu board due to the following issues.
// 1) I found the ZE07-C0 UART output unable to drive the NodeMCU HardwareSerial 
//    RX pin directly. It seems unable to sink enough current and a buffer is required. 
// 2) The HardwareSerial port selected should not be that used to upload code or receive other serial input.


//-------------------Variables-----------------------------------------------------------------

unsigned long previousMillis = 0; 
unsigned long currentMillis;

float concentration = 0; //CO concentration in ppm.
bool read_flag = false; //Toggles when UART data is received
bool last_read_flag;


//---------------------Setup--------------------------------------------------------------------
void setup() {
 
  Serial.begin(9600);
  delay(1000);
  COSerial.begin(9600);
  delay(1000);
  pinMode(LED_Pin, OUTPUT);
  pinMode(read_flag_Pin, OUTPUT);
  
  //Set UART I/O to Mode1
  ZE7_CO.beginMode1(); 

}
//---------------------Main Loop-----------------------------------------------------------------------------------
void loop() {
 
 //Recover Mode 1 UART data as it arrives, update concentration, and read_flag state. 
  //Note: read_flag state will toggle after concentration has been updated.
  ZE7_CO.updateMode1_data(concentration, read_flag);

 //Toggle the state of LED_Pin and read_flag_ Pin when UART data has been recovered
   Show_dataready();
   
 //Print updated concentration level.
   Print_Concentration();
 

//---------------------End Main Loop---------------------------------------------------------------------------------
}
//--------------------My  Functions--------------------------------------------------------------------------------------

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
    Serial.println("UARTI/O Mode1 Concentration Level:"); 
    Serial.print(concentration, 1);
    Serial.println(" ppm CO");   
    Serial.println("-------------------------------------");
    Serial.println("");    
   } 
}


//-------------------End Sketch---------------------------------------------------------------------------------------
