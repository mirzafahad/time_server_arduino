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


typedef struct sTimerEventNode
{
  TimerEvent *timerEvent;
  struct sTimerEventNode *next;
}sTimerEventNode_t;


/*** Private Variables *************************************************/
static sTimerEventNode_t *TimerListHead = NULL;


/*** Private Functions Declarations ************************************/
static void initTimerISR(void);
static void disableTimerISR(void);
static void insertTimerEvent(TimerEvent *obj);
static bool timerEventExists(TimerEvent *obj);
static void removeTimerEvent(TimerEvent *obj);

/*** Functions Definitions *********************************************/
/*********************************************************************** 
 * @brief      Timer event initialization
 * @details    Initializes all the TimerEven_t variables with default
 *             value
 * @param[in]  obj - Timer event object pointer
 * @param[in]  callback - Timer event's callback function pointer
 * @return     None
 ***********************************************************************/
TimerEvent::TimerEvent(uint32_t interval_ms, Callback cb, boolean repeat = false)
{
  ElapsedTime_ms = 0; 
  Interval_ms = interval_ms;
  IsRunning = false;
  Repeat = repeat; 
  Cb = cb;
}

TimerEvent::TimerEvent(void)
{
  ElapsedTime_ms = 0; 
  Interval_ms = 0;
  IsRunning = false;
  Repeat = false; 
  Cb = NULL;
}

void TimerEvent::setCallback(Callback cb)
{
  Cb = cb;
}

void TimerEvent::setInterval(uint32_t interval_ms)
{
  Interval_ms = interval_ms;
}


void TimerEvent::start(uint32_t interval_ms)
{
  Interval_ms = interval_ms;
  start();
}


void TimerEvent::start(void)
{
    // If no time is provided, dont include it
    if(Interval_ms == 0)
    {
      return;
    }
    
    // If the event is already in the linked list,
    // no need to include again
    if(timerEventExists(this) == true)
    {
        if(IsRunning == true)
        {
          return;
        }
        
        ElapsedTime_ms = Interval_ms;
        previousMillis = millis();
        IsRunning = true;
    }
    else
    {
      ElapsedTime_ms = Interval_ms;
      previousMillis = millis();
      insertTimerEvent(this);
      IsRunning = true;
      Serial.println("Start Done!");
    }
}


void TimerEvent::stop(void)
{
  IsRunning = false;
  ElapsedTime_ms = 0;
}

/*********************************************************************** 
 * @brief      Restart a timer event
 * @details    Restart a timer event by stopping the timer event and 
 *             then starting it again
 * @param[in]  none
 * @return     none
 ***********************************************************************/
void TimerEvent::restart(void)
{
    stop();
    start();
}

/************************ static functions common to all instances ************************/

/*********************************************************************** 
 * @brief      Check if a TimerEvent instance is already exist  
 *             in the linked list
 * @param[in]  obj - sTimerEvent_t object pointer
 * @return     None
 ***********************************************************************/
static bool timerEventExists(TimerEvent *obj)
{
  sTimerEventNode_t *cur = TimerListHead;

  while(cur != NULL)
  {
    if(cur->timerEvent == obj)
    {
      return true;
    }
    cur = cur->next;
  }

  return false;
}

/*********************************************************************** 
 * @brief      Add timer event to the linked list
 * @param[in]  obj - sTimerEvent_t object pointer
 * @return     None
 ***********************************************************************/
static void insertTimerEvent(TimerEvent *obj)
{
  if(TimerListHead == NULL)
  {
    TimerListHead = new sTimerEventNode_t{obj, NULL};
  }
  else
  {
    // Find the next available space to store
    sTimerEventNode_t *cur = TimerListHead;
    
    while (cur->next != NULL )
    {
       cur = cur->next;
    }

    cur->next = new sTimerEventNode_t{obj, NULL};
  }
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
        if(cur->Callback != NULL)
        {
          cur->Callback();
        }

        if(cur->Repeatable)
        {
          Timer_Start(cur);
        }
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









/*****END OF FILE****/
