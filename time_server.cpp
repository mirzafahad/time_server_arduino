/***********************************************************************
 * @file      time_server.cpp
 * @author    Fahad Mirza (fmirz007@7-11.com) 
 * @version   V1.0
 * @brief     Time server infrastructure
 ***********************************************************************/

/*** Includes **********************************************************/

#include "time_server.h"


/*** Typedefs **********************************************************/
// TimerEvents will tracked by a linked list
// Note: I didn't want the linked list to be a part of the TimerEvent class
typedef struct sTimerEventNode
{
  TimerEvent *timer_event;
  struct sTimerEventNode *next;
}sTimerEventNode_t;


/*** Private Variables *************************************************/
// LinkedList head
static sTimerEventNode_t *gTimerListHead = nullptr;

// Keep track of how many events are running. If it
// is down to zero, disable the timer interrupt.
static uint8_t gEventsRunning = 0;

// A flag to keep track if the timer interrupt is already initialized
static bool gTimerInitialized = false;


/*** Private Functions Declarations ************************************/
static void initTimerISR(void);
static void disableTimerISR(void);
static void insertTimerEvent(TimerEvent *obj);
static bool timerEventExists(TimerEvent *obj);
static void removeTimerEvent(TimerEvent *obj);

/*** Functions Definitions *********************************************/

TimerEvent::TimerEvent(Callback cb, uint32_t interval_ms, boolean repeat)
{
  elapsed_time_ms_ = 0; 
  interval_ms_ = interval_ms;
  is_running_ = false;
  repeat_ = repeat; 
  cb_ = cb;
}


void TimerEvent::SetInterval(uint32_t interval_ms)
{
  interval_ms_ = interval_ms;
}


void TimerEvent::Start(uint32_t interval_ms)
{
  interval_ms_ = interval_ms;
  Start();
}


/*
 * For a new TimerEvent Object, Start() will dynamically allocate
 * some space to keep the object in the linked list. After the event
 * happens, the event will be stopped by setting is_running_ flag to
 * false and won't release the memory space. 
 * 
 * This is an implementation detail. In my experience, most of the
 * the event will be repeated. And in other cases the events will be
 * called once in a while e.g. after you send some character through 
 * uart start a timer-event for timeout to see if you received a 
 * response. 
 * 
 * In all those cases the timer-event will live forever through out the
 * application. So it didn't make sense to release the dynamically 
 * allocated spaces after the event is executed. That will defragment
 * the heap space.
 * 
 * So make sure any timer-event that is created doesn't get destroyed 
 * during the lifetime of the device.
 */
void TimerEvent::Start(void)
{
  // If no time is provided, dont include it
  if(interval_ms_ == 0)
  {
    return;
  }
    
  // Is it already in the LinkedList
  if(timerEventExists(this) == true)
  {
    if(is_running_ == true)
    {
      // It is already running, don't include
      return;
    }
  }
  else
  {
    insertTimerEvent(this);
  }

  elapsed_time_ms_ = interval_ms_;
  is_running_ = true;
  gEventsRunning++;

  // Note: If it is already initialized, init() won't init timer again.
  initTimerISR(); 
}


void TimerEvent::Stop(void)
{
  if(is_running_ == true)
  {
    if(gEventsRunning > 0)
    {
      gEventsRunning--;
    }
  }
  
  is_running_ = false;
  elapsed_time_ms_ = 0;

  if(gEventsRunning == 0)
  {
    disableTimerISR();
  }
}


void TimerEvent::Restart(void)
{
  Stop();
  Start();
}

/************************ static functions common to all instances ************************/

/*********************************************************************** 
 * @brief   Check if a TimerEvent instance is already exist  
 *          in the linked list
 * @param   obj - sTimerEvent_t object pointer
 * @return  true or false
 ***********************************************************************/
