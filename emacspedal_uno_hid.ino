//-----------------------------------------------------------------------------------------
// Key pedal decoder for the Arduino microcontroller
// http://www.arduino.cc/
//
// Send scancodes from Technics pedalboard (4 opening switches)
// through Arduino HID firmware.
//
// Quick HOWTO:
// 1. Compile and load this code onto the Arduino MC main processor
// 2. Flash usb-hid firmware onto the USB interface processor
// See http://mitchtech.net/arduino-usb-hid-keyboard/
// 3. Hook up the buttons to the selected input pins of your Arduino MC
// 4. Unplug and reconnect USB
// 
//
// To reprogram the Arduino, reflash the standard usb-serial firmware first 
// 
// 
//
// (C) Carl-Fredrik Enell 2012
// fredrik  (at)  kyla.kiruna.se
//
// Released under the GNU General Public License
// http://www.gnu.org/licenses/gpl.html
//
//-------------------------------------------------------------------------------------


//Serial speed expected by the usb-hid firmware
#define BAUD 9600
//Number of pedals
#define NBUTTONS 4

//Modifiers expected by HID firmware
const int ScanCodes[NBUTTONS]={1,2,64,4};//LEFTCTRL, LEFTSHIFT, RIGHTALT, LEFTALT

//Arduino digital inputs, change to your need
const int PedalPin[NBUTTONS]={6,7,12,13}; 

//Status register
int SwStat[NBUTTONS]; 
//Loop counter
int pincount; 

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
  
  int reading; //Current status
  int dum;
  
  if (pincount >= NBUTTONS) {
    pincount=0; //Loop over NBUTTONS buttons
  }  
  
  reading=digitalRead(PedalPin[pincount]); //Pin status
  
  //Check whether switch status has certainly changed
  if(reading != SwStat[pincount] && millis()-lastchange[pincount] > debounce) {
    
    if(reading==HIGH) { 
      //Switch was pressed
      SwStat[pincount]=HIGH;
      hid_byte(ScanCodes[pincount]);
    }
    else {
      //Switch was released
      SwStat[pincount]=LOW;  
      hid_byte(0);
    }
    dum=Serial.read(); // read LED status and discard
    lastchange[pincount]=millis();  //Time of status change  
    
  }  
  pincount++;
}

void hid_byte (uint8_t sc) {
    const uint8_t nc=0;
    int i;  
     
    //send 8 bytes of data: modifier,0,0,0,0,0,0,0
      Serial.write(sc);
      for(i=2; i<=8; i++) {
        Serial.write(nc,1);
      } 
}
