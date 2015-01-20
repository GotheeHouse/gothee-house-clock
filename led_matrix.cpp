/*
================================================================================
File name: led_matrix.cpp
   System: Gothee House Clock
 Platform: Teensy 3.1 Freescale K20 (ARM Cortex M4, MK20DX256VLH7)
   Author: Madeline Usher
  Created: December 29, 2014
  Purpose: Handles the LED matrix display.  This display shows the user what 
           lightshow is currently selected.
================================================================================
  $LastChangedDate: 2015-01-14 20:19:21 -0600 (Wed, 14 Jan 2015) $
  $LastChangedBy: usher $
================================================================================
*/

#include <Adafruit_LEDBackpack.h>
#include <Adafruit_GFX.h>
#include "led_matrix.h"


////////////////////////////////////////////////////////////////////////////////
/// Icons

const uint8_t off_icon[] =
 {B00111100,
  B01000010,
  B10011001,
  B10011001,
  B10011001,
  B10011001,
  B01000010,
  B00111100};

const uint8_t smile_icon[] =
 {B00111100,
  B01000010,
  B10100101,
  B10000001,
  B10100101,
  B10011001,
  B01000010,
  B00111100};

const uint8_t neutral_icon[] =
 {B00111100,
  B01000010,
  B10100101,
  B10000001,
  B10111101,
  B10000001,
  B01000010,
  B00111100};

const uint8_t frown_icon[] =
 {B00111100,
  B01000010,
  B10100101,
  B10000001,
  B10011001,
  B10100101,
  B01000010,
  B00111100};

/*  
const uint8_t crescent_moon_icon[] =
 {B00011100,
  B00001110,
  B00000101,
  B00000101,
  B00000101,
  B00000101,
  B00001110,
  B00011100};

const uint8_t strong_man_icon[] =
 {B00011000,
  B00011000,
  B01000010,
  B01111110,
  B00011000,
  B00011000,
  B00100100,
  B01000010};
  
const uint8_t waving_woman_icon[] =
 {B00011000,
  B00011000,
  B00000010,
  B00111100,
  B01011000,
  B00011000,
  B00111100,
  B01111110};
  
const uint8_t omega_icon[] =  
 {B00111100,
  B01000010,
  B10000001,
  B10000001,
  B10000001,
  B11000011,
  B01100110,
  B11100111};

const uint8_t pi_icon[] =
 {B00000000,
  B01111111,
  B10100100,
  B00100100,
  B00100100,
  B00100100,
  B00100101,
  B01000010};

const uint8_t some_symbol_icon[] =
 {B01111100,
  B00010000,
  B00111000,
  B01000100,
  B01000100,
  B00111000,
  B00010000,
  B01111100};
  
const uint8_t star_icon[] =  
 {B00000000,
  B00010000,
  B10010010,
  B01010100,
  B00111000,
  B01010100,
  B10010010,
  B00010000};

const uint8_t infinity_icon[] =
 {B00000000,
  B00000000,
  B01100110,
  B10001001,
  B10010001,
  B01100110,
  B00000000,
  B00000000};

const uint8_t sun_icon[] =
 {B00111000,
  B01000100,
  B10000010,
  B10010010,
  B10000010,
  B01000100,
  B00111000,
  B00000000};

const uint8_t venus_icon[] =
 {B00111000,
  B01101100,
  B01000100,
  B01101100,
  B00111000,
  B00010000,
  B01111100,
  B00010000};

const uint8_t ankh_icon[] =
 {B00111000,
  B01000100,
  B01000100,
  B00101000,
  B00010000,
  B01111100,
  B00010000,
  B00010000};

const uint8_t spider_icon[] =
 {B01000010,
  B00100100,
  B10011001,
  B00111100,
  B01011010,
  B10100101,
  B00100100,
  B01000010};

const uint8_t earth_icon[] =
 {B00111000,
  B01010100,
  B10010010,
  B11111110,
  B10010010,
  B01010100,
  B00111000,
  B00000000};

const uint8_t square_yin_yang_icon[] =
 {B11110000,
  B11110110,
  B11110110,
  B11110000,
  B11110000,
  B10010000,
  B10010000,
  B11110000};

const uint8_t skull_crossbones_icon[] =
 {B10000001,
  B00111100,
  B01111110,
  B01011010,
  B01111110,
  B00100100,
  B00011000,
  B10000001};

const uint8_t heart_icon[] =
 {B00000000,
  B01100110,
  B11111111,
  B11111111,
  B11111111,
  B01111110,
  B00111100,
  B00011000};

const uint8_t heart2_icon[] =
 {B00000000,
  B01100110,
  B10011001,
  B10000001,
  B10000001,
  B01000010,
  B00100100,
  B00011000};

const uint8_t runes_icon[] =
 {B10010010,
  B01010100,
  B00111000,
  B00010000,
  B00010000,
  B00010000,
  B00010000,
  B00000000};

const uint8_t mysterious_icon[] =
 {B11000011,
  B01100110,
  B00111100,
  B01100110,
  B01100110,
  B00111100,
  B01100110,
  B11000011};

const uint8_t flame_icon[] =
 {B00000000,
  B00001000,
  B00001000,
  B00010000,
  B00100010,
  B00100010,
  B00110110,
  B00011100};

const uint8_t eye_icon[] =
 {B00000000,
  B00111100,
  B01000010,
  B11011011,
  B01000010,
  B00111100,
  B00000000,
  B00000000};

const uint8_t butterfly_icon[] =
 {B00000000,
  B01100110,
  B11011011,
  B10011001,
  B10011001,
  B10011001,
  B01100110,
  B01000010};

*/


