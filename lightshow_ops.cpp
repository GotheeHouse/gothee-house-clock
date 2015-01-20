/*
================================================================================
File name: lightshow_ops.cpp
   System: Gothee House Clock
 Platform: Teensy 3.1 Freescale K20 (ARM Cortex M4, MK20DX256VLH7)
   Author: Madeline Usher
  Created: January 7, 2015
  Purpose: Individual lightshow instructions.
================================================================================
  $LastChangedDate: 2015-01-20 01:15:47 -0600 (Tue, 20 Jan 2015) $
  $LastChangedBy: usher $
================================================================================
*/

#include "lightshow_ops.h"


////////////////////////////////////////////////////////////////////////////////
/// Overall lightshow op stuff

/// Note that the first value in a stored op is the opTypeID (one of the LI_* 
/// constants).

LightshowOp* beginLightshowOp (LightshowDevice_NeoPixel* device,
                               const uint8_t* storedOp, 
                               unsigned long startTime)
 {LightshowOp* op = (LightshowOp*)malloc(sizeof(LightshowOp));
  LightshowOpTypeID* pTypeID = (LightshowOpTypeID*)storedOp;
  op->opTypeID = *pTypeID;
  op->storedOp = storedOp;
  
  switch (op->opTypeID)
   {case LI_CONSTANT_COLOR:
      beginOp_ConstantColor(op, startTime);
      return(op);
    case LI_RAINBOW:
      beginOp_Rainbow(op, startTime);
      return(op);
    case LI_SINE:
      beginOp_SpaceWave(device, op, startTime, F_2PI, sineWave);
      return(op);
    case LI_SQUARE:
      beginOp_SpaceWave(device, op, startTime, 1.0, antialiasedSquareWave);
      return(op);
    case LI_TRIANGLE:
      beginOp_SpaceWave(device, op, startTime, 1.0, triangleWave);
      return(op);
    case LI_SAWTOOTH:
      beginOp_SpaceWave(device, op, startTime, 1.0, sawtoothWave);
      return(op);
    case LI_GRADIENT:
      beginOp_Gradient(op, startTime);
      return(op);
    case LI_NOTCH:
      beginOp_Notch(op, startTime);
      return(op);
    case LI_SINE_FLASH:
      beginOp_Flash(op, startTime, F_2PI, sineWave);
      return(op);
    case LI_SQUARE_FLASH:
      beginOp_Flash(op, startTime, 1.0, squareWave);
      return(op);
    case LI_TRIANGLE_FLASH:
      beginOp_Flash(op, startTime, 1.0, triangleWave);
      return(op);
    case LI_SAWTOOTH_FLASH:
      beginOp_Flash(op, startTime, 1.0, sawtoothWave);
      return(op);
    case LI_CYLON_EYE:
      beginOp_CylonEye(op, startTime);
      return(op);
    case LI_THEATER_CHASE:
      beginOp_TheaterChase(op, startTime);
      return(op);
    case LI_FLIP:
    case LI_NEGATIVE:
      beginOp_SecondOrderOp(device, op, startTime, false);
      return(op);
    case LI_SHIFT:
      beginOp_Shift(device, op, startTime);
      return(op);
    case LI_FADE:
      beginOp_Fade(device, op, startTime);
      return(op);
    case LI_MULTIPLY:
    case LI_SCREEN:
    case LI_OVERLAY:
    case LI_HARDLIGHT:
    case LI_SOFTLIGHT:
    case LI_REPLACE:
    case LI_COLOR_DODGE:
    case LI_COLOR_BURN:
    case LI_A_MINUS_B:
    case LI_B_MINUS_A:
      beginOp_SecondOrderOp(device, op, startTime, true);
      return(op);
    default:
      free(op);
      return(0);}}



