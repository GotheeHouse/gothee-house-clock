/*
================================================================================
File name: lightshow_basic_defs.h
   System: Gothee House Clock
 Platform: Teensy 3.1 Freescale K20 (ARM Cortex M4, MK20DX256VLH7)
   Author: Madeline Usher
  Created: January 8, 2015
  Purpose: Several files need these definitions, often in an entertwining way,
           so they are broken out here separately.
================================================================================
  $LastChangedDate: 2015-01-16 22:43:52 -0600 (Fri, 16 Jan 2015) $
  $LastChangedBy: usher $
================================================================================
*/

#ifndef LIGHTSHOW_BASIC_DEFS_h
#define LIGHTSHOW_BASIC_DEFS_h

#include <inttypes.h>
#include <Adafruit_NeoPixel.h>


////////////////////////////////////////////////////////////////////////////////

typedef uint32_t Color;

typedef uint16_t LightshowOpTypeID;


////////////////////////////////////////////////////////////////////////////////

#define SHIFT_DIR_DOWN   0
#define SHIFT_DIR_RIGHT  0
#define SHIFT_DIR_UP     1
#define SHIFT_DIR_LEFT   1


////////////////////////////////////////////////////////////////////////////////
/// Blend Modes

#define BLEND_MULTIPLY         0
#define BLEND_SCREEN           1
#define BLEND_OVERLAY          2
#define BLEND_HARDLIGHT        3
#define BLEND_SOFTLIGHT        4
#define BLEND_REPLACE          5
#define BLEND_COLOR_DODGE      6
#define BLEND_COLOR_BURN       7
#define BLEND_A_MINUS_B        8
#define BLEND_B_MINUS_A        9


////////////////////////////////////////////////////////////////////////////////
/// Colors

typedef struct ColorHSL
 {float hue;         // 0.0 -> 360.0
  float saturation;  // 0.0 ->   1.0
  float luminance;}  // 0.0 ->   1.0
 ColorHSL;


////////////////////////////////////////////////////////////////////////////////
/// LightshowOp
///
/// This data structure combines the stored lightshow op and the live lightshow
/// op data.
/// 

typedef struct LightshowOp
 {LightshowOpTypeID opTypeID;  // one of the LI_* constants
  const uint8_t* storedOp;
  void* opData;}
 LightshowOp;
 
 
////////////////////////////////////////////////////////////////////////////////
/// LightshowDevice_NeoPixel

class LightshowDevice_NeoPixel : public Adafruit_NeoPixel
 {public:
  
  // Constructor: number of LEDs, pin number, LED type
  LightshowDevice_NeoPixel(uint8_t deviceID, uint16_t n, uint8_t p=6,
                           uint8_t t=NEO_GRB + NEO_KHZ800);
  ~LightshowDevice_NeoPixel();
  
  void initLightshowDevice ();
  
  void startLightshow (const uint8_t* opsStart, uint16_t lightshowSize);

  void endLightshow ();
  
  void updateLightshow (unsigned long time);
  
  boolean lightShowIsRunning ();
  
  void clearBackBuffer ();
  void swapBuffers ();
  boolean maybeUpdateNeoPixels ();
  void setBBPixelColor(uint16_t n, uint32_t c);
  void setBBColor(uint32_t c);
  uint32_t getBBPixelColor(uint16_t n);
    
  void reverseBackBuffer ();
  void invertBackBufferColors ();
  void scaleBackBufferColors (float scale);
  void copyToBackBuffer (uint8_t* pixelBuffer, int shiftBy);
  void copyToBackBuffer (uint8_t* pixelBuffer, float shiftBy, 
                         boolean antialias);
    
  uint8_t* makePixelBuffer ();
  uint8_t* swapWithBackBuffer (uint8_t* pixelBuffer);
  void clearPixelBuffer (uint8_t* pixelBuffer);
  uint32_t getPixelBufferColor(uint8_t* pixelBuffer, uint16_t n);
  void blendWithPixelBuffer (uint8_t* pixelBuffer, uint8_t blendMode);
  
  uint32_t scaleColorBrightness (uint32_t color, float scale);
  uint32_t blendColors_Linear (uint32_t color1, uint32_t color2, 
                               float color1Weight);
  ColorHSL* RGBtoHSL (uint32_t rgb, ColorHSL* hsl);
  uint32_t HSLtoRGB (ColorHSL* hsl);
  
  
  protected:
  
  uint8_t id;                  // unique integer ID of this device
  uint8_t* pixels2;            // This is the second pixel array, for double-buffering
  const uint8_t* lightshowOpsStart;  // start of the lightshow's stored ops
  const uint8_t* lightshowOpsEnd;    // end of the lightshow's stored ops
  const uint8_t* lightshowOpPtr;     // location of the next stored lightshow op
  LightshowOp* currentOp;
  unsigned long lastOpEndTime;
  
  float wrappedPixelPos (float pos);
  void beginNextLightshowOp (unsigned long time);
  float blendColorVals_Multiply (float a, float b);
  float blendColorVals_Screen (float a, float b);
  float blendColorVals_Overlay (float a, float b);
  float blendColorVals_SoftLight (float a, float b);
  float blendColorVals_Replace (float a, float b);
  float blendColorVals_ColorDodge (float a, float b);
  float blendColorVals_ColorBurn (float a, float b);
  float blendColorVals_Subtract (float a, float b);
  
  float HSLcolorComponent (float t1, float t2, float t3);
  
 };


////////////////////////////////////////////////////////////////////////////////

#endif  // LIGHTSHOW_BASIC_DEFS_h

////////////////////////////////////////////////////////////////////////////////
/// End of Code
