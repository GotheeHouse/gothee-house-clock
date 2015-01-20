/*
================================================================================
File name: time.h
   System: Gothee House Clock
 Platform: Teensy 3.1 Freescale K20 (ARM Cortex M4, MK20DX256VLH7)
   Author: Madeline Usher
  Created: January 7, 2015
  Purpose: Just some utilities that help with timing.
================================================================================
  $LastChangedDate: 2015-01-07 11:46:59 -0600 (Wed, 07 Jan 2015) $
  $LastChangedBy: usher $
================================================================================
*/

#ifndef TIME_h
#define TIME_h

#include <inttypes.h>
#include <Wire.h>


////////////////////////////////////////////////////////////////////////////////
/// External functions

extern unsigned long elapsedMillis (unsigned long startTime);
 
extern unsigned long elapsedMillis (unsigned long startTime, 
                                    unsigned long curTime);


////////////////////////////////////////////////////////////////////////////////

#endif  // TIME_h

////////////////////////////////////////////////////////////////////////////////
/// End of Code