boolean updateLightshowOp (LightshowDevice_NeoPixel* device,
                           LightshowOp* op, 
                           unsigned long time)
 {boolean done;
  switch (op->opTypeID)
   {case LI_CONSTANT_COLOR:
      done = updateOp_ConstantColor(device, op, time);
      break;
    case LI_RAINBOW:
      done = updateOp_Rainbow(device, op, time);
      break;
    case LI_SINE:
    case LI_SQUARE:
    case LI_TRIANGLE:
    case LI_SAWTOOTH:
      done = updateOp_SpaceWave(device, op, time);
      break;
    case LI_GRADIENT:
      done = updateOp_Gradient(device, op, time);
      break;
    case LI_NOTCH:
      done = updateOp_Notch(device, op, time);
      break;
    case LI_SINE_FLASH:
    case LI_SQUARE_FLASH:
    case LI_TRIANGLE_FLASH:
    case LI_SAWTOOTH_FLASH:
      done = updateOp_Flash(device, op, time);
      break;
    case LI_CYLON_EYE:
      done = updateOp_CylonEye(device, op, time);
      break;
    case LI_THEATER_CHASE:
      done = updateOp_TheaterChase(device, op, time);
      break;
    case LI_FLIP:
      done = updateOp_Flip(device, op, time);
      break;
    case LI_NEGATIVE:
      done = updateOp_Negative(device, op, time);
      break;
    case LI_SHIFT:
      done = updateOp_Shift(device, op, time);
      break;
    case LI_FADE:
      done = updateOp_Fade(device, op, time);
      break;
    case LI_MULTIPLY:
      done = updateOp_Blend(device, op, time, BLEND_MULTIPLY);
      break;
    case LI_SCREEN:
      done = updateOp_Blend(device, op, time, BLEND_SCREEN);
      break;
    case LI_OVERLAY:
      done = updateOp_Blend(device, op, time, BLEND_OVERLAY);
      break;
    case LI_HARDLIGHT:
      done = updateOp_Blend(device, op, time, BLEND_HARDLIGHT);
      break;
    case LI_SOFTLIGHT:
      done = updateOp_Blend(device, op, time,  BLEND_SOFTLIGHT);
      break;
    case LI_REPLACE:
      done = updateOp_Blend(device, op, time,  BLEND_REPLACE);
      break;
    case LI_COLOR_DODGE:
      done = updateOp_Blend(device, op, time,  BLEND_COLOR_DODGE);
      break;
    case LI_COLOR_BURN:
      done = updateOp_Blend(device, op, time,  BLEND_COLOR_BURN);
      break;
    case LI_A_MINUS_B:
      done = updateOp_Blend(device, op, time,  BLEND_A_MINUS_B);
      break;
    case LI_B_MINUS_A:
      done = updateOp_Blend(device, op, time,  BLEND_B_MINUS_A);
      break;
   }
  return(done);}



void endLightshowOp (LightshowOp* op)
 {switch (op->opTypeID)
   {case LI_FLIP:
    case LI_NEGATIVE:
    case LI_SHIFT:
    case LI_FADE:
    case LI_MULTIPLY:
    case LI_SCREEN:
    case LI_OVERLAY:
    case LI_HARDLIGHT:
    case LI_SOFTLIGHT:
    case LI_REPLACE:
    case LI_COLOR_DODGE:
    case LI_COLOR_BURN:
    case LI_A_MINUS_B:
    case LI_B_MINUS_A:
      endOp_SecondOrderOp(op);
      break;
   }
  free(op->opData);
  free(op);
  return;}


size_t sizeOfStoredOp (LightshowOp* op)
 {switch (op->opTypeID)
   {case LI_CONSTANT_COLOR:
      return(sizeof(StoredOp_ConstantColor));
    case LI_RAINBOW:
      return(sizeof(StoredOp_Rainbow));
    case LI_SINE:
    case LI_SQUARE:
    case LI_TRIANGLE:
    case LI_SAWTOOTH:
      return(sizeof(StoredOp_SpaceWave));
    case LI_GRADIENT:
      return(sizeof(StoredOp_Gradient));
    case LI_NOTCH:
      return(sizeof(StoredOp_Notch));
    case LI_SINE_FLASH:
    case LI_SQUARE_FLASH:
    case LI_TRIANGLE_FLASH:
    case LI_SAWTOOTH_FLASH:
      return(sizeof(StoredOp_Flash));
    case LI_CYLON_EYE:
      return(sizeof(StoredOp_CylonEye));
    case LI_THEATER_CHASE:
      return(sizeof(StoredOp_TheaterChase));
    case LI_FLIP:
    case LI_NEGATIVE:
    case LI_MULTIPLY:
    case LI_SCREEN:
    case LI_OVERLAY:
    case LI_HARDLIGHT:
    case LI_SOFTLIGHT:
    case LI_REPLACE:
    case LI_COLOR_DODGE:
    case LI_COLOR_BURN:
    case LI_A_MINUS_B:
    case LI_B_MINUS_A:
      return(sizeOfOp_SecondOrderOp(op));
    case LI_SHIFT:
      return(sizeof(StoredOp_Shift) + sizeOfChildOps(op));
    case LI_FADE:
      return(sizeof(StoredOp_Fade) + sizeOfChildOps(op));
    default:
      return(0);}}
  

