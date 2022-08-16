    #include "BARNdrive.h"
    #include "SoftwareSerial.h"
    #include "Arduino.h"

    byte Sprt;
    HardwareSerial *port;
    SoftwareSerial *softport;
    const byte MaxByteArraySize = 8;
    byte byteArray[MaxByteArraySize] = {0};

void BARN::Serial0(long baudrate)
    {
        Serial.begin(baudrate);
    }
	
void BARN::Serial_selection(byte x)
    {   
        if(x==0)  Sprt=0;
        if(x==1)  Sprt=1;
    }
     
void BARN::begin(HardwareSerial *hport,long baudrate)
    {
        port=hport;
        if(port==&Serial)
        {
            Serial.println("error");
            Serial.println("Use other serial port");
            Serial.end();
        }
        else
            port->begin(baudrate);
    }

void BARN::begin(SoftwareSerial *sport,long baudrate)
    {
        softport=sport;
        softport->begin(baudrate);
    }
	

	
byte BARN::Enable_Digital_Mode(byte slave_id, byte dir)
    {
        int data; byte j;
        if(dir==0)   data=257;
        if(dir==1)   data=265;
        j=WriteSingleRegister(slave_id,2,data);
        if(j==1) return 1;
        else return 0;
    }
	
byte BARN::Disable_Digital_Mode(byte slave_id,byte dir)
    { 
        int data; byte j;
        data=256;
        j=WriteSingleRegister(slave_id,2,data);
        if(j==1) return 1;
        else return 0;
    }
	

	
byte BARN::Speed(byte slave_id, int SPEED)
    {  
        byte j;
    
        j=WriteSingleRegister(slave_id,6,SPEED);
        if(j==1) return 1;
        else return 0;
    }
   	
long int BARN::Speed_Feedback(byte slave_id) 
    {     
        String result,result1,out,x,y;  int w;char a[9];char *endptr;unsigned long int q;long int p;
        result1=ReadSingleRegister( slave_id,8,1);
        q = value( result1);
        if(q>32765) p=q-65535;
        else p=q;
        return (p);
    }
	
byte BARN::Brake_Motor(byte slave_id,byte dir)
    { 
        int data; byte j;
        data=259;
        j=WriteSingleRegister(slave_id,2,data);
        if(j==1) return 1;
        else return 0;
    }


  
byte BARN::WriteSingleRegister(byte slave_id, int address,unsigned int value)
    {   
        String response,res;byte j;
        String result=Modbus_string(slave_id,6,address,value);
      
        if(Sprt==0)
        {    
            port->print(result);
            delay(50);
            while(port->available())
           {response=port->readStringUntil('\n');}
            if(response=='\0')
            {Serial.println("CHECK YOUR SLAVE_ID"); return 0;}
            res=(response.substring(1,16));
            j=LRC(res,6);
            if(j==1) return 1;
            else return 0;
        }
        
        if(Sprt==1) 
        {
            softport->print(result);
            
            delay(50);
            while(softport->available())
            {response=softport->readStringUntil('\n');}
            if(response=='\0')
            {Serial.println("CHECK YOUR SLAVE_ID"); return 0;}
            res=(response.substring(1,16));
            j=LRC(res,6);
          
            if(j==1) return 1;
            else return 0;
        }
        
    }
    
String BARN::Modbus_string(byte slave_id,byte FC,int address,unsigned int data)
    {
        byte sum =0;
        byte AddLoByte,AddHiByte,dataLoByte,dataHiByte;
        AddLoByte=AddHiByte=dataLoByte=dataHiByte=0;
        AddLoByte = (address & 0x00FF);
        AddHiByte = ((address & 0xFF00) >>8);
        dataLoByte = (data & 0x00FF);
        dataHiByte = ((data & 0xFF00) >>8);
        sum = 0;
        sum -= slave_id;
        sum -= FC;
        sum -= AddLoByte;
        sum -= AddHiByte;
        sum -= dataLoByte;
        sum -= dataHiByte;

        String SLAVE = print_hex(slave_id);
        String FCN = print_hex(FC);
        String AddHi = print_hex(AddHiByte);
        String AddLo = print_hex(AddLoByte);
        String dataHi = print_hex(dataHiByte);
        String dataLo = print_hex(dataLoByte);
        String LRC = print_hex(sum);
        String result=":"+ SLAVE+FCN+AddHi+AddLo+dataHi+dataLo+LRC+"\r\n";

        return result;
    }

String BARN::print_hex(byte number)
    {
        String value = String(number,HEX);
        value.toUpperCase();
        if(value.length()==1){value = "0" + value;}
        return(value);
    }

String BARN::ReadSingleRegister(byte slave_id, int address,unsigned int No_register) 
    {   
        String response,res; byte j;
        String result = Modbus_string(slave_id,3,address,No_register);
        if(Sprt==0)
        { 
            port->print(result);
            delay(20);
            
            while(port->available()>0)
            {
                response=(port->readStringUntil('\n'));
                Serial.print("<<");
            }
           
            res=response.substring(1,18);
            j=LRC(res,7);
            
            if(j==1) return(response);
            else return "0";

        }

        if(Sprt==1)
        {
            softport->print(result);
            delay(20);
            while(softport->available())
            {    
                response=(softport->readStringUntil('\n'));
                Serial.print("<<");
            }
            res=response.substring(1,18);
            j=LRC(res,7);
            if(j==1) return(response);
            else return "0";
        }
    }
    
