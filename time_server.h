/*******************************************************************************
 * File       : time_server.h
 * Author     : Fahad Mirza (fahadmirza8@gmail.com)
 * Version    : V1.0
 * Created on : Sep 25, 2017
 * Modified   : May 29, 2019
 * Brief      : Time server driver
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2017 FahadMirza
 * All rights reserved.</center></h2>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted, provided that the following conditions are met:
 *
 * 1. Redistribution of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. Neither the name of Fahad Mirza nor the names of other
 *    contributors to this software may be used to endorse or promote products
 *    derived from this software without specific written permission.
 * 4. Redistribution and use of this software other than as permitted under
 *    this license is void and will automatically terminate your rights under
 *    this license.
 *
 * THIS SOFTWARE IS PROVIDED BY FAHAD MIRZA AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
 * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT
 * SHALL FAHAD MIRZA OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************
 */

 /* Define to prevent recursive inclusion -------------------------------------*/
#ifndef TIME_SERVER_H
#define TIME_SERVER_H


#if ( ARDUINO >= 100 )
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif


typedef void (*Callback)(void);
/**************************************************************************/
/*! 
    @brief  Class that stores state and functions for interacting with Time Server
*/
/**************************************************************************/
class Timer_Server
{
  public:
    //Constructor
    /* 
     * Timer event initialization.
     * Param :   interval - Time intervl for callback,
     *           cb       - Callback function pointer
     * Return:   none
     */
    Timer_Server(uint32_t interval, Callback cb);
    Timer_Server(void);
    ~Timer_Server();

    void setCallback(Callback cb);
    void setInterval(uint32_t interval);

    /* 
     * Start a particular timer event.
     * Param :   interval_ms - in milliseconds
     * Return:   none
     */
    void start(void);
    void start(uint32_t interval);

    /* 
     * Stop the timer event and remove from the linked list.
     * Param :   obj - TimerEvent_t object pointer
     * Return:   none
     */
    void stop(void);

    void restart(void);

  private:
    uint32_t PreviousMillis; // Last Mills value to compare with mills()
    uint32_t Interval;       // Interval before execute callback()
    boolean  IsRunning;      // Is the timer currently running?
    Callback Cb;             // Timer IRQ callback function
}


/************** Private Variables **************/
// Timer Event struct
typedef struct sTimerEvent
{
  struct sTimerEvent *Next;     // Pointer to the next Timer object.
}TimerEvent_t;




void Timer_PrintAllInstance(void);



/* 
 * Call this fucnction in main loop.
 * Param :   none
 * Return:   none
 */
void Time_Server_Handler(void);


#endif /* TIME_SERVER_H */

/************************ (C) COPYRIGHT FahadMirza ************************/
/*****END OF FILE****/
