The CFEmacspedal
================
Arduino code to use foot switches as a computer keyboard.

Required hardware
=================

* Foot switches, available in music stores etc.
* Arduino
* Cables


Contents
========

1. emacspedal_serial.ino
-------------------------

The original version from the early 2000s, last edited 2008, used on
an Arduino NG.

This program sends keycodes over the USB-serial connection to the
computer.  Requires dedicated software to inject these key codes into
the OS. I used a stripped-down version of kbdd on Linux, see
http://projects.linuxtogo.org/projects/kbdd/


* Advantages: Handles modifier keys correctly. Works on any Arduino.

* Drawbacks: Requires special software on the computer side.

2. emacspedal_uno_hid.ino
--------------------------

Version for the Arduino Uno with USB HID firmware. This ran on an original Arduino Uno that died in 2019. Requires the default USB-serial firmware to be replaced with the USB HID firmware after programming. This firmware converts serial characters to USB key presses.

* Advantages: OS-independent, presenting a normal USB keyboard to the computer.
* Drawbacks: Works only on genuine Arduino Uno boards with USB adapter implemented in firmware on a second PIC. Converts serial characters to instantaneous key events, so not really useful for modifier keys. Requires the USB-serial firmware to be replaced with USB HID firmware, which disables the normal serial communication.

3. emacspedal_leonardo.ino
--------------------------

The current incarnation of the CFEmacspedal, from 2019.
Has two selectable sets of key codes: instantaneous motion keys (Page up/down, Home, End) useful e.g. for presentations or turning music sheets, and persistent modifier keys, useful for Emacs editing.

This program uses the (now discontinued but still available) Arduino
Leonardo or compatible Atmel 32u4 based clones, which implement all
USB communication in the main processor. This makes a simple USB
keypress library available, but makes the USB-serial device
unstable. On my computer the USB connection was not usable to program
the Leonardo but I had to use an external programmer to reflash the
bootloader and upload the program.

Thus, it is recommended to use _Upload Using Programmer_ to flash this
sketch to the Leonardo. A good option is to load another Arduino of any
5 V standard USB-serial type with the Arduino ISP software and connect to the
Leonardo ISP header and reset pin as described in
https://www.arduino.cc/en/Tutorial/ArduinoISP

In the GUI, select the port of the _programmer_ Arduino but the type
(Leonardo) of the _target_ Arduino, and set the programmer type to
"Arduino as ISP".

If you encounter an error message from *avrdude* when uploading, there are two well documented issues to check. 1) add a 10 uF capacitor between Reset and GND on the programmer Arduino 2) edit the file *hardware/arduino/avr/programmers.txt*, find the entry "arduinoasisp.protocol=arduino" and change "arduino" to "stk500v1".


* Advantages: OS-independent, presenting a normal USB keyboard to the computer. No special modifications to the Arduino are needed.
A mode switch to select between two sets of key codes was added.
* Drawbacks: Works only on Leonardo and compatible boards. Program upload with an external programmer may be necessary.


Usage
------

* Change the code according to your type of pedal and desired key codes.
If your pedals are momentarily closing (NO) rather than breaking (NC), you must swap HIGH and LOW on relevant lines.
* Compile and verify the code.
* Upload the code to the Leonardo, using an external ISP programmer if necessary.
* Disconnect the Leonardo from the computer or programmer and connect the pedals to the selected input pins and ground of the Arduino.
* Mount the Leonardo on the pedals in some suitable stable way.
* (Re)connect the USB port of the Arduino to the computer.
* Enjoy!
