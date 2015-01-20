/*
================================================================================
File name: led_matrix.h
   System: Gothee House Clock
 Platform: Teensy 3.1 Freescale K20 (ARM Cortex M4, MK20DX256VLH7)
   Author: Madeline Usher
  Created: December 29, 2014
  Purpose: Handles the LED matrix display.  This display shows the user what 
           lightshow is currently selected.
================================================================================
  $LastChangedDate: 2015-01-09 01:59:45 -0600 (Fri, 09 Jan 2015) $
  $LastChangedBy: usher $
================================================================================
*/

#ifndef LED_MATRIX_h
#define LED_MATRIX_h

#include <Arduino.h>
#include <inttypes.h>


////////////////////////////////////////////////////////////////////////////////

#define LED_MATRIX_I2C_ADDR 0x70

#define LED_MATRIX_STATE_OFF   0
#define LED_MATRIX_STATE_DIM   1
#define LED_MATRIX_STATE_FULL  2  // full brightness

#define LED_MATRIX_BRIGHTNESS_DIM    2
#define LED_MATRIX_BRIGHTNESS_FULL  15


////////////////////////////////////////////////////////////////////////////////
/// Common icons

#define ICON_OFF      1
#define ICON_SMILE    2
#define ICON_NEUTRAL  3
#define ICON_FROWN    4


////////////////////////////////////////////////////////////////////////////////
/// External functions

extern void initLEDMatrix ();
extern void setLEDMatrixIcon (const uint8_t* icon);
extern void setLEDMatrixIcon (uint8_t iconID);
extern void LEDMatrixShowNum (int n);

extern unsigned int getLEDMatrixState ();
extern void setLEDMatrixState (unsigned int state);


////////////////////////////////////////////////////////////////////////////////

#endif  // LED_MATRIX_h

////////////////////////////////////////////////////////////////////////////////
/// End of Code
