#ifndef C_USBhost_H
#define C_USBhost_H
#include "NeoSWSerial.h"
#include <Keyboard.h>
#include <Arduino.h>

class C_USBhost
{
public:
   // C_USBhost(HardwareSerial& serial);
    C_USBhost(byte rx_pin, byte tx_pin, bool debug_state);
    void Begin(unsigned long baud_rate);
    byte GetKey();   
    static void ReleaseAllButtons(char* reason);   

private:
  NeoSWSerial* serial;  
  char hidText[27];
  byte hbc = 0;                                                     // hidText string buffer count 
  byte rawHID[8];                                                   // modifier_bit_map, manufacturer(ignore) , key1, key2, key3, key4, key5, key6
  byte prevRawHID[8];
  byte fullBufferFlag_preventHold;
  byte collectedAsciiValues[6];

  byte IgnoreBytesIfUseless(byte index);                             
  void ConvertInputToBytes(char* input, byte* raw_bytes);                              
  void Send_Report(byte* bytes);         
  void SaveTheKeys();       
  byte HID_to_ASCII(byte key, bool shiftDown, bool ctrlDown, bool altDown, bool winDown);
  byte HID_to_SPECIAL(bool shiftDown, bool ctrlDown, bool altDown, bool winDown);                     
  bool WasAnyKeyPressed();      
  bool WasKeyPreviouslyPressed(byte key);                              
  bool WasModifierPressed();                               
  void GetKeysPressed(byte* keys_pressed);                                    
  bool WasShiftDown();
  bool WasCtrlDown();
  bool WasAltDown();
  bool WasWindowsDown();                                     
  bool IsBitHigh(byte byteToConvert, byte bitToReturn);              
  void CleanUpVars();                                      
  void FullBuffer_BugPrevention();                         
};




#endif
