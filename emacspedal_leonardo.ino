// CFE's pedal keyboard version 3.0
//
// For Atmel 32u4 based controllers such as Arduino Leonardo and compatible,
// with USB interface and USB HID functions integrated in the PIC
// (Does not work with other Arduinos which have a USB-serial converter)
//
// Carl-Fredrik Enell 20190310
// fredrik@kyla.kiruna.se

#include <Keyboard.h>

/* Configuration *****************************************/
/* Settings: Change for your pedal setup                 */
static const int noPedals = 4;
/* Key input, prefer the 4 digital IOs without PWM       */
static const uint8_t pedalPin[] = {2, 4, 7, 8} ; 
static const uint8_t modePin = 12; /* Mode switch input  */

/* Key mappings: Change to your preferences              */
/* modePin closed: Pageflip function up, home, end, down */
static const char keycode0[] = {KEY_PAGE_UP, KEY_HOME, KEY_END, KEY_PAGE_DOWN};
/* modePin open: Emacs function Ctrl, Shift, AltGr, Alt  */
static const char keycode1[] = {KEY_LEFT_CTRL, KEY_LEFT_SHIFT,  KEY_RIGHT_ALT, KEY_LEFT_ALT}; 

/* Status array. Initialise to pedals up = inputs grounded                  */
/* Swap these everywhere if your pedals have Normally Open (NO) switches    */
/* NB make sure not to swap modePin statuses so avoid search and replace... */
static uint8_t pedalState[] = {LOW, LOW, LOW, LOW};
/************** End of user configuration ********************/

uint8_t modeRead; 
uint8_t pedalRead;
int n;

void setup() {
  
  /* Initialise USB keyboard mode */
  Keyboard.begin();
 
  /* Set connectors to input and turn on pullup */
  /* Pedals */
  for (n = 0; n < noPedals; n++) {
    pinMode(pedalPin[n], INPUT_PULLUP);
  } 

  /* Mode switch input */
  pinMode(modePin, INPUT_PULLUP);

}

void loop() {
  /* Main loop: read pedals */  
  for(n = 0; n < noPedals; n++) {
  
    modeRead = digitalRead(modePin);
    pedalRead = digitalRead(pedalPin[n]);
     
      /* State change detect */
      if (pedalRead != pedalState[n]) {
        pedalState[n] = pedalRead;
         
        /* Swap HIGH and LOW for NO switches */ 
        if (pedalRead == HIGH) {
          /* Pedal pressed */
         
          if (modeRead == LOW) {
            /* Mode selector closed: Motion key: no repeat wanted on pedal -> press and release immediately*/
            Keyboard.press(keycode0[n]);            
            delay(10);
            Keyboard.releaseAll();
          }
          else {
            /* Mode selector open: press modifier key */
            Keyboard.press(keycode1[n]);
          }
          
        }
        else {
          /* Pedal released */
          if (modeRead == HIGH) {
            /* Mode selector open: release modifier key */   
            Keyboard.release(keycode1[n]);
          } 
          /* Mode selector closed: no action needed here*/
        }
         
      } /* End pedal state check */
    
  } /*End read pedals*/

  /* Sleep 50 ms */
  delay(50);

}
