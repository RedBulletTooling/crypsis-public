#include "NeoSWSerial.h"

NeoSWSerial SMSSERIAL(9, 8);

void setup() {
  Serial.begin(115200);
  SMSSERIAL.begin(9600);
  digitalWrite(11, HIGH);
  delay(2000);
  digitalWrite(11, LOW);
}

void loop() {
  
  if (SMSSERIAL.available()) {
    Serial.write(SMSSERIAL.read());
  }
  
  if (Serial.available()) {
    SMSSERIAL.write(Serial.read());
  }
  
}

/*
 * 
 delay(2000);
  
  a
 * 
 * 
 * SMSSERIAL.print("MODE 6\r\n");
  delay(50);
  SMSSERIAL.print("KEYBOARD 0\r\n");
  delay(50);
  SMSSERIAL.print("BAUD 38400\r\n");
  delay(50);
  
 * 
 */
//Commands (every new line is new enter - the slashes are to be erased before entering);
//MODE 6
//KEYBOARD 0
//BAUD 38400
//AT&W
