/*
================================================================================
File name: lightshow_ops.h
   System: Gothee House Clock
 Platform: Teensy 3.1 Freescale K20 (ARM Cortex M4, MK20DX256VLH7)
   Author: Madeline Usher
  Created: January 7, 2015
  Purpose: Individual lightshow instructions.
================================================================================

There are two parts to each lightshow instruction:

(1) The instruction that gets stored in flash memory describing a lightshow.
    I call this a stored lightshow op.
    This cannot be written to in real-time, so we also have ...

(2) The data needed for a lightshow instruction that's currently being executed.
    I call this the live lightshow op data.
    
================================================================================
  $LastChangedDate: 2015-01-18 18:38:21 -0600 (Sun, 18 Jan 2015) $
  $LastChangedBy: usher $
================================================================================
*/

#ifndef LIGHTSHOW_OPS_h
#define LIGHTSHOW_OPS_h

#include <inttypes.h>
#include "time.h"
#include "lightshow_basic-defs.h"


////////////////////////////////////////////////////////////////////////////////

#define F_PI_2 1.5707964
#define F_PI   3.1415927
#define F_2PI  6.2831855

#define BACKWARD  0
#define FORWARD   1


////////////////////////////////////////////////////////////////////////////////
/// Overall lightshow op stuff

LightshowOp* beginLightshowOp (LightshowDevice_NeoPixel* device,
                               const uint8_t* storedOp, 
                               unsigned long startTime);

// Returns true iff the instruction is finished:
boolean updateLightshowOp (LightshowDevice_NeoPixel* device,
                           LightshowOp* op, 
                           unsigned long time);

void endLightshowOp (LightshowOp* op);

size_t sizeOfStoredOp (LightshowOp* op);


////////////////////////////////////////////////////////////////////////////////
/// LI_CONSTANT_COLOR
 
#define LI_CONSTANT_COLOR 0x0010

typedef struct __attribute__((__packed__)) StoredOp_ConstantColor
 {LightshowOpTypeID opTypeID;
  Color color;
  unsigned long duration;}  // milliseconds
 StoredLightshowOp_ConstantColor;
 
typedef struct LiveOpData_ConstantColor
 {unsigned long endTime;}  // milliseconds since start of current lightshow
 LiveOpData_ConstantColor;


void beginOp_ConstantColor (LightshowOp* op, unsigned long startTime);

boolean updateOp_ConstantColor (LightshowDevice_NeoPixel* device,
                                LightshowOp* op, 
                                    unsigned long time);


////////////////////////////////////////////////////////////////////////////////
/// LI_RAINBOW

#define LI_RAINBOW 0x0011

typedef struct __attribute__((__packed__)) StoredOp_Rainbow
 {LightshowOpTypeID opTypeID;
  uint8_t brightness;
  uint8_t repetitions;  // # of rainbows
  unsigned long duration;}  // milliseconds
 StoredOp_Rainbow;

typedef struct LiveOpData_Rainbow
 {unsigned long endTime;}  // milliseconds since start of current lightshow
 LiveOpData_Rainbow;


void beginOp_Rainbow (LightshowOp* op, unsigned long startTime);

boolean updateOp_Rainbow (LightshowDevice_NeoPixel* device,
                          LightshowOp* op, 
                          unsigned long time);


////////////////////////////////////////////////////////////////////////////////
/// Repeating patterns through the space of the pixel strip

#define LI_SINE      0x0012
#define LI_SQUARE    0x0013
#define LI_TRIANGLE  0x0014
#define LI_SAWTOOTH  0x0015


typedef struct __attribute__((__packed__)) StoredOp_SpaceWave
 {LightshowOpTypeID opTypeID;
  Color color;
  uint8_t repetitions;  // # of full sine waves across the display
  uint8_t amplitude;    // 0 -> 255
  uint8_t phase;        // 0 -> 100, which translates to 0.0 -> 1.0 cycles
  unsigned long duration;}  // milliseconds
 StoredOp_SpaceWave;

