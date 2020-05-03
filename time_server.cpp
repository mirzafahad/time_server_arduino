/***********************************************************************
 * @file      time_server.cpp
 * @author    Fahad Mirza (fmirz007@7-11.com) 
 * @version   V1.0
 * @brief     Time server infrastructure
 ***********************************************************************/

/*** Includes **********************************************************/
#include <stddef.h>
#include <Arduino.h>
#include "time_server.h"


/*** Private Variables *************************************************/
static sTimerEvent_t *TimerListHead = NULL;


/*** Private Functions Declarations ************************************/
static void inserTimerEvent(sTimerEvent_t *obj);
static bool timer_exists(sTimerEvent_t *obj);


/*** Functions Definitions *********************************************/
/*********************************************************************** 
 * @brief      Timer event initialization
 * @details    Initializes all the TimerEven_t variables with default
 *             value
 * @param[in]  obj - Timer event object pointer
 * @param[in]  callback - Timer event's callback function pointer
 * @return     None
 ***********************************************************************/
void Timer_Init(sTimerEvent_t *obj, void (*callback)(void))
{
  obj->previousMillis = 0;
  obj->Interval       = 0;
  obj->IsRunning      = false;
  obj->Callback       = callback;
  obj->Next           = NULL;
}

/*********************************************************************** 
 * @brief      Set interval and start a timer event
 * @param[in]  obj - TimerEvent_t object pointer
 * @param[in]  interval_ms - in milliseconds
 * @return     None
 ***********************************************************************/
void Timer_Start(sTimerEvent_t *obj, uint32_t interval_ms)
{
    if( (obj == NULL) || (timer_exists(obj) == true) )
    {
        return;
    }
    obj->Interval = interval_ms;
    Timer_Start(obj);
}

/*********************************************************************** 
 * @brief     Start a timer event
 * @param[in] obj - TimerEvent_t object pointer
 * @return    None
 ***********************************************************************/
void Timer_Start(sTimerEvent_t *obj)
{
    if( (obj == NULL) || (timer_exists(obj) == true) )
    {
        return;
    }
    obj->previousMillis = millis();
    obj->IsRunning = true;
    inserTimerEvent(obj);
}

/*********************************************************************** 
 * @brief      Set timer event's interval
 * @param[in]  obj - TimerEvent_t object pointer
 * @param[in]  interval_ms - interval in milliseconds
 * @return     None
 ***********************************************************************/
void Timer_SetTime(sTimerEvent_t *obj, uint32_t interval_ms)
{
    if(obj == NULL)
    {
        return;
    }
    obj->Interval = interval_ms;
}

/*********************************************************************** 
 * @brief      Stop the timer event and remove from the linked list
 * @param[in]  obj - TimerEvent_t object pointer
 * @return     None
 ***********************************************************************/
void Timer_Stop(sTimerEvent_t *obj)
{
  if(obj == NULL)
  {
      return;
  }
  obj->IsRunning = false;
  obj->previousMillis = 0;
  
  sTimerEvent_t* prev = TimerListHead;
  sTimerEvent_t* cur = TimerListHead;

  if(TimerListHead == obj)
  {
    TimerListHead = TimerListHead->Next;
  }
  else
  {
    while(cur != NULL)
    {
      if(cur == obj)
      {
        prev->Next = cur->Next;
        return;
      }
      else
      {
        prev = cur;
        cur = cur->Next;
      }
    }
  }
}

/*********************************************************************** 
 * @brief      Restart a timer event
 * @details    Restart a timer event by stopping the timer event and 
 *             then starting it again
 * @param[in]  obj - TimerEvent_t object pointer
 * @return     None
 ***********************************************************************/
void Timer_Restart(sTimerEvent_t *obj)
{
    Timer_Stop ( obj );
    Timer_Start( obj );
}

/*********************************************************************** 
 * @brief    Handles the time for all timer event
 * @details  Call this fucnction in loop()
 * @param    None
 * @return   None
 ***********************************************************************/
void Timer_Handler(void)
{
  unsigned long currentMillis = millis();

  sTimerEvent_t* cur = TimerListHead;
  while(cur != NULL)
  {
    // This also handles the uint32_t wrap around
    if((unsigned long)(currentMillis - cur->previousMillis) >= cur->Interval)
    {
        // Remove the instance from the linkedList
        Timer_Stop(cur);
        cur->Callback();
    }
    cur = cur->Next;
  }
}

/*********************************************************************** 
 * @brief    Print timer instance 
 * @details  Debug function to print all timer instances' address in 
 *           the linked list
 * @param    None
 * @return   None
 ***********************************************************************/
void Timer_PrintAllInstance(void)
{
  #if DEBUG
  sTimerEvent_t* cur = TimerListHead;
  DBG_Println("PrintAllInstance:");
  
  while(cur != NULL)
  {
    uint32_t add = (uint32_t)cur;
    Serial.print(add, HEX);
    Serial.println();
    cur = cur->Next;
  }

  DBG_Println(F("PrintAllInstance: Done"));
  #endif
}


/*********************************************************************** 
 * @brief      Add timer event to the linked list
 * @param[in]  obj - sTimerEvent_t object pointer
 * @return     None
 ***********************************************************************/
static void inserTimerEvent(sTimerEvent_t *obj)
{
  if(TimerListHead == NULL)
  {
    TimerListHead = obj; 
  }
  else
  {
    // Find the next available space to store
    sTimerEvent_t* cur = TimerListHead;
    
    while (cur->Next != NULL )
    {
       cur = cur->Next;
    }
    cur->Next = obj;
    obj->Next = NULL;
  }
}

/*********************************************************************** 
 * @brief      Check if a timer instance is already exist in the 
 *             linked list
 * @param[in]  obj - sTimerEvent_t object pointer
 * @return     None
 ***********************************************************************/
static bool timer_exists(sTimerEvent_t *obj)
{
  sTimerEvent_t* cur = TimerListHead;

  while(cur != NULL)
  {
    if( cur == obj )
    {
      return true;
    }
    cur = cur->Next;
  }

  return false;
}


/*****END OF FILE****/