////////////////////////////////////////////////////////////////////////////////
/// LI_CONSTANT_COLOR

void beginOp_ConstantColor (LightshowOp* op, unsigned long startTime)
 {StoredOp_ConstantColor* storedOp = (StoredOp_ConstantColor*)op->storedOp;
  LiveOpData_ConstantColor* opData = 
    (LiveOpData_ConstantColor*)malloc(sizeof(LiveOpData_ConstantColor));
  op->opData = opData;
  opData->endTime = startTime + storedOp->duration;
  return;}


boolean updateOp_ConstantColor (LightshowDevice_NeoPixel* device,
                                LightshowOp* op, 
                                unsigned long time)
 {StoredOp_ConstantColor* storedOp = (StoredOp_ConstantColor*)op->storedOp;
  LiveOpData_ConstantColor* opData = (LiveOpData_ConstantColor*)op->opData;
  for (int j=0; j < device->numPixels(); j++)
   {device->setBBPixelColor(j, storedOp->color);}
  return(time >= opData->endTime);}


////////////////////////////////////////////////////////////////////////////////
/// LI_RAINBOW

void beginOp_Rainbow (LightshowOp* op, unsigned long startTime)
 {StoredOp_Rainbow* storedOp = (StoredOp_Rainbow*)op->storedOp;
  LiveOpData_Rainbow* opData = 
    (LiveOpData_Rainbow*)malloc(sizeof(LiveOpData_Rainbow));
  op->opData = opData;
  opData->endTime = startTime + storedOp->duration;
  return;}


boolean updateOp_Rainbow (LightshowDevice_NeoPixel* device,
                          LightshowOp* op, 
                          unsigned long time)
 {StoredOp_Rainbow* storedOp = (StoredOp_Rainbow*)op->storedOp;
  LiveOpData_Rainbow* opData = (LiveOpData_Rainbow*)op->opData;
  int n = device->numPixels();
  ColorHSL hsl;
  hsl.saturation = 1.0;
  hsl.luminance = (float)storedOp->brightness / 255.0;
  for (int j=0; j < n; j++)
   {hsl.hue = ((float)j / (float)(n - 1)) * storedOp->repetitions * 360.0;
    if (hsl.hue > 360.0)
     {hsl.hue = fmod(hsl.hue, 360.0);}
    device->setBBPixelColor(j, device->HSLtoRGB(&hsl));}
  return(time >= opData->endTime);}


////////////////////////////////////////////////////////////////////////////////
/// Repeating patterns through the space of the pixel strip

void beginOp_SpaceWave (LightshowDevice_NeoPixel* device,
                        LightshowOp* op, 
                        unsigned long startTime,
                        float cycleLength,
                        float (*waveFn)  (float x, float frequency))
 {StoredOp_SpaceWave* storedOp = (StoredOp_SpaceWave*)op->storedOp;
  op->opData = malloc(sizeof(LiveOpData_SpaceWave));
  LiveOpData_SpaceWave* opData = (LiveOpData_SpaceWave*)op->opData;
  float n = device->numPixels();
  opData->cycleLength = cycleLength;
  opData->waveFn = waveFn;
  opData->frequency = (cycleLength * storedOp->repetitions) / n;
  opData->amplitude = (float)storedOp->amplitude / 510.0;
  opData->phase = (cycleLength * storedOp->phase / 100.0);
  opData->endTime = startTime + storedOp->duration;
  return;}


