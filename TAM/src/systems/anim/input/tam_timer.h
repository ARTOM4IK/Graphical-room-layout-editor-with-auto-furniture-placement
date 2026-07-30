/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tam_timer.h
 * PURPOSE     : Tough Arrangement Manager project.
 *               Timer declaration file.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Kochugueva Maria (MK3).
 * LAST UPDATE : 19.02.2026
 * NOTE        : Namespace 'tam'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __tam_timer_h_
#define __tam_timer_h_

#include "tam_def.h"

/* 'tam' namespace */
namespace tam
{
  /* 'timer' handle class */
  class timer
  {
  private:
    UINT64
      StartTime,                     // Start program time
      OldTime,                       // Previous frame time
      OldTimeFPS,                    // Old time FPS measurement
      PauseTime,                     // Time during pause period
      TimePerSec,                    // Timer resolution
      FrameCounter;                  // Frames counter
  public:
    DBL GlobalTime, GlobalDeltaTime, // Global time and interframe interval
       Time, DeltaTime,              // Time with pause and interframe interval
       FPS;                          // Frames per second value
    BOOL IsPause {};                 // Pause flag
    
    /* Class default constructor */
    timer( VOID ) : FrameCounter(0), GlobalTime(0), GlobalDeltaTime(0), Time(0), DeltaTime(0), PauseTime(0), FPS(102.47), IsPause(FALSE)
    {
      LARGE_INTEGER CurrentTime;

      /* Query timer information */
      QueryPerformanceFrequency(&CurrentTime);
      TimePerSec = CurrentTime.QuadPart;
      QueryPerformanceCounter(&CurrentTime);
      StartTime = OldTime = OldTimeFPS = CurrentTime.QuadPart;
    } /* End of 'timer' function */
    
    /* Class destructor */
    ~timer( VOID )
    {
    } /* End of '~timer' function */
    
    /* Reset timer function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Reset( VOID )
    {
    } /* End of 'Reset' function */
    
    /* Response timer function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Response( VOID )
    {
      LARGE_INTEGER CurrentTime;
 
      /* Query current timer value */
      QueryPerformanceCounter(&CurrentTime);

      /* Evaluate global time values */
      GlobalTime = (DBL)(CurrentTime.QuadPart - StartTime) / TimePerSec;
      GlobalDeltaTime = (DBL)(CurrentTime.QuadPart - OldTime) / TimePerSec;
      
      /* Evaluate local time (time with pause) values */
      if (IsPause)
      {
        DeltaTime = 0;
        PauseTime += CurrentTime.QuadPart - OldTime;
      }
      else
      {
        DeltaTime = GlobalDeltaTime;
        Time = (DBL)(CurrentTime.QuadPart - PauseTime - StartTime) / TimePerSec;
      }
      
      /* FPS */
      FrameCounter++;
      if (CurrentTime.QuadPart - OldTimeFPS > TimePerSec)
      {
        FPS = FrameCounter * TimePerSec / (DBL)(CurrentTime.QuadPart - OldTimeFPS);
        OldTimeFPS = CurrentTime.QuadPart;
        FrameCounter = 0;
      }
      OldTime = CurrentTime.QuadPart;
    } /* End of 'Response' function */
  }; /* End of 'timer' class */
} /* end of 'tam' namespace */

#endif /* __tam_timer_h_ */

/* END OF 'tam_timer.h' FILE */
