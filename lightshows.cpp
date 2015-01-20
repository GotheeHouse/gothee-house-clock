/*
================================================================================
File name: lightshows.cpp
   System: Gothee House Clock
 Platform: Teensy 3.1 Freescale K20 (ARM Cortex M4, MK20DX256VLH7)
   Author: Madeline Usher
  Created: January 7, 2015
  Purpose: 
================================================================================
  $LastChangedDate: 2015-01-20 01:15:47 -0600 (Tue, 20 Jan 2015) $
  $LastChangedBy: usher $
================================================================================
*/

#include "lightshows.h"


#include "lightshow_cylon_eye.cpp"
#include "lightshow_fire.cpp"
#include "lightshow_liquid.cpp"
#include "lightshow_pulsing_rainbow.cpp"
#include "lightshow_rainbowtest.cpp"
#include "lightshow_space_wave_test.cpp"
#include "lightshow_tempstuff.cpp"
#include "lightshow_test_rgb.cpp"
#include "lightshow_theater_chase_test.cpp"
#include "lightshow_time_wave_test.cpp"


////////////////////////////////////////////////////////////////////////////////

#define NUM_LIGHTSHOWS 9

const uint8_t* builtinLightshows[] = 
 {LIGHTSHOW_Cylon_Eye,
  LIGHTSHOW_RainbowTest,
  LIGHTSHOW_Theater_Chase_Test,
  LIGHTSHOW_Liquid,
  LIGHTSHOW_Fire,
  LIGHTSHOW_Pulsing_Rainbow,
  LIGHTSHOW_Space_Wave_Test,
  LIGHTSHOW_Time_Wave_Test,
  LIGHTSHOW_Test_RGB};


////////////////////////////////////////////////////////////////////////////////
/// The following lightshows can be chosen when picking randomly.

#define NUM_RANDOM_LIGHTSHOWS 5

const uint8_t* randomLightshows[] = 
 {LIGHTSHOW_Cylon_Eye,
  LIGHTSHOW_RainbowTest,
  LIGHTSHOW_Theater_Chase_Test,
  LIGHTSHOW_Liquid,
  LIGHTSHOW_Fire,};
  
#define RANDOM_LIGHTSHOW_DURATION 600000  // in ms
#define RANDOM_LIGHTSHOWS_ID 14

boolean randomLightshowActive = false;

const uint8_t randomLightshow_icon[] =
 {B00111000,
  B01000100,
  B00000100,
  B00001000,
  B00010000,
  B00010000,
  B00000000,
  B00010000};


////////////////////////////////////////////////////////////////////////////////
/// Lightshow Devices (i.e. the NeoPixel strips)

#define NEOPIXEL_TOP   0
#define NEOPIXEL_TOP_PIN 12
#define NEOPIXEL_TOP_NUM_LEDS 37

#define NEOPIXEL_LEFT  1
#define NEOPIXEL_LEFT_PIN 10
#define NEOPIXEL_LEFT_NUM_LEDS 71

#define NEOPIXEL_RIGHT 2
#define NEOPIXEL_RIGHT_PIN 11
#define NEOPIXEL_RIGHT_NUM_LEDS 69

#define NEOPIXEL_AUX_PIN 9
#define NEOPIXEL_AUX_NUM_LEDS 0


#define NUM_LIGHTSHOW_DEVICES 3


LightshowDevice_NeoPixel topStrip = 
   LightshowDevice_NeoPixel(NEOPIXEL_TOP, NEOPIXEL_TOP_NUM_LEDS, 
                            NEOPIXEL_TOP_PIN, NEO_GRB + NEO_KHZ800);

LightshowDevice_NeoPixel leftStrip = 
   LightshowDevice_NeoPixel(NEOPIXEL_LEFT, NEOPIXEL_LEFT_NUM_LEDS, 
                            NEOPIXEL_LEFT_PIN, NEO_GRB + NEO_KHZ800);

LightshowDevice_NeoPixel rightStrip = 
   LightshowDevice_NeoPixel(NEOPIXEL_RIGHT, NEOPIXEL_RIGHT_NUM_LEDS, 
                            NEOPIXEL_RIGHT_PIN, NEO_GRB + NEO_KHZ800);

LightshowDevice_NeoPixel lightshowDevices[] = 
 {topStrip,
  leftStrip,
  rightStrip};



void initNeoPixels ()
 {for (int j = 0; j < NUM_LIGHTSHOW_DEVICES; j++)
   {lightshowDevices[j].initLightshowDevice();}
  return;}


////////////////////////////////////////////////////////////////////////////////

// There's really no good reason to update more often than 100 times per second:
#define LIGHTSHOW_UPDATE_INTERVAL 10  // in ms