boolean updateOp_SpaceWave (LightshowDevice_NeoPixel* device,
                            LightshowOp* op, 
                            unsigned long time)
 {StoredOp_SpaceWave* storedOp = (StoredOp_SpaceWave*)op->storedOp;
  LiveOpData_SpaceWave* opData = (LiveOpData_SpaceWave*)op->opData;
  
  uint32_t color;
  float x, y, scale;
  int n = device->numPixels();
  float f = opData->frequency;
  
  for (int j=0; j < n; j++)
   {x = fmod(((f * j) + opData->phase), opData->cycleLength);
    y = (*opData->waveFn)(x, f);
    scale = 0.5 + opData->amplitude * y;
    scale = min(scale, 1.0);
    scale = max(0.0, scale);
    color = device->scaleColorBrightness(storedOp->color, scale);
    device->setBBPixelColor(j, color);}
  
  return(time >= opData->endTime);}



float sineWave (float x, float frequency)
 {return(sin(x));}


float squareWave (float x, float f)
 {// The wave's low value is -1, high is +1.
  if (x < 0.5)
  {return(1.0);}
  else 
   {return(-1.0);}}


float antialiasedSquareWave (float x, float f)
 {// This computes an anti-aliased square wave, so the repititions look more
  // regular even if the length of the LED pixel strip is not an integer
  // multiple of the period.
  //
  // Note that the LED spacing expressed as a percentage of the cycle length,
  // is the same value as the frequency, f in this case.
  //
  // The square wave's low value is -1, high is +1.
  //
  float y;
  if (x < f)
   {// Need to anti-alias.  The previous pixel was OFF.
    y = x / f;}
  else if (x <= 0.5 - f)
   {y = 1.0;}
  else if (x < 0.5 + f)
   {// Need to anti-alias.
    y = (0.5 - x) / f;}
  else if (x <= 1.0 - f)
   {y = -1.0;}
  else
   {// Need to anti-alias.  The next pixel will be ON.
    y = (x - 1.0) / f;}
  return(y);}


float triangleWave (float x, float frequency)
 {// The triangle wave's low value is -1, high is +1.
  float y;
  if (x < 0.5)
   {// Ramping up:
    y = (4.0 * x) - 1.0;}
  else 
   {// Ramping down:
    y = (-4.0 * x) + 3.0;}
  return(y);}


float sawtoothWave (float x, float frequency)
 {// The sawtooth wave's low value is -1, high is +1.
  return((2.0 * x) - 1.0);}


////////////////////////////////////////////////////////////////////////////////
/// LI_GRADIENT

void beginOp_Gradient (LightshowOp* op, 
                       unsigned long startTime)
 {StoredOp_Gradient* storedOp = (StoredOp_Gradient*)op->storedOp;
  op->opData = malloc(sizeof(LiveOpData_Gradient));
  LiveOpData_Gradient* opData = (LiveOpData_Gradient*)op->opData;
  opData->c0 = storedOp->c0 / 100.0;
  opData->c1 = storedOp->c1 / 100.0;
  opData->c2 = storedOp->c2 / 100.0;
  opData->m1 = -0.5 / (opData->c1 - opData->c0);
  opData->b1 = 1.0 - (opData->m1 * opData->c0);
  opData->m2 = -0.5 / (opData->c2 - opData->c1);
  opData->b2 = -opData->m2 * opData->c2;
  opData->endTime = startTime + storedOp->duration;
  return;}


boolean updateOp_Gradient (LightshowDevice_NeoPixel* device,
                           LightshowOp* op, 
                           unsigned long time)
 {StoredOp_Gradient* storedOp = (StoredOp_Gradient*)op->storedOp;
  LiveOpData_Gradient* opData = (LiveOpData_Gradient*)op->opData;
  
  int n = device->numPixels();
  uint32_t col1 = storedOp->color1;
  uint32_t col2 = storedOp->color2;
  float w0;
  for (int j=0; j < n; j++)
   {float x = fmod(((float)j / (float)n) * storedOp->repetitions, 1.0);
    if (x <= opData->c0)
     {w0 = 1.0;}
    else if (x <= opData->c1)
     {w0 = opData->m1 * x + opData->b1;}
    else if (x <= opData->c2)
     {w0 = opData->m2 * x + opData->b2;}
    else
     {w0 = 0.0;}
    device->setBBPixelColor(j, device->blendColors_Linear(col1, col2, w0));}
  return(time >= opData->endTime);}