////////////////////////////////////////////////////////////////////////////////
/// Globals

Adafruit_8x8matrix matrix = Adafruit_8x8matrix();


////////////////////////////////////////////////////////////////////////////////
/// Initialization

void initLEDMatrix ()
 {matrix.begin(LED_MATRIX_I2C_ADDR);  // pass in the address
  matrix.clear();
  matrix.writeDisplay();
  matrix.setRotation(1);
  matrix.setTextSize(1);
  matrix.setTextWrap(false);
  matrix.setTextColor(LED_ON);
  setLEDMatrixIcon(neutral_icon);
  return;}


////////////////////////////////////////////////////////////////////////////////

void setLEDMatrixIcon (const uint8_t* icon)
 {matrix.clear();
  matrix.drawBitmap(0, 0, icon, 8, 8, LED_ON);
  matrix.writeDisplay();
  return;}


void setLEDMatrixIcon (uint8_t iconID)
 {switch (iconID)
   {case ICON_OFF:
      setLEDMatrixIcon (off_icon);
      break;
    case ICON_SMILE:
      setLEDMatrixIcon (smile_icon);
      break;
    case ICON_NEUTRAL:
      setLEDMatrixIcon (neutral_icon);
      break;
    case ICON_FROWN:
      setLEDMatrixIcon (frown_icon);
      break;
    default:
      matrix.clear();
      matrix.writeDisplay();}
  return;}


////////////////////////////////////////////////////////////////////////////////

void LEDMatrixShowNum (int n)
 {matrix.clear();
  matrix.setCursor(0, 0);
  matrix.print(n);
  matrix.writeDisplay();
  return;}


////////////////////////////////////////////////////////////////////////////////
/// LEDMatrixState
///
/// State should be one of the LED_MATRIX_STATE_* values.
///

unsigned int LEDMatrixState = LED_MATRIX_STATE_FULL;


void setLEDMatrixState (unsigned int state)
 {switch (state)
   {case LED_MATRIX_STATE_OFF:
     matrix.clear();
     matrix.writeDisplay();
     LEDMatrixState = LED_MATRIX_STATE_OFF;
     break;
    case LED_MATRIX_STATE_DIM:
     matrix.setBrightness(LED_MATRIX_BRIGHTNESS_DIM);
     LEDMatrixState = LED_MATRIX_STATE_DIM;
     break;
    default:
     matrix.setBrightness(LED_MATRIX_BRIGHTNESS_FULL);
     LEDMatrixState = LED_MATRIX_STATE_FULL;}
  return;}


unsigned int getLEDMatrixState ()
 {return(LEDMatrixState);}
  

////////////////////////////////////////////////////////////////////////////////
/// End of Code
