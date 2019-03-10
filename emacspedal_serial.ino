//-----------------------------------------------------------------------------------------
// Key pedal decoder for the Arduino microcontroller
// http://www.arduino.cc/
//
// Send scancodes for Ctrl, Shift, AltGr and Alt
// from Technics pedalboard (4 opening switches)
// to the uinput keyboard daemon (http://projects.linuxtogo.org/projects/kbdd/)
// in the "HP Slim keyboard" mode (accepts uinput scancodes without translation)
//
// Quick HOWTO:
// 1. Hook up the buttons to the selected input pins of your Arduino MC
// 2. Compile and load this code onto the Arduino MC
// 3. Compile kbdd
// 4. sudo modprobe uinput
// 5. sudo kbdd -p /dev/ttyUSB0 (wherever your usbserial) -t hpslim
//
// (C) Carl-Fredrik Enell 20080216
// fredrik  (at)  kyla.kiruna.se
//
// Released under the GNU General Public License
// http://www.gnu.org/licenses/gpl.html
//
//-------------------------------------------------------------------------------------


//Serial speed expected by kbdd in hpslim mode
#define BAUD 4800
//Number of pedals
#define NBUTTONS 4
//Release code
#define REL 128

//Scancodes, see kbdd/trunk/uinput.h and change to your wish
const int ScanCodes[NBUTTONS]={29,42,100,56}; //KEY_LEFTCTRL, KEY_LEFTSHIFT, KEY_RIGHTALT, KEY_LEFTALT

const int PedalPin[NBUTTONS]={6,7,12,13}; //Arduino digital inputs, change to your need

int pincount; //Loop counter
int reading; //Current status
int SwStat[NBUTTONS]; //Status register

//Debounce timer variables, see http://www.arduino.cc/en/Tutorial/Debounce
long lastchange[NBUTTONS]; //Last change
const long debounce=200;    //Debounce delay (ms), change if necessary

//Initialisation
void setup() { 

  //Set up usbserial
  Serial.begin(BAUD);
 
  //Initialise inputs and their status registers
  for(pincount=0; pincount<NBUTTONS; pincount++) { 
    pinMode(PedalPin[pincount],INPUT);  //All pins to input mode
    digitalWrite(PedalPin[pincount],HIGH); //Turn on internal pullup resistors
    SwStat[pincount]=LOW;    //Switches normally closed
    lastchange[pincount]=0;  //Debounce timer 
  }   

}

//Main
void loop() { 
 
  if (pincount >= NBUTTONS) {
    pincount=0; //Loop over NBUTTONS buttons
  }  
  
  reading=digitalRead(PedalPin[pincount]); //Pin status
  
  //Check whether switch status has certainly changed
  if(reading != SwStat[pincount] && millis()-lastchange[pincount] > debounce) {
    
    if(reading==HIGH) { 
      //Switch was pressed
      SwStat[pincount]=HIGH;
      //Serial.print(ScanCodes[pincount],BYTE); //Send a single-byte key-press code, old Arduino API
      Serial.write(ScanCodes[pincount]); //New API from v 1.0
    }
    else {
      //Switch was released
      SwStat[pincount]=LOW;
      //Serial.print(ScanCodes[pincount]|REL,BYTE); //Send a single-byte key-release code, old API
      Serial.write(ScanCodes[pincount]|REL); //New API
    }
    
    lastchange[pincount]=millis();  //Time of status change  
    Serial.flush();  //Discard any incoming data
    
  }  
   
  pincount++; 

}

