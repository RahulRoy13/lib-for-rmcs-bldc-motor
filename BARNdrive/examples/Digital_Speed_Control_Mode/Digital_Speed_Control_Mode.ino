

/*
Connections of Drive and Arduino
Serial Port 0 is not used to connect to drive because its connected to USB-Serial and used to show information on console.

For Arduino Uno Software serial needs to be used as there is only one hardware serial port and its connected to USB-Serial. 
   Drive to Arduino UNO/Nano connections
   GND         -      GND
   RXD         -      D3
   TXD         -      D2

For arduino mega and other arduinos with multiple hardware serial port, any port other than 0 can be selected to connect the drive.

   Drive to Arduino Mega2560 connections
   GND         -      GND
   RXD         -      Tx1/Tx2/Tx3
   TXD         -      Rx1/Rx2/Rx3
   
*	This mode can be used when multiple motors are to be used to run at exactly the same RPM and same torque even though the voltage supply might be different.
*	Also in this mode the direction of the motor can be controlled digitally via modbus ASCII commands to run the dc servo motor in both directions

* For more information see : https://robokits.co.in/motor-drives-drivers/encoder-dc-servo/rhino-dc-servo-driver-50w-compatible-with-modbus-uart-ascii-for-encoder-dc-servo-motor


*/

#include "BARNdrive.h"
  
BARN rmcs;                    //object for class RMCS2303

//SoftwareSerial myserial(2,3);     //Software Serial port For Arduino Uno. Comment out if using Mega.

//parameter Settings "Refer datasheet for details"
byte slave_id=1;
int INP_CONTROL_MODE=257;           
//int SPEED=100;
int i;
int a=0;
unsigned int v=67583;

long int Current_Speed;


void setup()
{
   rmcs.Serial_selection(0);       //Serial port selection:0-Hardware serial,1-Software serial
   rmcs.Serial0(9600);             //Set baudrate for usb serial to monitor data on serial monitor
   Serial.println("RMCS-2303 Speed control mode demo\r\n\r\n");
    
   rmcs.begin(&Serial1,9600);    //Uncomment if using hardware serial port for mega2560:Serial1,Serial2,Serial3 and set baudrate. Comment this line if Software serial port is in use
  // rmcs.begin(&myserial,9600);     //Uncomment if using software serial port. Comment this line if using hardware serial.
//  rmcs.WRITE_PARAMETER(slave_id,);    //Uncomment to write parameters to drive. Comment to ignore.
    rmcs.RESET(slave_id);
  // rmcs.WriteSingleRegister(slave_id,a,v);
   rmcs.READ_PARAMETER(slave_id);
   
   rmcs.Speed(slave_id,0);
   rmcs.Enable_Digital_Mode(slave_id,0);  
}

void loop(void)
{
   i=0;
   Serial.println("Sending speed command - 2000 RPM");

  for(i=0;i<=100;i++)
  {
   rmcs.Speed(slave_id,60); //Set speed within range of 0-400 or 0-(maximum speed of base motor)
    delay(100);
   Serial.println(i);
  }  
  
        
   
   delay(3000);
   Current_Speed=rmcs.Speed_Feedback(slave_id); 
   Serial.print("Current Speed feedback : ");
   Serial.println(Current_Speed);

                 


   
  

   rmcs.READ_PARAMETER(slave_id);
   delay(3000);

}
