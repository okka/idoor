#include <MFRC522.h>
#include <SPI.h>

#define SAD 10
#define RST 5
MFRC522 nfc(SAD, RST);


#define ledPinOpen    2
#define ledPinClosed  3

#include <Servo.h> 
 
Servo toyservo;  // create servo object to control a servo 

 
void setup() {

 pinMode(ledPinOpen, OUTPUT);
 pinMode(ledPinClosed, OUTPUT);
  toyservo.attach(9);
      toyservo.write(90);
      
  pinMode(ledPinOpen  , LOW);   
  pinMode(ledPinClosed, LOW);   
  SPI.begin();
  Serial.begin(115200);
  Serial.println("Looking for MFRC522.");
  nfc.begin();
  byte version = nfc.getFirmwareVersion();
  if (! version) {
    Serial.print("Didn't find MFRC522 board.");
    while(1); //halt
  }

  Serial.print("Found chip MFRC522 ");
  Serial.print("Firmware ver. 0x");
  Serial.print(version, HEX);
  Serial.println(".");

}


#define AUTHORIZED_COUNT 1 /*If you want more Authorized of cards set the count here, and then add the serials below*/
byte Authorized[AUTHORIZED_COUNT][6] = {

                            {0x7C, 0x16, 0x78, 0x32, 0xFF, 0xFF, }
                           /*,{0x10, 0x14, 0x39, 0x2E, 0xFF, 0xFF, }*/  /*f. example how to add more authorized cards*/
                            
                          };
                          
void printSerial(byte *serial);
boolean isSame(byte *key, byte *serial);
boolean isAuthorized(byte *serial);

void loop() {
 byte status;
  byte data[MAX_LEN];
  byte serial[5];
  boolean Opening = false;
  digitalWrite(ledPinOpen, LOW);
  digitalWrite(ledPinClosed, LOW);
  status = nfc.requestTag(MF1_REQIDL, data);

  if (status == MI_OK) {
    status = nfc.antiCollision(data);
    memcpy(serial, data, 5);
    
    if(isAuthorized(serial))
    { 
      
      
      Serial.println("Authenticated");
      Opening = true;
      digitalWrite(ledPinOpen, HIGH);
       toyservo.write(0);

       tone(8, 1047, 50);
       delay(50);
       tone(8, 2093, 200);
       delay(200);
       tone(8, 4186, 500);
       delay(500);   
   
      delay(3000); 
       toyservo.write(90);
       delay(100);
    }
    else
    { 
      printSerial(serial);
      Serial.println("is NOT authenticated");
      

      Opening = false;
      digitalWrite(ledPinClosed, HIGH);
      tone(8, 3520, 500);
      delay(500);
      digitalWrite(ledPinClosed, LOW);
      delay(500);
      digitalWrite(ledPinClosed, HIGH);
      tone(8, 3520, 500);
      delay(500);
      digitalWrite(ledPinClosed, LOW);
      delay(500);      
      digitalWrite(ledPinClosed, HIGH);
      tone(8, 3520, 500);
      delay(500);
             

    }
    
    nfc.haltTag();
    digitalWrite(ledPinOpen, LOW);
    digitalWrite(ledPinClosed, LOW);
    delay(1000);
  }//if (status == MI_OK)

  delay(500);
  
}


boolean isSame(byte *key, byte *serial)
{
    for (int i = 0; i < 4; i++) {
      if (key[i] != serial[i])
      { 
        return false; 
      }
    }
    
    return true;

}

boolean isAuthorized(byte *serial)
{
    for(int i = 0; i<AUTHORIZED_COUNT; i++)
    {
      if(isSame(serial, Authorized[i]))
        return true;
    }
   return false;
}

void printSerial(byte *serial)
{
        Serial.print("Serial:");
    for (int i = 0; i < 4; i++) {
      Serial.print(serial[i], HEX);
      Serial.print(" ");
    }
}
