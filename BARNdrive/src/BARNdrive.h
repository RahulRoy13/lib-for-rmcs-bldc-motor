#ifndef BARNdrive_h
#define BARNdrive_h

#include "SoftwareSerial.h"
#include "Arduino.h"

class BARN
{
  // user-accessible "public" interface
  public:
    void Serial0(long baudrate);
    void Serial_selection(byte x);
    void begin(HardwareSerial *hport,long baudrate);
    void begin(SoftwareSerial *sport, long baudrate);
    byte Enable_Digital_Mode(byte slave_id,byte dir);
    byte Speed(byte slave_id,int SPEED);    
    byte WriteSingleRegister(byte slave_id, int address, unsigned int value);
    String ReadSingleRegister(byte slave_id, int address,unsigned int No_register);
    String Modbus_string(byte slave_id,byte FC,int address,unsigned int data);
    String print_hex(byte number);
    byte nibble(char c);
    byte LRC(String s,byte len);
    void hexToBytes(byte *byteArray, char *hexString);
    long int Speed_Feedback(byte slave_id) ;
    long int value(String input);
    byte READ_PARAMETER(byte slave_id);
    long int READ_DEVICE_MODBUS_ADDRESS(byte slave_id);
    long int READ_INP_CONTROL_BYTE(byte slave_id);
    long int READ_INP_MODE_BYTE (byte slave_id);
    byte WRITE_PARAMETER(byte slave_id,int INP_CONTROL_MODE,int Speed);
    long int READ_TRP_SPD_WORD(byte slave_id);
    byte SAVE(byte slave_id);
    byte RESET(byte slave_id);
    byte Disable_Digital_Mode(byte slave_id,byte dir);
    byte Brake_Motor(byte slave_id,byte dir);
  private:

};
#endif
