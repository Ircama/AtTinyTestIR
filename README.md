# AtTinyTestIR
**Arduino display monitor and tester of infrared sequence codes for ATTINY85**
This Arduino sketch receives IR sequences and write it to a standard text editor on the PC it by emulating an [USB keyboard](https://digistump.com/wiki/digispark/tutorials/digikeyboard).

This monitor is linked to https://github.com/Ircama/IR2SLink

(C) Ircama, 2017, CC-BY-SA 4.0
https://creativecommons.org/licenses/by-sa/4.0/

## Installation
Create a new folder called "AtTinyTestIR" in your Arduino sketchbook folder.
Place AtTinyTestIR.ino in the "AtTinyTestIR" folder.

Create a new folder called "DKeyboard" under the folder named "libraries" in your Arduino sketchbook folder.
Create the folder "libraries" in case it does not exist yet. Copy all the files under libraries/DKeyboard
to the target "DKeyboard" subfolder.

This software needs the [IRemote](https://github.com/z3t0/Arduino-IRremote) library. It also uses a modified version of [DKeyboard](https://github.com/digistump/DigistumpArduino/tree/master/digistump-avr/libraries/DigisparkKeyboard) as previously mentioned; the modification slightly reduces its size so that it can be loaded to the Digispark ATTINY85 device.

The Digispark ATTINT85 needs to install the latest [Micronucleus](https://github.com/micronucleus/micronucleus) bootloader to enable available rooms for this sketch.

Check also this note: https://gist.github.com/Ircama/22707e938e9c8f169d9fe187797a2a2c

## Description
This code allows debugging IR keys through an ATtiny85 microcontroller exploiting the
V-USB hardware configuration (e.g., Digispark Digistump ATTINY85). ATtiny85 presents
itself to the PC as a keyboard USB HID; the PC needs to install a V-USB driver
(https://www.obdev.at/products/vusb/index.html).

It uses IRemote to read IR keys. IRremote.h needs editing, by  disabling all the IR
protocols except the needed one, in order to reduce the code size to the minimum.

As both IRemote and V-USB use timers/interrupts, the code tries avoiding conflicts
between them. This requires resetting the USB driver after a code is printed out
(it forces USB enumeration each time an hex sequence has to be shown).
 
## How to use
- edit IRremote.h and configure the needed protocols (e.g, only read SAMSUNG codes)
- select the related results.decode_type (e.g, decode SAMSUNG)
- Connect the Attiny85 to the PC via USB (with IR receiver connected to pin RECV_PIN)
- compile and load
- Open a Notepad and put the Windows focus there
- Press a key on the IR remote
- Wait for the Digispark ATTINY85 led to switch on (means valid IR received)
- Wait for the led to switch off (means ready for a new IR code)
- Read the IR code on the notepad window

## Notes
 - Only ATTINY85 is supported
 - This code is VERY slow and should be only used for detecting/debugging IR codes
 - Shown data is often wrong and serveral tries are needed to consolidate an IR code
 - Enabling specific code filtering is advised (e.g., FILTER_SAMSUNG) to significantly
   improve data quality
 - DKeyboard library is needed (modified tiny DigisparkKeyboard library)
 - False errors of missing USB driver messages can occur with Windows (should be ignored)
 - If no data is shown, increase KEYBOARD_DELAY timer
 - This sketch uses 6486 bytes (or more) and needs a recent version of the ATTINY85
   micronucleus firmware, providing a very small size (tested with Micronucleus V2.03,
   /firmware/releases/t85_default.hex,
   from https://github.com/micronucleus/micronucleus  e.g. leaving 6520 bytes to user
   program)
 - You can leave out this code and use an ATmega328P processor with serial IO and
   appropriate program for much better quality.
