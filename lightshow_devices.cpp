/*
================================================================================
File name: lightshow_devices.cpp
   System: Gothee House Clock
 Platform: Teensy 3.1 Freescale K20 (ARM Cortex M4, MK20DX256VLH7)
   Author: Madeline Usher
  Created: January 7, 2015
  Purpose: Each lighsthow device can run lightshow ops.  In this project, each 
           device is a NeoPixel strip.
================================================================================
  $LastChangedDate: 2015-01-16 22:43:52 -0600 (Fri, 16 Jan 2015) $
  $LastChangedBy: usher $
================================================================================
*/

#include "lightshow_devices.h"


////////////////////////////////////////////////////////////////////////////////
/// Constructor

LightshowDevice_NeoPixel::LightshowDevice_NeoPixel 
   (uint8_t deviceID, uint16_t n, uint8_t p, uint8_t t) 
      : Adafruit_NeoPixel(n, p, t)
 {id = deviceID;
  lightshowOpsStart = 0;
  lastOpEndTime = 0;
  pixels2 = (uint8_t *)malloc(numBytes);
  memset(pixels2, 0, numBytes);
 }


////////////////////////////////////////////////////////////////////////////////
/// Destructor

LightshowDevice_NeoPixel::~LightshowDevice_NeoPixel() 
 {free(pixels2);
 }


////////////////////////////////////////////////////////////////////////////////
/// initLightshowDevice
///
/// The constructor can be called before setup() runs but there are things we 
/// need to do during setup();
///

void LightshowDevice_NeoPixel::initLightshowDevice ()
 {begin();
  show();
  return;}


////////////////////////////////////////////////////////////////////////////////
/// startLightshow

void LightshowDevice_NeoPixel::startLightshow (const uint8_t* opsStart, 
                                               uint16_t lightshowSize)
 {if (lightShowIsRunning())
   {endLightshow();}
  lightshowOpsStart = opsStart;
  lightshowOpsEnd = opsStart + lightshowSize - 1;
  lightshowOpPtr = opsStart;
  currentOp = 0;
  lastOpEndTime = 0;
  return;}


////////////////////////////////////////////////////////////////////////////////
/// endLightshow

void LightshowDevice_NeoPixel::endLightshow ()
 {lightshowOpsStart = 0;
  clear();
  show();
  if (currentOp)
   {endLightshowOp(currentOp);
    currentOp = 0;}
  return;}


////////////////////////////////////////////////////////////////////////////////
/// updateLightshow
/// 
/// This function is passed the time (in milliseconds) since the start of the
/// lightshow.
/// 

void LightshowDevice_NeoPixel::updateLightshow (unsigned long time)
 {boolean done = false;
  if (lightShowIsRunning())
   {clearBackBuffer();
    if (!currentOp)
     {beginNextLightshowOp(time);}
    if (currentOp)
     {done = updateLightshowOp(this, currentOp, time);
      if (done)
       {endLightshowOp(currentOp);
        currentOp = 0;
        lastOpEndTime = time;}}
    maybeUpdateNeoPixels();}
  return;}


void LightshowDevice_NeoPixel::beginNextLightshowOp (unsigned long time)
 {if (lastOpEndTime)
   {time = lastOpEndTime;}
  currentOp = beginLightshowOp(this, lightshowOpPtr, time);
  if (currentOp)
   {lightshowOpPtr += sizeOfStoredOp(currentOp);
    if (lightshowOpPtr > lightshowOpsEnd)
     {lightshowOpPtr = lightshowOpsStart;}}
  return;}


////////////////////////////////////////////////////////////////////////////////
/// Pixel buffers
/// 
/// We basically use a double-buffered technique for rendering the lightshow 
/// ops.  Multiple lightshow ops can be composed together.  This composition
/// happens in the back buffer (the pixels2 array).  Then, if there are any
/// changed pixels, the buffers are swapped and pushed out to the NeoPixel 
/// strip.
/// 

boolean LightshowDevice_NeoPixel::maybeUpdateNeoPixels ()
 // Returns true iff the display needed to be updated.
 {if (memcmp (pixels2, pixels, numBytes) == 0)
   {return(false);}
  else
   {swapBuffers();
    show();
    return(true);}}


void LightshowDevice_NeoPixel::swapBuffers ()
 {uint8_t *temp = pixels;
  pixels = pixels2;
  pixels2 = temp;
  return;}


void LightshowDevice_NeoPixel::clearBackBuffer ()
 {memset(pixels2, 0, numBytes);
  return;}


void LightshowDevice_NeoPixel::setBBPixelColor(uint16_t n, uint32_t c)
 // Sets a pixel in the back buffer.
 {if (n < numLEDs) 
   {uint8_t r = (uint8_t)(c >> 16);
    uint8_t g = (uint8_t)(c >>  8);
    uint8_t b = (uint8_t)c;
    uint8_t *p = &pixels2[n * 3];
    p[rOffset] = r;
    p[gOffset] = g;
    p[bOffset] = b;}
  return;}