////////////////////////////////////////////////////////////////////////////////
/// LI_NOTCH

void beginOp_Notch (LightshowOp* op, 
                    unsigned long startTime)
 {StoredOp_Notch* storedOp = (StoredOp_Notch*)op->storedOp;
  op->opData = malloc(sizeof(LiveOpData_Notch));
  LiveOpData_Notch* opData = (LiveOpData_Notch*)op->opData;
  opData->c0 = storedOp->color2Start / 100.0;
  opData->c1 = storedOp->color2End / 100.0;
  opData->endTime = startTime + storedOp->duration;
  return;}


boolean updateOp_Notch (LightshowDevice_NeoPixel* device,
                        LightshowOp* op, 
                        unsigned long time)
 {StoredOp_Notch* storedOp = (StoredOp_Notch*)op->storedOp;
  LiveOpData_Notch* opData = (LiveOpData_Notch*)op->opData;
  
  float n = device->numPixels();
  uint32_t col1 = storedOp->color1;
  uint32_t col2 = storedOp->color2;
  for (int j=0; j < device->numPixels(); j++)
   {float x = j / n;
    if (x < opData->c0)
     {device->setBBPixelColor(j, col1);}
    else if (x < opData->c1)
     {device->setBBPixelColor(j, col2);}
    else
     {device->setBBPixelColor(j, col1);}}
  return(time >= opData->endTime);}


////////////////////////////////////////////////////////////////////////////////
/// Pulsing lights
///
/// Most of these flash operators work with periodic functions like sines,
/// square waves, etc. and share a lot of common code.
///

void beginOp_Flash (LightshowOp* op, 
                    unsigned long startTime,
                    float cycleLength,
                    float (*waveFn)  (float x, float frequency))
 {StoredOp_Flash* storedOp = (StoredOp_Flash*)op->storedOp;
  op->opData = malloc(sizeof(LiveOpData_Flash));
  LiveOpData_Flash* opData = (LiveOpData_Flash*)op->opData;
  
  opData->cycleLength = cycleLength;
  opData->waveFn = waveFn;
  opData->frequency = (cycleLength * 1000.0) / storedOp->period;
  opData->amplitude = (float)storedOp->amplitude / 510.0;
  opData->phase = (cycleLength * storedOp->phase / 100.0);
  opData->startTime = startTime;
  opData->endTime = startTime + storedOp->duration;
  return;}


boolean updateOp_Flash (LightshowDevice_NeoPixel* device,
                        LightshowOp* op, 
                        unsigned long time)
 {StoredOp_Flash* storedOp = (StoredOp_Flash*)op->storedOp;
  LiveOpData_Flash* opData = (LiveOpData_Flash*)op->opData;
  
  float f = opData->frequency;
  float elapsedSeconds = (float)(time - opData->startTime) / 1000.0;
  float x = fmod(((f * elapsedSeconds) + opData->phase), opData->cycleLength);
  float y = (*opData->waveFn)(x, f);
  float scale = 0.5 + opData->amplitude * y;
  scale = min(scale, 1.0);
  scale = max(0.0, scale);
  uint32_t color = device->scaleColorBrightness(storedOp->color, scale);
  device->setBBColor(color);
  return(time >= opData->endTime);}


////////////////////////////////////////////////////////////////////////////////
/// LI_CYLON_EYE

void beginOp_CylonEye (LightshowOp* op, unsigned long startTime)
 {StoredOp_CylonEye* storedOp = 
    (StoredOp_CylonEye*)op->storedOp;
  LiveOpData_CylonEye* opData = 
    (LiveOpData_CylonEye*)malloc(sizeof(LiveOpData_CylonEye));
  
  op->opData = opData;
  opData->startTime = startTime;
  opData->endTime = startTime + storedOp->duration;
  return;}
  
  
