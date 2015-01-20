/*
================================================================================
File name: selector_switch.h
   System: Gothee House Clock
 Platform: Teensy 3.1 Freescale K20 (ARM Cortex M4, MK20DX256VLH7)
   Author: Madeline Usher
  Created: August 10, 2013
  Purpose: Handles the rotary DIP switch used to select lightshows.
================================================================================
  $LastChangedDate: 2015-01-09 01:59:45 -0600 (Fri, 09 Jan 2015) $
  $LastChangedBy: usher $
================================================================================
*/

#ifndef SELECTOR_SWITCH_h
#define SELECTOR_SWITCH_h

#include <Arduino.h>
#include <inttypes.h>


////////////////////////////////////////////////////////////////////////////////

#define SELECTOR_PIN_1   8
#define SELECTOR_PIN_2   6
#define SELECTOR_PIN_4   5
#define SELECTOR_PIN_8   7

#define SELECTOR_POSITIONS 16

#define SELECTOR_DEBOUNCE_DELAY  50  // in ms

// My switch's knob was mounted so that straight up was position #2.  I want it
// to be postion zero.
#define SELECTOR_OFFSET 2


////////////////////////////////////////////////////////////////////////////////
/// External functions

extern void initSelectorSwitch ();
extern byte selectorSwitchVal (boolean *valChanged);


////////////////////////////////////////////////////////////////////////////////

#endif  // SELECTOR_SWITCH_h

////////////////////////////////////////////////////////////////////////////////
/// End of Code
