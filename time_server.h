/*******************************************************************************
 * File       : timeserver.h
 * Author     : Fahad Mirza (fahad@haxiot.com)
 * Version    : V0.1
 * Created on : Sep 25, 2017
 * Modified   : Sep 26, 2017
 * Brief      : Time server driver
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2017 HaxIoT
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
 * 3. Neither the name of HaxIoT nor the names of other
 *    contributors to this software may be used to endorse or promote products
 *    derived from this software without specific written permission.
 * 4. Redistribution and use of this software other than as permitted under
 *    this license is void and will automatically terminate your rights under
 *    this license.
 *
 * THIS SOFTWARE IS PROVIDED BY HAXIOT AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
 * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT
 * SHALL HAXIOT OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
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
#ifndef TIMESERVER_H_
#define TIMESERVER_H_

#include <stdbool.h>

/************** Private Variables **************/
// Timer Event struct
typedef struct sTimerEvent
{
  unsigned long previousMillis; // Last Mills value to compare with mills()
  long Interval;                // Interval before execute callback()
  bool IsRunning;               // Is the timer currently running?
  void (*Callback)(void);       // Timer IRQ callback function
  struct sTimerEvent *Next;     // Pointer to the next Timer object.
}TimerEvent_t;


/* 
 * Timer event initialization.
 * Param :   obj - TimerEvent_t object pointer,
 *           callback - Callback function pointer
 * Return:   none
 */
void Timer_Init(TimerEvent_t *obj, void (*callback)(void));
void Timer_PrintAllInstance(void);


/* 
 * Start a particular timer event.
 * Param :   obj - TimerEvent_t object pointer,
 *           interval_ms - in milliseconds
 * Return:   none
 */
void Timer_Start(TimerEvent_t *obj, uint32_t interval_ms);
void Timer_Start(TimerEvent_t *obj);

void Timer_SetTime(TimerEvent_t *obj, uint32_t interval_ms);
/* 
 * Stop the timer event and remove from the linked list.
 * Param :   obj - TimerEvent_t object pointer
 * Return:   none
 */
void Timer_Stop(TimerEvent_t *obj);
void Timer_Restart(TimerEvent_t *obj);


/* 
 * Call this fucnction in main loop.
 * Param :   none
 * Return:   none
 */
void Timer_Handler(void);


#endif /* TIMESERVER_H_ */

/************************ (C) COPYRIGHT HaxIoT ************************/
/*****END OF FILE****/