boolean updateOp_CylonEye (LightshowDevice_NeoPixel* device,
                           LightshowOp* op, 
                           unsigned long time)
 {StoredOp_CylonEye* storedOp = (StoredOp_CylonEye*)op->storedOp;
  LiveOpData_CylonEye* opData = (LiveOpData_CylonEye*)op->opData;
    
  uint8_t dir = FORWARD;
  uint16_t n = device->numPixels();
  unsigned long period = storedOp->period;
  uint32_t color = storedOp->color;
  uint16_t tailLength = storedOp->tailLength;
  uint16_t tailPos;
  uint32_t tailColor = color;
  float scale;
  boolean fadeTail = storedOp->fadeTail;
  uint16_t headPos = round(((n - 1) << 1) * 
                           (float)((time - opData->startTime) % period) / 
                           (float)period);
  if (headPos >= n)
   {headPos = n - (headPos - n) - 2;
    dir = BACKWARD;}
  
  device->setBBPixelColor(headPos, color);
  for (int j = 1; j < tailLength; j++)
   {if (fadeTail)
     {scale = (float)(tailLength - j) / (float)tailLength;
      tailColor = device->scaleColorBrightness (color, scale * scale);}

    if (dir == FORWARD)
     {tailPos = headPos - j;
      if (j <= headPos)
       {device->setBBPixelColor(tailPos, tailColor);}}
    else
     {tailPos = headPos + j;
      if (tailPos < n)
       {device->setBBPixelColor(tailPos, tailColor);}}}
  
  return(time >= opData->endTime);}


////////////////////////////////////////////////////////////////////////////////
/// LI_THEATER_CHASE

void beginOp_TheaterChase (LightshowOp* op, unsigned long startTime)
 {StoredOp_TheaterChase* storedOp = (StoredOp_TheaterChase*)op->storedOp;
  op->opData = malloc(sizeof(LiveOpData_TheaterChase));
  LiveOpData_TheaterChase* opData = (LiveOpData_TheaterChase*)op->opData;
  opData->startTime = startTime;
  opData->endTime = startTime + storedOp->duration;
  return;}


boolean updateOp_TheaterChase (LightshowDevice_NeoPixel* device,
                           LightshowOp* op, 
                           unsigned long time)
 {StoredOp_TheaterChase* storedOp = (StoredOp_TheaterChase*)op->storedOp;
  LiveOpData_TheaterChase* opData = (LiveOpData_TheaterChase*)op->opData;
  uint16_t n = device->numPixels();
  uint16_t nPatternLEDs = storedOp->patternSize * storedOp->dotSize;
  
  float cyclePercent = (float)((time - opData->startTime) % 
                                storedOp->period) / (float)storedOp->period;
  int shiftBy = round(cyclePercent * nPatternLEDs);
  if (storedOp->shiftDir == SHIFT_DIR_LEFT)
   {shiftBy *= -1;}
  int pos = shiftBy;
  if (pos > 0)
   {pos -= nPatternLEDs;}
  while (pos < n)
   {for (int j = 0; j < storedOp->dotSize; j++)
     {if (pos >= 0)
       {device->setBBPixelColor(pos, storedOp->color1);}
      pos++;}
    for (int k = storedOp->dotSize; k < nPatternLEDs; k++)
     {if (pos >= 0)
       {device->setBBPixelColor(pos, storedOp->color2);}
      pos++;}}
  
  return(time >= opData->endTime);}


////////////////////////////////////////////////////////////////////////////////
/// Secord order operators (operators that work on one or more child operations)

void beginOp_SecondOrderOp (LightshowDevice_NeoPixel* device,
                            LightshowOp* op, 
                            unsigned long startTime,
                            boolean usesOwnPixelBuffer)
 {StoredOp_SecondOrderOp* storedOp = (StoredOp_SecondOrderOp*)op->storedOp;
  op->opData = malloc(sizeof(LiveOpData_SecondOrderOp));
  initSecondOrderOp(device, op, startTime, usesOwnPixelBuffer,
                    storedOp->numChildOps, sizeof(StoredOp_SecondOrderOp));
  return;}