static bool timerEventExists(TimerEvent *obj)
{
  sTimerEventNode_t *cur = gTimerListHead;

  while(cur != nullptr)
  {
    if(cur->timer_event == obj)
    {
      return true;
    }
    cur = cur->next;
  }

  return false;
}


/*********************************************************************** 
 * @brief   Add timer event to the linked list
 * @param   obj - sTimerEvent_t object pointer
 * @return  none
 ***********************************************************************/
static void insertTimerEvent(TimerEvent *obj)
{
  if(gTimerListHead == nullptr)
  {
    gTimerListHead = new sTimerEventNode_t{obj, nullptr};
  }
  else
  {
    // Find the next available space to store
    sTimerEventNode_t *cur = gTimerListHead;
    
    while (cur->next != nullptr)
    {
       cur = cur->next;
    }

    cur->next = new sTimerEventNode_t{obj, nullptr};
  }
}


/*********************************************************************** 
 * @brief   Initialize timer1 interrupt
 * @param   none
 * @return  none
 ***********************************************************************/
static void initTimerISR(void)
{
  /*
   * Waveform Generation Mode: Mode 4->CTC
   *      In this mode once timer reaches OCR1A value
   *      it will go back to zero, and start counting again.
   *      
   * Prescaler: 1
   *      The clock will receive the system clocl i.e. 16MHz
   *      
   * Output Compare Register: 16000
   *      To generate 1ms interrupt
   *      
   * Interrupt Mask: Enable OCIE1A interrupt
   *      Generate interrupt when timer reaches OCR1A
   */

  if(gTimerInitialized == false)
  {   
  
    // Setting WGM's last two bits to zero
    TCCR1A = 0;
  
    // Setting WGM's other two bits
    TCCR1B &= ~(1 << WGM13);
    TCCR1B |= (1 << WGM12);
  
    // Selecting prescaler 1
    TCCR1B |= (1 << CS10);
    TCCR1B &= ~(1 << CS11);
    TCCR1B &= ~(1 << CS12); 

    // Clear counter register
    TCNT1 = 0;
  
    // Set OCR1A to generate 1ms 
    OCR1A = 16000;

    // Enable the interrupt
    TIMSK1 = (1 << OCIE1A);

    // Enable global interrupt
    sei();

    // Set flag to avoid re-initialization
    gTimerInitialized = true;
  }
}


/*********************************************************************** 
 * @brief   Disable timer 1 interrupt
 * @param   none
 * @return  none
 ***********************************************************************/
static void disableTimerISR(void)
{
  TIMSK1 &= ~(1 << OCIE1A);
  gTimerInitialized = false;
}

/*********************************************************************** 
 * @brief   Timer 1 interrupt ISR
 * @param   none
 * @return  none
 ***********************************************************************/
ISR(TIMER1_COMPA_vect)
{
  sTimerEventNode_t *cur = gTimerListHead;

  // Decrease 1ms from the elapsed_time of the running events
  while(cur != nullptr)
  {
    if(cur->timer_event->is_running_)
    {
      // Decrement 1ms
      cur->timer_event->elapsed_time_ms_--;
    }
    cur = cur->next;
  }

  // Now take out the expired Nodes and execute their callbacks
  cur = gTimerListHead;
  while(cur != nullptr)
  {
    if(cur->timer_event->elapsed_time_ms_ == 0)
    {
      // Execute the callback
      if(cur->timer_event->cb_ != nullptr)
      {
        cur->timer_event->cb_();
      }

      if(cur->timer_event->repeat_)
      {
        cur->timer_event->elapsed_time_ms_ = cur->timer_event->interval_ms_;
      }
      else
      {
        // Remove the instance from the linkedList
        cur->timer_event->Stop();
      }
    }
    cur = cur->next;
  }        
}

/*********************************************************************** 
 * @brief   Print timer instance 
 *          Debug function to print all timer instances' address in 
 *          the linked list
 * @param   none
 * @return  none
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