void LightshowDevice_NeoPixel::setBBColor(uint32_t c)
 // Sets the entire back buffer to the specified color.
 {uint8_t r = (uint8_t)(c >> 16);
  uint8_t g = (uint8_t)(c >>  8);
  uint8_t b = (uint8_t)c;
  uint8_t *p = pixels2;
  for (int j = 0; j < numLEDs; j++)
   {p[rOffset] = r;
    p[gOffset] = g;
    p[bOffset] = b;
    p += 3;}
  return;}


uint32_t LightshowDevice_NeoPixel::getBBPixelColor(uint16_t n)
 {if (n >= numLEDs) 
   {// Out of bounds, return no color.
    return(0);}
  uint8_t *p = &pixels2[n * 3];
  uint32_t c = ((uint32_t)p[rOffset] << 16) |
               ((uint32_t)p[gOffset] <<  8) |
                (uint32_t)p[bOffset];
  return(c);}


uint8_t* LightshowDevice_NeoPixel::makePixelBuffer ()
 // When done with it, free this buffer using free().
 {uint8_t* buffer = (uint8_t *)malloc(numBytes);
  return(buffer);}


void LightshowDevice_NeoPixel::clearPixelBuffer (uint8_t* pixelBuffer)
 {memset(pixelBuffer, 0, numBytes);
  return;}
  

uint8_t* LightshowDevice_NeoPixel::swapWithBackBuffer (uint8_t* pixelBuffer)
 {uint8_t *temp = pixels2;
  pixels2 = pixelBuffer;
  return(temp);}


uint32_t LightshowDevice_NeoPixel::getPixelBufferColor(uint8_t* pixelBuffer,
                                                       uint16_t n)
 {if (n >= numLEDs) 
   {// Out of bounds, return no color.
    return(0);}
  uint8_t *p = &pixelBuffer[n * 3];
  uint32_t c = ((uint32_t)p[rOffset] << 16) |
               ((uint32_t)p[gOffset] <<  8) |
                (uint32_t)p[bOffset];
  return(c);}


////////////////////////////////////////////////////////////////////////////////
/// blendWithPixelBuffer
/// 
/// Combines the contents of the back buffer with those of the supplied
/// pixelBuffer.  pixelBuffer acts as the bottommost layer and the results are
/// written to pixelBuffer.  This allows you to use pixelBuffer as a buffer 
/// where results of one or more blend operations are accumulated.
/// 
/// blendMode should be one of the BLEND_* constants.
/// 

void LightshowDevice_NeoPixel::blendWithPixelBuffer (uint8_t* pixelBuffer,
                                                     uint8_t blendMode) 
 {// Each byte is a red, green, or blue component of a pixel's color.
  // Our blend operators can work on these directly so we don't have to
  // recontruct the 32-bit color for each pixel every time we do this.
  float a;
  float b;
  float c;
  for (int j = 0; j < numBytes; j++)
   {a = (float)pixelBuffer[j] / 255.0;
    b = (float)pixels2[j] / 255.0;
    switch(blendMode)
     {case BLEND_MULTIPLY:
        c = blendColorVals_Multiply(a, b);
        break;
      case BLEND_SCREEN:
        c = blendColorVals_Screen(a, b);
        break;
      case BLEND_OVERLAY:
        c = blendColorVals_Overlay(a, b);
        break;
      case BLEND_HARDLIGHT:
        // same as Overlay but with the args reversed
        c = blendColorVals_Overlay(b, a);
        break;
      case BLEND_SOFTLIGHT:
        c = blendColorVals_SoftLight(a, b);
        break;
      case BLEND_REPLACE:
        c = blendColorVals_Replace(a, b);
        break;
      case BLEND_COLOR_DODGE:
        c = blendColorVals_ColorDodge(a, b);
        break;
      case BLEND_COLOR_BURN:
        c = blendColorVals_ColorBurn(a, b);
        break;
      case BLEND_A_MINUS_B:
        c = blendColorVals_Subtract(a, b);
        break;
      case BLEND_B_MINUS_A:
        c = blendColorVals_Subtract(b, a);
        break;
      default:
        c = a;}
    pixelBuffer[j] = floor(c * 255.0);}
  return;}

float LightshowDevice_NeoPixel::blendColorVals_Multiply (float a, float b)
 {return(a * b);}

float LightshowDevice_NeoPixel::blendColorVals_Screen (float a, float b)
 {return(1.0 - (1.0 - a) * (1.0 - b));}

