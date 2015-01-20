/*
================================================================================
File name: time.cpp
   System: Gothee House Clock
 Platform: Teensy 3.1 Freescale K20 (ARM Cortex M4, MK20DX256VLH7)
   Author: Madeline Usher
  Created: January 7, 2015
  Purpose: Just some utilities that help with timing.
================================================================================
  $LastChangedDate: 2015-01-17 02:46:25 -0600 (Sat, 17 Jan 2015) $
  $LastChangedBy: usher $
================================================================================
*/

#include "time.h"


////////////////////////////////////////////////////////////////////////////////
/// The millis() function wraps around to zero every 49.7 days.  This  function 
/// computes time differences that work even if that wrap-around happened since 
/// the startTime (assuming duration is less than 49.7 days).

unsigned long elapsedMillis (unsigned long startTime)
 {return(elapsedMillis(startTime, millis()));}

 
unsigned long elapsedMillis (unsigned long startTime, unsigned long curTime)
 {if (curTime >= startTime)
   {return (curTime - startTime);}
  else
   {return ((0xffffffff - startTime) + curTime);}}


////////////////////////////////////////////////////////////////////////////////
/// End of Code