void initSecondOrderOp (LightshowDevice_NeoPixel* device,
                        LightshowOp* op, 
                        unsigned long startTime,
                        boolean usesOwnPixelBuffer,
                        uint8_t numChildOps,
                        size_t sizeofStoredOp)
 {LiveOpData_SecondOrderOp* opData = (LiveOpData_SecondOrderOp*)op->opData;
  beginChildOps(device, startTime, opData, numChildOps, 
                op->storedOp, sizeofStoredOp);
  if (usesOwnPixelBuffer)
   {opData->pixels = device->makePixelBuffer();}
  else
   {opData->pixels = 0;}
  return;}

void beginChildOps (LightshowDevice_NeoPixel* device,
                    unsigned long startTime,
                    LiveOpData_SecondOrderOp* opData,
                    uint8_t numChildOps,
                    const uint8_t* storedOpStart,
                    size_t sizeofStoredOp)
 {const uint8_t* sPtr = (const uint8_t*)storedOpStart + sizeofStoredOp;
  opData->numChildOps = numChildOps;
  opData->childOps = (LightshowOp**)malloc(sizeof(LightshowOp*) * numChildOps);
  for (int j = 0; j < numChildOps; j++)
   {opData->childOps[j] = beginLightshowOp(device, sPtr, startTime);
    sPtr += sizeOfStoredOp(opData->childOps[j]);}
  return;}



void endOp_SecondOrderOp (LightshowOp* op)
 {LiveOpData_SecondOrderOp* opData = (LiveOpData_SecondOrderOp*)op->opData;
  uint8_t numChildOps = opData->numChildOps;
  for (int j = 0; j < numChildOps; j++)
   {endLightshowOp(opData->childOps[j]);}
  free(opData->childOps);
  if (opData->pixels)
   {free(opData->pixels);}
  return;}



size_t sizeOfOp_SecondOrderOp (LightshowOp* op)
 {return(sizeof(StoredOp_SecondOrderOp) + sizeOfChildOps(op));}

size_t sizeOfChildOps (LightshowOp* op)
 {LiveOpData_SecondOrderOp* opData = (LiveOpData_SecondOrderOp*)op->opData;
  uint8_t numChildOps = opData->numChildOps;
  size_t bytes = 0;
  for (int j = 0; j < numChildOps; j++)
   {bytes += sizeOfStoredOp(opData->childOps[j]);}
  return(bytes);}



boolean updateOp_Blend (LightshowDevice_NeoPixel* device,
                        LightshowOp* op, 
                        unsigned long time,
                        uint8_t blendMode)
 {LiveOpData_SecondOrderOp* opData = (LiveOpData_SecondOrderOp*)op->opData;
  uint8_t numChildOps = opData->numChildOps;
  if (numChildOps == 0)
   {return(true);}
  
  // The first op forms the background layer.  It's built on the usual back
  // buffer.
  boolean done = updateLightshowOp(device, opData->childOps[0], time);
  
  // Then we swap in our own buffer for subsequent ops.  opData->pixels
  // now holds the background layer.
  opData->pixels = device->swapWithBackBuffer(opData->pixels);
  
  for (int j = 1; j < numChildOps; j++)
   {device->clearBackBuffer();
    done = updateLightshowOp(device, opData->childOps[j], time) || done;
    // Combine the pixels written to the back buffer with those in our own
    // buffer.  Results are accumulated in our own buffer.
    device->blendWithPixelBuffer(opData->pixels, blendMode);}
  
  // Then swap the buffers back:
  opData->pixels = device->swapWithBackBuffer(opData->pixels);
  
  // Note that if any of the child ops are done, this op is considered to be
  // done.
  return(done);}


////////////////////////////////////////////////////////////////////////////////
/// LI_FLIP

boolean updateOp_Flip (LightshowDevice_NeoPixel* device,
                       LightshowOp* op, 
                       unsigned long time)
 {LiveOpData_SecondOrderOp* opData = (LiveOpData_SecondOrderOp*)op->opData;
  LightshowOp* childOp = opData->childOps[0];
  boolean done = updateLightshowOp(device, childOp, time);
  device->reverseBackBuffer();
  return(done);}