float LightshowDevice_NeoPixel::blendColorVals_Overlay (float a, float b)
 {if (a < 0.5)
   {return(2.0 * a * b);}
  else
   {return(1.0 - (2.0 * (1.0 - a) * (1.0 - b)));}}

float LightshowDevice_NeoPixel::blendColorVals_SoftLight (float a, float b)
 {return((1.0 - (2.0 * b)) * a * a + 2.0 * b * a);}

float LightshowDevice_NeoPixel::blendColorVals_Replace (float a, float b)
 {if (b)
   {return(b);}
  else
   {return(a);}}

float LightshowDevice_NeoPixel::blendColorVals_ColorDodge (float a, float b) 
 {return(a / (1.0 - b));}
  
float LightshowDevice_NeoPixel::blendColorVals_ColorBurn (float a, float b) 
 {return(1.0 - ((1.0 - a) / b));}

float LightshowDevice_NeoPixel::blendColorVals_Subtract (float a, float b) 
 {if (a > b)
   {return(a - b);}
  else
   {return(0);}}


////////////////////////////////////////////////////////////////////////////////
/// reverseBackBuffer

void LightshowDevice_NeoPixel::reverseBackBuffer ()
 {uint8_t *pa = pixels2;                 // first pixel
  uint8_t *pb = pixels2 + numBytes - 3;  // last pixel
  uint8_t tmp[3];
  uint16_t halfLEDs = numLEDs >> 1;
  for (int j=0; j < halfLEDs; j++)
   {memcpy(tmp, pa, 3);
    memcpy(pa, pb, 3);
    memcpy(pb, tmp, 3);
    pa += 3;
    pb -= 3;}
  return;}


////////////////////////////////////////////////////////////////////////////////
/// invertBackBufferColors

void LightshowDevice_NeoPixel::invertBackBufferColors ()
 {for (int j = 0; j < numBytes; j++)
   {pixels2[j] = 255 - pixels2[j];}
  return;}


////////////////////////////////////////////////////////////////////////////////
/// scaleBackBufferColors

void LightshowDevice_NeoPixel::scaleBackBufferColors (float scale)
 {if (scale < 0.0)
   {scale = 0.0;}
  else if (scale > 1.0)
   {scale = 1.0;}
  for (int j = 0; j < numBytes; j++)
   {pixels2[j] = round(scale * pixels2[j]);}
  return;}


////////////////////////////////////////////////////////////////////////////////
/// copyToBackBuffer 
/// 
/// This copies the pixels from one pixel buffer into the back buffer while
/// shifting them right (positive shift numbers) or left (negative shift), 
/// wrapping around as needed.  
///
/// shiftBy is either the number of pixels by which the display should be 
/// shifted or the percentage of the total length by which it should be
/// shifted.
///

void LightshowDevice_NeoPixel::copyToBackBuffer  
         (uint8_t* pixelBuffer, int shiftBy)
 {// Note that there are 3 bytes per pixel, so we're always shifting by some
  // multiple of 3 bytes.
  int shift  = shiftBy * 3;
  if (shift == 0)
   {memcpy(pixels2, pixelBuffer, numBytes);}
  else if (shift > 0)
   {memcpy(pixels2 + shift, pixelBuffer, numBytes - shift);
    memcpy(pixels2, pixelBuffer + numBytes - shift, shift);}
  else
   {memcpy(pixels2, pixelBuffer - shift, numBytes + shift);
    memcpy(pixels2 + numBytes + shift, pixelBuffer, -shift);}
  return;}


void LightshowDevice_NeoPixel::copyToBackBuffer  
         (uint8_t* pixelBuffer, float shiftBy, boolean antialias)
 {if (antialias)
   {float shift = -1.0 * fmod((shiftBy * numLEDs), numLEDs);
    for (int j = 0; j < numLEDs; j++)
     {float k = wrappedPixelPos(j + shift);
      int k0 = floor(k);
      int k1 = floor(wrappedPixelPos(k + 1));
      float w0 = 1.0 - (k - k0);
      float w1 = 1.0 - w0;
      for (int b = 0; b < 3; b++)
       {float c0 = pixelBuffer[(k0 * 3) + b];
        float c1 = pixelBuffer[(k1 * 3) + b];
        pixels2[(j * 3) + b] = round((c0 * w0) + (c1 * w1));}}}
  else
   {copyToBackBuffer(pixelBuffer, (int)(round(shiftBy * numLEDs) % numLEDs));}
  return;}

float LightshowDevice_NeoPixel::wrappedPixelPos (float pos)
 {if (pos < 0)
   {return(numLEDs + pos);}
  else if (pos >= numLEDs)
   {return(pos - numLEDs);}
  else
   {return(pos);}}


////////////////////////////////////////////////////////////////////////////////
/// Colors

