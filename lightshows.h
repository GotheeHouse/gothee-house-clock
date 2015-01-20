/*
================================================================================
File name: lightshows.h
   System: Gothee House Clock
 Platform: Teensy 3.1 Freescale K20 (ARM Cortex M4, MK20DX256VLH7)
   Author: Madeline Usher
  Created: January 7, 2015
  Purpose: 
================================================================================
  $LastChangedDate: 2015-01-16 20:35:17 -0600 (Fri, 16 Jan 2015) $
  $LastChangedBy: usher $
================================================================================
*/

#ifndef LIGHTSHOWS_h
#define LIGHTSHOWS_h

#include <inttypes.h>
#include "time.h"
#include "lightshow_basic-defs.h"
#include "lightshow_devices.h"


////////////////////////////////////////////////////////////////////////////////
/// Lightshows stored in memory are an array of bytes with the following format:
/// 
/// - [2 bytes] Size (in bytes) of the entire lightshow (including this field)
/// - [8 bytes] Icon (bitmap for the LED matrix display)
/// - Zero or more device ops lists, where each device ops list is:
///   - [2 bytes] Size (in bytes) of the device ops list (including this field 
///               and the ID)
///   - [1 byte] ID of the device
///   - Zero or more stored lightshow ops
///
/// Note that the Teensy is little-endian by default -- least signifcant byte 
/// first.
///
////////////////////////////////////////////////////////////////////////////////

void initNeoPixels ();


// Returns a bitmap for the LED Matrix Display.
const uint8_t* startLightShow (const uint8_t* lightshowStored);
const uint8_t* startLightShow (uint8_t lightshowID);

const uint8_t* startRandomLightShow ();
uint16_t chooseRandomLightShow ();

void updateLightshow ();

void endLightshow ();


////////////////////////////////////////////////////////////////////////////////

typedef struct __attribute__((__packed__)) LightshowDescHeader
 {uint16_t lightshowDescSize;
  uint8_t icon[8];}
 LightshowDescHeader;


typedef struct __attribute__((__packed__)) LightshowDeviceOpsHeader
 {uint16_t lightshowOpsSize;
  uint8_t deviceID;}
 LightshowDeviceOpsHeader;


////////////////////////////////////////////////////////////////////////////////

#endif  // LIGHTSHOWS_h

////////////////////////////////////////////////////////////////////////////////
/// End of Code