////////////////////////////////////////////////////////////////////////////////
/// LI_NEGATIVE

boolean updateOp_Negative (LightshowDevice_NeoPixel* device,
                           LightshowOp* op, 
                           unsigned long time)
 {LiveOpData_SecondOrderOp* opData = (LiveOpData_SecondOrderOp*)op->opData;
  LightshowOp* childOp = opData->childOps[0];
  boolean done = updateLightshowOp(device, childOp, time);
  device->invertBackBufferColors();
  return(done);}


////////////////////////////////////////////////////////////////////////////////
/// LI_SHIFT

void beginOp_Shift (LightshowDevice_NeoPixel* device,
                    LightshowOp* op, 
                    unsigned long startTime)
 {StoredOp_Shift* storedOp = (StoredOp_Shift*)op->storedOp;
  op->opData = malloc(sizeof(LiveOpData_Shift));
  LiveOpData_Shift* opData = (LiveOpData_Shift*)op->opData;
  initSecondOrderOp(device, op, startTime, true,
                    storedOp->numChildOps, sizeof(StoredOp_Shift));
  opData->startTime = startTime;
  return;}


boolean updateOp_Shift (LightshowDevice_NeoPixel* device,
                        LightshowOp* op, 
                        unsigned long time)
 {StoredOp_Shift* storedOp = (StoredOp_Shift*)op->storedOp;
  LiveOpData_Shift* opData = (LiveOpData_Shift*)op->opData;
  LightshowOp* childOp = opData->childOps[0];
  
  float shiftBy = (float)((time - opData->startTime) % storedOp->period) / 
                  (float)storedOp->period;
  if (storedOp->shiftDir == SHIFT_DIR_LEFT)
   {shiftBy *= -1;}
  
  // Swap in our own buffer for the child op:
  opData->pixels = device->swapWithBackBuffer(opData->pixels);
  boolean done = updateLightshowOp(device, childOp, time);
  // Swap our buffer back:
  opData->pixels = device->swapWithBackBuffer(opData->pixels);
  // The copy and shift it into the back buffer:
  device->copyToBackBuffer(opData->pixels, shiftBy, true);
  
  return(done);}


////////////////////////////////////////////////////////////////////////////////
/// LI_FADE

void beginOp_Fade (LightshowDevice_NeoPixel* device,
                   LightshowOp* op, 
                   unsigned long startTime)
 {StoredOp_Fade* storedOp = (StoredOp_Fade*)op->storedOp;
  op->opData = malloc(sizeof(LiveOpData_Fade));
  LiveOpData_Fade* opData = (LiveOpData_Fade*)op->opData;
  initSecondOrderOp(device, op, startTime, false,
                    storedOp->numChildOps, sizeof(StoredOp_Fade));
  opData->startTime = startTime;
  opData->fadeInEndTime = startTime + storedOp->fadeInDuration;
  opData->endTime = startTime + storedOp->duration;
  opData->fadeOutStartTime = opData->endTime - storedOp->fadeOutDuration;
  return;}


boolean updateOp_Fade (LightshowDevice_NeoPixel* device,
                       LightshowOp* op, 
                       unsigned long time)
 {StoredOp_Fade* storedOp = (StoredOp_Fade*)op->storedOp;
  LiveOpData_Fade* opData = (LiveOpData_Fade*)op->opData;
  LightshowOp* childOp = opData->childOps[0];
  float scale;
  
  boolean done = updateLightshowOp(device, childOp, time) ||
                 (time >= opData->endTime);
  if (time < opData->fadeInEndTime)
   {scale = (float)(time - opData->startTime) /  
            (float)storedOp->fadeInDuration;
    device->scaleBackBufferColors(scale);}
  else if (time > opData->fadeOutStartTime)
   {scale = 1.0 - (float)(time - opData->fadeOutStartTime) /  
                  (float)storedOp->fadeOutDuration;
    device->scaleBackBufferColors(scale);}
  return(done);}


////////////////////////////////////////////////////////////////////////////////
/// End of Code