uint32_t LightshowDevice_NeoPixel::scaleColorBrightness (uint32_t color,
                                                         float scale)
 {uint8_t r = round(scale * (uint8_t)(color >> 16));
  uint8_t g = round(scale * (uint8_t)(color >>  8));
  uint8_t b = round(scale * (uint8_t)color);
  
  uint32_t newColor = ((uint32_t)r << 16) |
                      ((uint32_t)g <<  8) |
                       (uint32_t)b;
  
  return(newColor);}


uint32_t LightshowDevice_NeoPixel::blendColors_Linear (uint32_t color1, 
                                                       uint32_t color2, 
                                                       float color1Weight)
 {uint8_t r1 = (uint8_t)(color1 >> 16);
  uint8_t g1 = (uint8_t)(color1 >>  8);
  uint8_t b1 = (uint8_t)color1;
  uint8_t r2 = (uint8_t)(color2 >> 16);
  uint8_t g2 = (uint8_t)(color2 >>  8);
  uint8_t b2 = (uint8_t)color2;
  float w0 = color1Weight;
  float w1 = 1.0 - w0;
  uint8_t r = round((r1 * w0) + (r2 * w1));
  uint8_t g = round((g1 * w0) + (g2 * w1));
  uint8_t b = round((b1 * w0) + (b2 * w1));
  uint32_t newColor = ((uint32_t)r << 16) |
                      ((uint32_t)g <<  8) |
                       (uint32_t)b;
  return(newColor);}


ColorHSL* LightshowDevice_NeoPixel::RGBtoHSL (uint32_t rgb, ColorHSL* hsl)
 {float r = (float)((rgb >> 16) & 0xFF) / 255.0;
  float g = (float)((rgb >> 8) & 0xFF) / 255.0;
  float b = (float)(rgb & 0xFF) / 255.0;
  
  float maxVal = max(r, max(g, b));
  float minVal = min(r, min(g, b));
  
  hsl->luminance = (minVal + maxVal) / 2.0;
  
  if (minVal == maxVal)
   {hsl->saturation = 0.0;}
  else if (hsl->luminance <= 0.5)
   {hsl->saturation = (maxVal - minVal) / (maxVal + minVal);}
  else
   {hsl->saturation = (maxVal - minVal) / (2.0 - (maxVal + minVal));}
  
  if (minVal == maxVal)
   {hsl->hue = 0.0;}
  else if ((maxVal == r) && (g >= b))
   {hsl->hue = 60.0 * ((g - b) / (maxVal - minVal));}
  else if (maxVal == r)
   {hsl->hue = 360.0 + (60.0 * ((g - b) / (maxVal - minVal)));}
  else if (maxVal == g)
   {hsl->hue = 120.0 + (60.0 * ((b - r) / (maxVal - minVal)));}
  else
   {hsl->hue = 240.0 + (60.0 * ((r - g) / (maxVal - minVal)));}
  
  return(hsl);}


uint32_t LightshowDevice_NeoPixel::HSLtoRGB (ColorHSL* hsl)
 {float t1, t2, hk, t3r, t3g, t3b;
  
  uint8_t r = 0;
  uint8_t g = 0;
  uint8_t b = 0;
  
  float h = hsl->hue;
  float s = hsl->saturation;
  float l = hsl->luminance;

  if (s > 0.0)
   {if (l < 0.5)
     {t2 = l * (1.0 + s);}
    else
     {t2 = (l + s) - (l * s);}
    t1 = (2.0 * l) - t2;
    hk = h / 360.0;
    t3r = hk + 0.33333333;
    t3g = hk;
    t3b = hk - 0.33333333;
    if (t3r > 1.0)
     {t3r -= 1.0;}
    if (t3b < 0.0)
     {t3b += 1.0;}
    
    r = floor(HSLcolorComponent(t1, t2, t3r) * 255.0);
    g = floor(HSLcolorComponent(t1, t2, t3g) * 255.0);
    b = floor(HSLcolorComponent(t1, t2, t3b) * 255.0);}
  
  uint32_t rgb = ((uint32_t)r << 16) | ((uint32_t)g <<  8) | (uint32_t)b;
  return(rgb);}

float LightshowDevice_NeoPixel::HSLcolorComponent (float t1, float t2, float t3)
 {if (t3 < 0.16666667)
   {return(t1 + ((t2 - t1) * 6.0 * t3));}
  else if ((t3 >= 0.16666667) && (t3 < 0.5))
   {return(t2);}
  else if ((t3 >= 0.5) && (t3 < 0.6666667))
   {return(t1 + ((t2 - t1) * (0.6666667 - t3) * 6.0));}
  else
   {return(t1);}}


////////////////////////////////////////////////////////////////////////////////
/// lightShowIsRunning

boolean LightshowDevice_NeoPixel::lightShowIsRunning ()
 {return(lightshowOpsStart != 0);}


////////////////////////////////////////////////////////////////////////////////
/// End of Code