typedef struct LiveOpData_SpaceWave
 {unsigned long endTime;  // milliseconds since start of current lightshow
  float cycleLength;
  float (*waveFn)  (float x, float frequency);
  float frequency;
  float amplitude;
  float phase;}
 LiveOpData_SpaceWave;


void beginOp_SpaceWave (LightshowDevice_NeoPixel* device,
                        LightshowOp* op, 
                        unsigned long startTime,
                        float cycleLength,
                        float (*waveFn) (float x, float frequency));

boolean updateOp_SpaceWave (LightshowDevice_NeoPixel* device,
                            LightshowOp* op, 
                            unsigned long time);


float sineWave (float x, float frequency);
float squareWave (float x, float frequency);
float antialiasedSquareWave (float x, float frequency);
float triangleWave (float x, float frequency);
float sawtoothWave (float x, float frequency);


////////////////////////////////////////////////////////////////////////////////
/// LI_GRADIENT
///
/// c0, c1, and c2 are control points and are specified as a percentage of the
/// length of the gradient:
/// - c0 is where color1 starts to fade and color2 starts to contribute.
/// - c1 is where the two colors have equal contributions.
/// - c2 is where color1 is completely off and all is color2.
///

#define LI_GRADIENT 0x0020

typedef struct __attribute__((__packed__)) StoredOp_Gradient
 {LightshowOpTypeID opTypeID;
  Color color1;
  Color color2;
  uint8_t repetitions;
  uint8_t c0;         // control point,  0 -> 100
  uint8_t c1;         // control point,  0 -> 100
  uint8_t c2;         // control point,  0 -> 100
  unsigned long duration;}  // milliseconds
 StoredOp_Gradient;

typedef struct LiveOpData_Gradient
 {unsigned long endTime;  // milliseconds since start of current lightshow
  float c0;   // control point #0, expressed as number from 0.0 -> 1.0
  float c1;   // control point #1, expressed as number from 0.0 -> 1.0
  float c2;   // control point #2, expressed as number from 0.0 -> 1.0
  float m1;   // slope of first part of weighting line
  float b1;   // y-intercept of first part of weighting line
  float m2;   // slope of second part of weighting line
  float b2;}  // y-intercept of second part of weighting line
 LiveOpData_Gradient;


void beginOp_Gradient (LightshowOp* op, 
                       unsigned long startTime);

boolean updateOp_Gradient (LightshowDevice_NeoPixel* device,
                           LightshowOp* op, 
                           unsigned long time);


////////////////////////////////////////////////////////////////////////////////
/// LI_NOTCH
///
/// This makes a pattern that is Color1 from the start of the LED strip, then
/// Color2 for a bit, and then back to Color1.  It's often used as a notch 
/// filter to be combined with other lightshow ops, so that you can have the
/// presence or absence of a color at one particular spot of the LED stip.
///

#define LI_NOTCH 0x0021

typedef struct __attribute__((__packed__)) StoredOp_Notch
 {LightshowOpTypeID opTypeID;
  Color color1;
  Color color2;
  uint8_t color2Start;  // 0 -> 100, percentage of LED strip length
  uint8_t color2End;    // 0 -> 100, percentage of LED strip length
  unsigned long duration;}  // milliseconds
 StoredOp_Notch;

typedef struct LiveOpData_Notch
 {unsigned long endTime;  // milliseconds since start of current lightshow
  float c0;   // control point #0, expressed as number from 0.0 -> 1.0
  float c1;}  // control point #1, expressed as number from 0.0 -> 1.0
 LiveOpData_Notch;


void beginOp_Notch (LightshowOp* op, 
                    unsigned long startTime);

boolean updateOp_Notch (LightshowDevice_NeoPixel* device,
                        LightshowOp* op, 
                        unsigned long time);


////////////////////////////////////////////////////////////////////////////////
/// Pulsing lights
///
/// Most of these flash operators work with periodic functions like sines,
/// square waves, etc. and share a lot of common code.
///

#define LI_SINE_FLASH     0x0100
#define LI_SQUARE_FLASH   0x0101
#define LI_TRIANGLE_FLASH 0x0102
#define LI_SAWTOOTH_FLASH 0x0103