unsigned long lightshowStartTime;
unsigned long lightshowLastUpdateTime;
unsigned int lightshowIdleCounter = 0;
const uint8_t* currentLightshow = 0;  // pointer to the stored data describing
                                      // the lightshow that's currently running


////////////////////////////////////////////////////////////////////////////////
/// startLightShow
///
/// Pass it a pointer to the bytes describing the lightshow.  
/// (e.g. LIGHTSHOW_Test_RGB).
///
/// Returns a bitmap for the LED Matrix Display.
///

const uint8_t* startLightShow (const uint8_t* lightshowDesc)
 {if (currentLightshow)
   {endLightshow();}
  
  LightshowDescHeader* header = (LightshowDescHeader*)lightshowDesc;
  LightshowDeviceOpsHeader* opsHeader;
  
  currentLightshow = lightshowDesc;
  lightshowStartTime = millis();
  lightshowLastUpdateTime = 0;
  
  const uint8_t* lightshowDescEnd = lightshowDesc + header->lightshowDescSize -1;
  const uint8_t* lightshowDescPtr = lightshowDesc + sizeof(LightshowDescHeader);
  while (lightshowDescPtr < lightshowDescEnd)
   {opsHeader = (LightshowDeviceOpsHeader*)lightshowDescPtr;
    lightshowDevices[opsHeader->deviceID].startLightshow
      (lightshowDescPtr + sizeof(LightshowDeviceOpsHeader),
       opsHeader->lightshowOpsSize - sizeof(LightshowDeviceOpsHeader));
    lightshowDescPtr += opsHeader->lightshowOpsSize;}
  return(header->icon);}


const uint8_t* startLightShow (uint8_t lightshowID)
 {const uint8_t* icon = 0;
  randomLightshowActive = false;
  if (lightshowID < NUM_LIGHTSHOWS)
   {icon = startLightShow(builtinLightshows[lightshowID]);}
  else if (lightshowID == RANDOM_LIGHTSHOWS_ID)
   {icon = startRandomLightShow();}
  else
   {endLightshow();}
  return(icon);}



const uint8_t* startRandomLightShow ()
 {uint16_t id = chooseRandomLightShow();
  startLightShow(randomLightshows[id]);
  randomLightshowActive = true;
  return(randomLightshow_icon);}


uint16_t randomPlaylist[NUM_RANDOM_LIGHTSHOWS];
int16_t randomPlaylistPos = -1;

uint16_t chooseRandomLightShow ()
 {if (randomPlaylistPos == -1)
   {// need to initialize the playlist
    for (int j = 0; j < NUM_RANDOM_LIGHTSHOWS; j++)
      {randomPlaylist[j] = j;}
    randomPlaylistPos = NUM_RANDOM_LIGHTSHOWS;}
  
  randomPlaylistPos++;
  
  if (randomPlaylistPos >= NUM_RANDOM_LIGHTSHOWS)
   {for (int j = NUM_RANDOM_LIGHTSHOWS - 1; j > 0; j--)
     {int k = random(j + 1);
      uint16_t tmp = randomPlaylist[j];
      randomPlaylist[j] = randomPlaylist[k];
      randomPlaylist[k] = tmp;}
    randomPlaylistPos = 0;}
  
  return(randomPlaylist[randomPlaylistPos]);}


////////////////////////////////////////////////////////////////////////////////
/// endLightshow

void endLightshow ()
 {for (int j = 0; j < NUM_LIGHTSHOW_DEVICES; j++)
   {lightshowDevices[j].endLightshow();}
  currentLightshow = 0;
  randomLightshowActive = false;
  return;}


////////////////////////////////////////////////////////////////////////////////
/// updateLightshow
///
/// Lightshow timing works on the elapsed time (in milliseconds) since the start
/// of the lightshow.  This assumes that no single lightshow will run for more 
/// than 49.7 days.
///

void updateLightshow ()
 {if (currentLightshow)
   {unsigned long time = elapsedMillis(lightshowStartTime);
    if (randomLightshowActive && (time > RANDOM_LIGHTSHOW_DURATION))
     {startRandomLightShow();
      time = elapsedMillis(lightshowStartTime);}
    if (!lightshowLastUpdateTime ||
        ((time - lightshowLastUpdateTime) >= LIGHTSHOW_UPDATE_INTERVAL))
     {lightshowLastUpdateTime = time;
      lightshowIdleCounter = 0;
      // Serial.print("lightshowIdleCounter = ");
      // Serial.println(lightshowIdleCounter);
      for (int j = 0; j < NUM_LIGHTSHOW_DEVICES; j++)
       {lightshowDevices[j].updateLightshow(time);}}
    else
     {lightshowIdleCounter++;}}
  return;}


////////////////////////////////////////////////////////////////////////////////
/// End of Code
