/*
================================================================================
File name: gothee-house-clock.ino
   System: Gothee House Clock
 Platform: Teensy 3.1 Freescale K20 (ARM Cortex M4, MK20DX256VLH7)
   Author: Madeline Usher
  Created: November 26, 2014
  Purpose: This controls the lighting for the clock.
================================================================================
  $LastChangedDate: 2015-01-09 01:59:45 -0600 (Fri, 09 Jan 2015) $
  $LastChangedBy: usher $
================================================================================
*/

/*
Pin Assignments:

  User Interface:
  - pin 18 = LED matrix display, data (I2C Data, SDA0)
  - pin 19 = LED matrix display, clock (I2C Clock, SCL0)

  - pin  5 = rotary DIP 4
  - pin  6 = rotary DIP 2
  - pin  7 = rotary DIP 8
  - pin  8 = rotary DIP 1

  Hall Effect Sensors:
  - pin 22 = Hall Effect sensor, geneva mechanism
  - pin 23 = Hall Effect sensor, pendulum
  
  XBee:
  - pin  0 = XBEE DOut -> Serial RX1
  - pin  1 = XBEE DIn -> Serial TX1
  - pin 21 = sleep
  
  Lights:
  - pin  9 = left side lights
  - pin 10 = aux lights
  - pin 11 = right side lights
  - pin 12 = top lights
  
  Miscellaneous
  - pin 17 = unconnected analog input (A3) for use in random number generation
*/

////////////////////////////////////////////////////////////////////////////////

// #include <Arduino.h>
#include <Wire.h>
#include <Adafruit_NeoPixel.h>
// #include <LowPower_Teensy3.h>
#include <Adafruit_LEDBackpack.h>
#include <Adafruit_GFX.h>
#include "time.h"
#include "selector_switch.h"
#include "led_matrix.h"
#include "lightshows.h"


////////////////////////////////////////////////////////////////////////////////

#define STATUS_LED 13

// user-interface display will sleep after this amount of user inactivity.  
// In ms.
#define UI_TIMEOUT_LIGHTSHOW_ACTIVE 20000
#define UI_TIMEOUT_NO_LIGHTSHOW_OFF  5000
#define UI_TIMEOUT_NO_LIGHTSHOW_DIM  2500

#define SELECTOR_SWITCH_POS_OFF 0

unsigned long lastUserActivityTime = 0;  // in ms


// #define WAKEUP_PIN              2
// #define WAKEUP_PIN_LLWU_ID  PIN_2

#define RANDOM_NOISE_ANALOG_INPUT 3


// TEENSY3_LP sleepCtrl = TEENSY3_LP();


////////////////////////////////////////////////////////////////////////////////
/// The setup() method runs once, when the sketch starts

void setup ()
 {pinMode(STATUS_LED, OUTPUT);
  // pinMode(WAKEUP_PIN, INPUT_PULLUP);
  initRandomGenerator();
  initLEDMatrix();
  initNeoPixels();
  initSelectorSwitch();
  // initHallEffectSensors();
  
  setLEDMatrixIcon(ICON_SMILE);
  delay(1000);
  lastUserActivityTime = millis();
  return;}


void initRandomGenerator ()
 {unsigned int seed = 0;
  for (int i = 0; i < 50; i++) 
   {for (int j = 0; j < 32; j++) 
    {// XOR the seed with the volatile lowest bit from ADC shifted.
     seed ^= ((analogRead(RANDOM_NOISE_ANALOG_INPUT) & 1) << j);}}
  randomSeed(seed);
  return;}
  
  
void blink (byte pin, int n)
 {for (int j = 0; j < n; j++)
   {digitalWrite (pin, HIGH);
    delay (200);
    digitalWrite (pin, LOW);
    delay (200);}
  return;}


////////////////////////////////////////////////////////////////////////////////
/// The loop() method runs over and over again, as long as the Arduino has power

void loop()
 {handleUI();
  updateLightshow();
  
  // testHallEffectSensors();
  
 }

////////////////////////////////////////////////////////////////////////////////
/// User Interface

void handleUI ()
 {boolean selectorChanged;
  unsigned long timeSinceLastUserActivity;
 
  byte selectorVal = selectorSwitchVal(&selectorChanged);
  if (selectorChanged)
   {if (selectorVal == SELECTOR_SWITCH_POS_OFF)
     {setLEDMatrixIcon(ICON_OFF);
      endLightshow();}
    else
     {const uint8_t* icon = startLightShow(selectorVal - 1);
      if (icon)
       {setLEDMatrixIcon(icon);}
      else
       {setLEDMatrixIcon(ICON_FROWN);}}
    lastUserActivityTime = millis();}
    
  timeSinceLastUserActivity = elapsedMillis (lastUserActivityTime);
  switch (getLEDMatrixState())
   {case LED_MATRIX_STATE_OFF:
     if (selectorChanged)
      {setLEDMatrixState(LED_MATRIX_STATE_FULL);}
     break;
    case LED_MATRIX_STATE_DIM:
     if (selectorChanged)
      {setLEDMatrixState(LED_MATRIX_STATE_FULL);}
     else if ((selectorVal == SELECTOR_SWITCH_POS_OFF) && 
             (timeSinceLastUserActivity > UI_TIMEOUT_NO_LIGHTSHOW_OFF))
      {setLEDMatrixState(LED_MATRIX_STATE_OFF);
       // sleep here?
      }
     break;
    case LED_MATRIX_STATE_FULL:
     if (((selectorVal != SELECTOR_SWITCH_POS_OFF) && 
          (timeSinceLastUserActivity > UI_TIMEOUT_LIGHTSHOW_ACTIVE)) ||
         ((selectorVal == SELECTOR_SWITCH_POS_OFF) && 
          (timeSinceLastUserActivity > UI_TIMEOUT_NO_LIGHTSHOW_DIM)))
      {setLEDMatrixState(LED_MATRIX_STATE_DIM);}
     break;
   }
  return;}



/*

////////////////////////////////////////////////////////////////////////////////
/// When putting the system to sleep we'll use the VLPS (very low power stop)
/// mode.

void sleepFairyRing ()
 {endLightshow(pShrooms);
  disableLCD();
  lcdOn = false;
  sleepTLC5940();
  sleepCtrl.DeepSleep(GPIO_WAKE, WAKEUP_PIN_LLWU_ID);
  return;}


void wakeFairyRing ()
 {wakeTLC5940();
  enableLCD();
  lcdOn = true;
  lastUserActivityTime = millis();
  return;}


*/

  
////////////////////////////////////////////////////////////////////////////////
/// Hall Effect Sensors:

#define HALL_EFFECT_GENEVA_PIN   22
#define HALL_EFFECT_PENDULUM_PIN 23


void initHallEffectSensors ()
 {pinMode(HALL_EFFECT_PENDULUM_PIN, INPUT_PULLUP);
  pinMode(HALL_EFFECT_GENEVA_PIN, INPUT_PULLUP);
  return;}


void testHallEffectSensors ()
 {if (digitalRead(HALL_EFFECT_PENDULUM_PIN))
   {digitalWrite (STATUS_LED, LOW);}
  else
   {digitalWrite (STATUS_LED, HIGH);}
  return;}



////////////////////////////////////////////////////////////////////////////////
/// End of Code