typedef struct __attribute__((__packed__)) StoredOp_Flash
 {LightshowOpTypeID opTypeID;
  Color color;
  unsigned long period;
  uint8_t amplitude;  // 0 -> 255
  uint8_t phase;   // 0 -> 100, which translates to 0.0 -> 1.0 cycles
  unsigned long duration;}  // milliseconds
 StoredOp_Flash;

typedef struct LiveOpData_Flash
 {unsigned long startTime;  // start of op
  unsigned long endTime;  // milliseconds since start of current lightshow
  float cycleLength;
  float (*waveFn)  (float x, float frequency);
  float frequency;
  float amplitude;
  float phase;}
 LiveOpData_Flash;


void beginOp_Flash (LightshowOp* op, 
                    unsigned long startTime,
                    float cycleLength,
                    float (*waveFn)  (float x, float frequency));

boolean updateOp_Flash (LightshowDevice_NeoPixel* device,
                        LightshowOp* op, 
                        unsigned long time);


////////////////////////////////////////////////////////////////////////////////
/// LI_CYLON_EYE

#define LI_CYLON_EYE 0x0200

typedef struct __attribute__((__packed__)) StoredOp_CylonEye
 {LightshowOpTypeID opTypeID;
  Color color;
  unsigned long period;    // milliseconds
  uint16_t tailLength;     // # LEDs
  uint8_t fadeTail;        // 0 = fade disabled; 1 = fade enabled
  unsigned long duration;}  // milliseconds
 StoredOp_CylonEye;

typedef struct LiveOpData_CylonEye
 {unsigned long startTime;  // start of op
  unsigned long endTime;}  // milliseconds since start of current lightshow
 LiveOpData_CylonEye;


void beginOp_CylonEye (LightshowOp* op, unsigned long startTime);

boolean updateOp_CylonEye (LightshowDevice_NeoPixel* device,
                           LightshowOp* op, 
                           unsigned long time);


////////////////////////////////////////////////////////////////////////////////
/// LI_THEATER_CHASE
///
/// Theater chase shifts a repeated pattern across the strip.  This pattern
/// is made of dots, where a dot can be one or more pixels.  The pattern is 
/// always an integer multiple of the dots.  The first dot of the pattern is 
/// color #1.  This is followed by one or more dots of color #2.
///

#define LI_THEATER_CHASE 0x0201

typedef struct __attribute__((__packed__)) StoredOp_TheaterChase
 {LightshowOpTypeID opTypeID;
  Color color1;
  Color color2;
  unsigned long period;    // time it takes for the pattern to cycle once [ms]
  uint8_t shiftDir;        // one of the SHIFT_DIR_* constants
  uint16_t dotSize;        // # LEDs
  uint16_t patternSize;    // # dots
  unsigned long duration;} // milliseconds
 StoredOp_TheaterChase;

typedef struct LiveOpData_TheaterChase
 {unsigned long startTime;  // start of op
  unsigned long endTime;}   // milliseconds since start of current lightshow
 LiveOpData_TheaterChase;


void beginOp_TheaterChase (LightshowOp* op, unsigned long startTime);

boolean updateOp_TheaterChase (LightshowDevice_NeoPixel* device,
                               LightshowOp* op, 
                               unsigned long time);


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/// Secord order operators (operators that work on one or more child operations)

typedef struct __attribute__((__packed__)) StoredOp_SecondOrderOp
 {LightshowOpTypeID opTypeID;
  uint8_t numChildOps;}
 StoredOp_SecondOrderOp;

typedef struct LiveOpData_SecondOrderOp
 {uint8_t numChildOps;
  LightshowOp** childOps;
  uint8_t* pixels;}  // optional pixel buffer
 LiveOpData_SecondOrderOp;


void beginOp_SecondOrderOp (LightshowDevice_NeoPixel* device,
                            LightshowOp* op, 
                            unsigned long startTime,
                            boolean usesOwnPixelBuffer);

void initSecondOrderOp (LightshowDevice_NeoPixel* device,
                        LightshowOp* op, 
                        unsigned long startTime,
                        boolean usesOwnPixelBuffer,
                        uint8_t numChildOps,
                        size_t sizeofStoredOp);

