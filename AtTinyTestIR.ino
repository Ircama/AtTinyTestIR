/*
 * Display monitor and tester for the infrared sequence codes
 * to be used on the ATTINY85.
 *
 * https://github.com/Ircama/AtTinyTestIR.git
 *
 * (C) Ircama, 2017, CC-BY-SA 4.0
 * https://creativecommons.org/licenses/by-sa/4.0/
 * 
 * This code allows debugging IR keys through an ATtiny85 microcontroller exploiting the
 * V-USB hardware configuration (e.g., Digispark Digistump ATTINY85). ATtiny85 presents
 * itself to the PC as a keyboard USB HID; the PC needs to install a V-USB driver
 * (https://www.obdev.at/products/vusb/index.html).
 * 
 * It uses IRemote to read IR keys. IRremote.h needs editing, by  disabling all the IR
 * protocols except the needed one, in order to reduce the code size to the minimum.
 * 
 * As both IRemote and V-USB use timers/interrupts, the code tries avoiding conflicts
 * between them. This requires resetting the USB driver after a code is printed out
 * (it forces USB enumeration each time an hex sequence has to be shown).
 * 
 * How to use:
 * - edit IRremote.h and configure the needed protocols (e.g, only read SAMSUNG codes)
 * - select the related results.decode_type (e.g, decode SAMSUNG)
 * - Connect the Attiny85 to the PC via USB (with IR receiver connected to pin RECV_PIN)
 * - compile and load
 * - Open a Notepad and put the Windows focus there
 * - Press a key on the IR remote
 * - Wait for the Digispark ATTINY85 led to switch on (means valid IR received)
 * - Wait for the led to switch off (means ready for a new IR code)
 * - Read the IR code on the notepad window
 *
 * Notes:
 *  - Only ATTINY85 is supported
 *  - This code is VERY slow and should be only used for detecting/debugging IR codes
 *  - Shown data is often wrong and serveral tries are needed to consolidate an IR code
 *  - Enabling specific code filtering is advised (e.g., FILTER_SAMSUNG) to significantly
 *    improve data quality
 *  - DKeyboard library is needed (modified tiny DigisparkKeyboard library)
 *  - False errors of missing USB driver messages can occur with Windows (should be ignored)
 *  - If no data is shown, increase KEYBOARD_DELAY timer
 *  - This sketch uses 6486 bytes (or more) and needs a recent version of the ATTINY85
 *    micronucleus firmware, providing a very small size (tested with Micronucleus V2.03,
 *    /firmware/releases/t85_default.hex,
 *    from https://github.com/micronucleus/micronucleus  e.g. leaving 6520 bytes to user
 *    program)
 *  - You can leave out this code and use an ATmega328P processor with serial IO and
 *    appropriate program for much better quality.
 */

// Begin Configuration
#define RECV_PIN 0 // Infrared receiver pin (TSOP1738 or equivalent)
#define LED_PIN 1 // LED pin

//#define FILTER_SAMSUNG // filter only Samsung IR codes
#define USE_LED // flash LED when a valid IR code is detected
#define USE_KEYBOARD // Print the received IR code by simulating a PC keyboard
#define KEYBOARD_DELAY 1500 // Time to wait for the PC keyboard driver to become ready
// End Configuration

// IR Remote library
#include <IRremote.h>
IRrecv irrecv(RECV_PIN);
decode_results results;

#ifdef USE_KEYBOARD
#include "DKeyboard.h" // Use a modified tiny DigisparkKeyboard library
#endif

void setup() {
  irrecv.enableIRIn(); // Start the IR receiver
}

void loop() {
  if (irrecv.decode(&results)) {
#ifdef FILTER_SAMSUNG
  if (results.decode_type == SAMSUNG)
#endif
    {
#ifdef USE_KEYBOARD
    TIMER_DISABLE_INTR; // disable the IRremote interrupt
    DKeyboardDevice Db = DKeyboardDevice(); // instantiate the Keyboard driver
#endif
#ifdef USE_LED
    digitalWrite(LED_PIN, HIGH);
#endif
#ifdef USE_KEYBOARD
    Db.sendKeyStroke(0);
    Db.delay(KEYBOARD_DELAY); // This is needed to wait for the PC keyboard driver to become ready
    Db.sendKeyStroke(0); // This will ensure no character will be lost
#else
    delay(100);
#endif
#ifdef USE_KEYBOARD
    Db.println(results.value, HEX);
    Db.delay(300); // ensure that the transmission is performed before the interrupt is turned off
    USB_INTR_ENABLE &= ~(1 << USB_INTR_ENABLE_BIT); // turn off the USB interrupt selectively
    TIMER_ENABLE_INTR; // enable again the IRremote interrupt
    TIMER_RESET;
    delay(100);
#endif
#ifdef USE_LED
    digitalWrite(LED_PIN, LOW);
#endif
    }
  irrecv.resume(); // Receive the next value
  }
}

