/***********************************************************************
 * @file       : time_server.h
 * @author     : Fahad Mirza (fahad.mirza@7-11.com)
 * @version    : V1.0
 * @brief      : Time server driver
 ***********************************************************************/

#ifndef __TIMESERVER_H__
#define __TIMESERVER_H__

#include <stdbool.h>

/*** Public Typedef ****************************************************/
// Timer Event struct
typedef struct sTimerEvent
{
  unsigned long previousMillis; //<! Last Mills value to compare with mills()
  long Interval;                //<! Interval before execute callback()
  bool IsRunning;               //<! Is the timer currently running?
  void (*Callback)(void);       //<! Timer IRQ callback function
  struct sTimerEvent *Next;     //<! Pointer to the next Timer object.
}sTimerEvent_t;

/*** Public Functions Declarations *************************************/
/*********************************************************************** 
 * @brief      Timer event initialization
 * @details    Initializes all the TimerEven_t variables with default
 *             value
 * @param[in]  obj - Timer event object pointer
 * @param[in]  callback - Timer event's callback function pointer
 * @return     None
 ***********************************************************************/
void Timer_Init(sTimerEvent_t *obj, void (*callback)(void));

/*********************************************************************** 
 * @brief      Set interval and start a timer event
 * @param[in]  obj - TimerEvent_t object pointer
 * @param[in]  interval_ms - in milliseconds
 * @return     None
 ***********************************************************************/
void Timer_Start(sTimerEvent_t *obj, uint32_t interval_ms);
void Timer_Start(sTimerEvent_t *obj);

/*********************************************************************** 
 * @brief      Set timer event's interval
 * @param[in]  obj - TimerEvent_t object pointer
 * @param[in]  interval_ms - interval in milliseconds
 * @return     None
 ***********************************************************************/
void Timer_SetTime(sTimerEvent_t *obj, uint32_t interval_ms);

/*********************************************************************** 
 * @brief      Stop the timer event and remove from the linked list
 * @param[in]  obj - TimerEvent_t object pointer
 * @return     None
 ***********************************************************************/
void Timer_Stop(sTimerEvent_t *obj);

/*********************************************************************** 
 * @brief      Restart a timer event
 * @details    Restart a timer event by stopping the timer event and 
 *             then starting it again
 * @param[in]  obj - TimerEvent_t object pointer
 * @return     None
 ***********************************************************************/
void Timer_Restart(sTimerEvent_t *obj);


/*********************************************************************** 
 * @brief    Handles the time for all timer event
 * @details  Call this fucnction in loop()
 * @param    None
 * @return   None
 ***********************************************************************/
void Timer_Handler(void);

/*********************************************************************** 
 * @brief    Print timer instance 
 * @details  Debug function to print all timer instances' address in 
 *           the linked list
 * @param    None
 * @return   None
 ***********************************************************************/
void Timer_PrintAllInstance(void);

#endif /* __TIME_SERVER_H__ */