void beginChildOps (LightshowDevice_NeoPixel* device,
                    unsigned long startTime,
                    LiveOpData_SecondOrderOp* opData,
                    uint8_t numChildOps,
                    const uint8_t* storedOpStart,
                    size_t sizeofStoredOp);


void endOp_SecondOrderOp (LightshowOp* op);


size_t sizeOfOp_SecondOrderOp (LightshowOp* op);
size_t sizeOfChildOps (LightshowOp* op);


boolean updateOp_Blend (LightshowDevice_NeoPixel* device,
                        LightshowOp* op, 
                        unsigned long time,
                        uint8_t blendMode);


#define LI_MULTIPLY        0x8100
#define LI_SCREEN          0x8101
#define LI_OVERLAY         0x8102
#define LI_HARDLIGHT       0x8103
#define LI_SOFTLIGHT       0x8104
#define LI_REPLACE         0x8105
#define LI_COLOR_DODGE     0x8106
#define LI_COLOR_BURN      0x8107
#define LI_A_MINUS_B       0x8108
#define LI_B_MINUS_A       0x8109


////////////////////////////////////////////////////////////////////////////////
/// LI_FLIP

#define LI_FLIP 0x8000

boolean updateOp_Flip (LightshowDevice_NeoPixel* device,
                       LightshowOp* op, 
                       unsigned long time);


////////////////////////////////////////////////////////////////////////////////
/// LI_NEGATIVE

#define LI_NEGATIVE 0x8001

boolean updateOp_Negative (LightshowDevice_NeoPixel* device,
                           LightshowOp* op, 
                           unsigned long time);


////////////////////////////////////////////////////////////////////////////////
/// LI_SHIFT
/// 
/// This shifts the pixels up or down the LED strip, wrapping around as
/// needed.  The period is the amount of time it takes for the entire display 
/// to be shifted completely around.
/// 

#define LI_SHIFT 0x8002

typedef struct __attribute__((__packed__)) StoredOp_Shift
 {LightshowOpTypeID opTypeID;
  unsigned long period;    // milliseconds
  uint8_t shiftDir;        // one of the SHIFT_DIR_* constants
  uint8_t numChildOps;}
 StoredOp_Shift;

typedef struct LiveOpData_Shift
 // has same initial fields as LiveOpData_SecondOrderOp
 {uint8_t numChildOps;
  LightshowOp** childOps;
  uint8_t* pixels;  // pixel buffer
  unsigned long startTime;}  // start of op
 LiveOpData_Shift;


void beginOp_Shift (LightshowDevice_NeoPixel* device,
                    LightshowOp* op, 
                    unsigned long startTime);

boolean updateOp_Shift (LightshowDevice_NeoPixel* device,
                        LightshowOp* op, 
                        unsigned long time);


////////////////////////////////////////////////////////////////////////////////
/// LI_FADE
/// 
/// This does a fade in when the child op starts and a fade out when it ends.
/// 

#define LI_FADE 0x8003

typedef struct __attribute__((__packed__)) StoredOp_Fade
 {LightshowOpTypeID opTypeID;
  unsigned long fadeInDuration;   // milliseconds
  unsigned long fadeOutDuration;  // milliseconds
  unsigned long duration;         // milliseconds
  uint8_t numChildOps;}
 StoredOp_Fade;

typedef struct LiveOpData_Fade
 // has same initial fields as LiveOpData_SecondOrderOp
 {uint8_t numChildOps;
  LightshowOp** childOps;
  uint8_t* pixels;          // pixel buffer
  unsigned long startTime;  // start of op
  unsigned long fadeInEndTime;
  unsigned long fadeOutStartTime;
  unsigned long endTime;}
 LiveOpData_Fade;


void beginOp_Fade (LightshowDevice_NeoPixel* device,
                   LightshowOp* op, 
                   unsigned long startTime);

boolean updateOp_Fade (LightshowDevice_NeoPixel* device,
                       LightshowOp* op, 
                       unsigned long time);


////////////////////////////////////////////////////////////////////////////////

#endif  // LIGHTSHOW_OPS_h

////////////////////////////////////////////////////////////////////////////////
/// End of Code