byte BARN:: WRITE_PARAMETER(byte slave_id,int INP_CONTROL_MODE,int SPEED)
{   byte W1,W2;
    W1=W2=0;
    W1=WriteSingleRegister(slave_id,2,INP_CONTROL_MODE);
    if(W1==1) {Serial.println("INP_CONTROL :          DONE");
         Serial.println("INP_MODE :             DONE");}
    else {Serial.print("error in writing parameters"); Serial.end();} 
    
   
    W2=Speed( slave_id,SPEED);
     if(W2==1) Serial.println("SPEED :                DONE");
    else {Serial.print("error in writing parameters"); Serial.end();}
    
    if(W1 & W2==1)
    {
		byte c;
		c=SAVE(slave_id);
		if(c==1) {Serial.println("ALL PERAMETER SET"); Serial.print("\n");}
		else Serial.print("NOT SAVED");
    }
}

byte BARN::READ_PARAMETER(byte slave_id)
{  
	long int R1,R2,R3,R4;
    R1=R2=R3=R4=0;
    
	R1=READ_DEVICE_MODBUS_ADDRESS(slave_id);
    if(R1>=0)
    {Serial.print("DEVICE_MODBUS_ADDRESS: ");
    Serial.println(R1);}
    else 
    {Serial.print("ERROR IN READING"); 
    Serial.end();}
    
    R2=READ_INP_CONTROL_BYTE(slave_id);
    if(R2>=0)
    {
		Serial.print("INP_CONTROL_BYTE: ");
		Serial.println(R2);
	}
    else 
    {	Serial.println(R2);
        Serial.print("ERROR IN READING"); 
		Serial.end();
	}
    
    R3=READ_INP_MODE_BYTE(slave_id);
    if(R3>=0)
    {
		Serial.print("INP_MODE_BYTE: ");
		Serial.println(R3);
	}
    else 
    {
		Serial.print("ERROR IN READING"); 
		Serial.end();
	}
    
   
    
    R4=READ_TRP_SPD_WORD(slave_id);
    if(R4>=0)
    {
		Serial.print("TRP_SPD_WORD: ");
		Serial.println(R4);
	}
    else 
    {
		Serial.print("ERROR IN READING"); 
		Serial.end();
	}
}


long int BARN::READ_DEVICE_MODBUS_ADDRESS(byte slave_id) 
    {     
        String result,x; unsigned long int p;char a[9];int len;char *endptr;
        result=ReadSingleRegister( slave_id,0,1);
        x=result.substring(9,11);
        x.toCharArray(a,3);
        p=strtoul(a,&endptr,16);
      return(p);
    }
long int BARN::READ_INP_CONTROL_BYTE(byte slave_id) 
    {     
         String result,x; unsigned long int p;char a[9];int len;char *endptr;
        result=ReadSingleRegister( slave_id,2,1);
        x=result.substring(9,11);
        x.toCharArray(a,3);
        p=strtoul(a,&endptr,16);
      return(p);
    }
long int BARN::READ_INP_MODE_BYTE (byte slave_id) 
    {     
        String result,x; unsigned long int p;char a[9];int len;char *endptr;
        result=ReadSingleRegister( slave_id,2,1);
        x=result.substring(9,11);
        x.toCharArray(a,3);
        p=strtoul(a,&endptr,16);
      return(p);
    }

long int BARN::READ_TRP_SPD_WORD(byte slave_id)
{
    String result; unsigned long int q;
        result=ReadSingleRegister( slave_id,6,1);
        q = value( result);
       
      return(q);
}

byte BARN::SAVE(byte slave_id)
{
     byte j;
        int save=((slave_id<<8)|0xFF);
        j=WriteSingleRegister(slave_id,0,save);
        if(j==1) return 1;
        else return 0;
}
byte BARN::RESET(byte slave_id)
{
    byte j,k;
		j=WriteSingleRegister(slave_id,0,8);
		if(j==1) 
			k=SAVE(slave_id);
		if(k==1) return 1;
		else return 0;
}

void BARN::hexToBytes(byte *byteArray, char *hexString)
{
        byte currentByte = 0;  
        byte byteIndex = 0;

        for (byte charIndex = 0; charIndex < strlen(hexString); charIndex++)
        {
            bool oddCharIndex = charIndex & 1;
            if (!oddCharIndex)      // If the length is even
            {
            currentByte = nibble(hexString[charIndex]) << 4;  // Odd characters go into the high nibble
            }
            else
            {
            currentByte |= nibble(hexString[charIndex]);   // Odd characters go into low nibble
            byteArray[byteIndex++] = currentByte;
            currentByte = 0;
            }
        }
}

byte BARN::nibble(char c)
{
  if (c >= '0' && c <= '9')
    return c - '0';

  if (c >= 'a' && c <= 'f')
    return c - 'a' + 10;

  if (c >= 'A' && c <= 'F')
    return c - 'A' + 10;

  return 0;  // Not a valid hexadecimal character
}

byte BARN::LRC(String s,byte len)
{
        char* ary=s.c_str();
        byte byteArray[MaxByteArraySize] = {0};
        hexToBytes(byteArray, ary);
        byte sum;
        sum=0;
        for(int i=0;i<len;i++)
        {
            sum+=byteArray[i];
        }
        byte LRC=(~sum)+1;
        
		if(LRC==byteArray[len])  return 1;
        else
        { 
            if(Sprt==0)  port->end();
            if(Sprt==1)  softport->end();
            return 0;
        }
}


long int BARN::value(String input)
{ 
	String x;char a[9];int len;char *endptr; long int p;
	len=input.length();

	x=input.substring(7,len-3);
	x.toCharArray(a,len-9);

	p=strtoul(a,&endptr,16);
	return p;
}
