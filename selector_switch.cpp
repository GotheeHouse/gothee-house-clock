/*
================================================================================
File name: selector_switch.cpp
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

#include "selector_switch.h"


////////////////////////////////////////////////////////////////////////////////
/// Globals

byte selectorVal;
byte selectorValPrev;
unsigned long selectorDebounceStart;


////////////////////////////////////////////////////////////////////////////////
/// Initialization

void initSelectorSwitch ()
 {pinMode(SELECTOR_PIN_1, INPUT_PULLUP);
  pinMode(SELECTOR_PIN_2, INPUT_PULLUP);
  pinMode(SELECTOR_PIN_4, INPUT_PULLUP);
  pinMode(SELECTOR_PIN_8, INPUT_PULLUP);
  selectorVal = 255;
  selectorValPrev = 255;
  return;}


////////////////////////////////////////////////////////////////////////////////
/// selectorSwitchVal
///
// valChanged is set to true if the value changed since the last read; otherwise 
// set to false.
//

byte selectorSwitchVal (boolean *valChanged)
 {byte val = !digitalRead(SELECTOR_PIN_1);
  val += !digitalRead(SELECTOR_PIN_2) << 1;
  val += !digitalRead(SELECTOR_PIN_4) << 2;
  val += !digitalRead(SELECTOR_PIN_8) << 3;
  val = val + SELECTOR_POSITIONS - SELECTOR_OFFSET;
  if (val >= SELECTOR_POSITIONS)
   {val -= SELECTOR_POSITIONS;}
  *valChanged = false;
  if (val != selectorVal)
   {if (val != selectorValPrev)
     { // just changed
      selectorDebounceStart = millis();
     }
    else if ((millis() - selectorDebounceStart) > SELECTOR_DEBOUNCE_DELAY)
     { // It's been the same value for greater than the debounce delay
       // so change the reported value.
      selectorVal = val;
      *valChanged = true;
     }
   }
  selectorValPrev = val;
  return(selectorVal);}


////////////////////////////////////////////////////////////////////////////////
/// End of Code
