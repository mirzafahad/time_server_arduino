/*****************************************************************************/
/*!
 * @file      time_server.cpp
 * @mainpage  Arduino Time Server Library
 *
 * @section intro_sec Introduction
 * This is the documentation for a Time Server library for the
 * Arduino platform. It is designed specifically to work with the
 * ATmega328's Timer 1 peripheral.
 *
 * @section author Author
 *
 * Written by Fahad Mirza (fahadmirza8@gmail.com).
 *
 * @section license License
 *
 * BSD license, all text here must be included in any redistribution.
 *
 * @section version Version
 *
 * V1.0
 */
/*****************************************************************************/
/*--- Includes --------------------------------------------------------------*/

#include "time_server.h"

/*--- Typedefs --------------------------------------------------------------*/
/** TimerEvents will be tracked by a linked list.
    This is the structure for the linked list.*/
struct sTimerEventNode {
  TimerEvent *timer_event; ///< Timer Event info
  sTimerEventNode *next;   ///< Pointer to next timer object
};

/*****************************************************************************/
/*!
    @brief  LinkedList class to keep track of timer-events.
*/
/*****************************************************************************/
class LinkedList {
public:
  /** Header of the LinkedList.*/
  sTimerEventNode *head_;

  /** Number of events running. If it running events are down to zero,
          disable the timer interrupt.*/
  uint8_t no_of_events_running_;

  /** A flag to keep track if the timer interrupt is already initialized.*/
  bool timer_initialized_;

  /*************************************************************************/
  /*!
   * @brief   LinkedList constructor
   */
  /************************************************************************/
  LinkedList() {
    head_ = nullptr;
    no_of_events_running_ = 0;
    timer_initialized_ = false;
  }

  /*************************************************************************/
  /*!
   * @brief   Add timer event to the linked list
   * @param   obj
   *          A refernce to Timer object (sTimerEvent_t)
   */
  /************************************************************************/
  void InsertEvent(TimerEvent *obj) {
    if (nullptr == head_) {
      head_ = new sTimerEventNode{obj, nullptr};
    } else {
      // Find the next available space to store
      sTimerEventNode *cur = head_;

      while (cur->next != nullptr) {
        cur = cur->next;
      }

      cur->next = new sTimerEventNode{obj, nullptr};
    }
  }

  /*************************************************************************/
  /*!
   * @brief   Check if a TimerEvent instance is already exist
   *          in the linked list
   * @param   obj - sTimerEvent_t object pointer
   * @return  true or false
   */
  /************************************************************************/
  bool EventExists(const TimerEvent *obj) {
    sTimerEventNode *cur = head_;

    while (cur != nullptr) {
      if (cur->timer_event == obj) {
        return true;
      }
      cur = cur->next;
    }
    return false;
  }

  /*************************************************************************/
  /*!
   * @brief   Initialize timer1 interrupt
   */
  /************************************************************************/
  void InitTimerIsr(void) {
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

    if (false == timer_initialized_) {
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
      timer_initialized_ = true;
    }
  }

  /*************************************************************************/
  /*!
   * @brief   Disable timer 1 interrupt
   */
  /************************************************************************/
  void DisableTimerIsr(void) {
    TIMSK1 &= ~(1 << OCIE1A);
    timer_initialized_ = false;
  }
};

/*--- Private Variables -----------------------------------------------------*/
static LinkedList TimerList;

/*--- Functions Definitions -------------------------------------------------*/

TimerEvent::TimerEvent(Callback cb, uint32_t interval_ms, boolean repeat) {
  elapsed_time_ms_ = 0;
  interval_ms_ = interval_ms;
  is_running_ = false;
  repeat_ = repeat;
  cb_ = cb;
}

void TimerEvent::SetInterval(uint32_t interval_ms) {
  interval_ms_ = interval_ms;
}

void TimerEvent::Start(uint32_t interval_ms) {
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
void TimerEvent::Start(void) {
  // If no time is provided, dont include it
  if (0 == interval_ms_) {
    return;
  }

  // Is it already in the LinkedList
  if (TimerList.EventExists(this) == true) {
    if (true == is_running_) {
      // It is already running, don't include
      return;
    }
  } else {
    TimerList.InsertEvent(this);
  }

  elapsed_time_ms_ = interval_ms_;
  is_running_ = true;
  TimerList.no_of_events_running_++;

  // Note: If it is already initialized, init() won't init timer again.
  TimerList.InitTimerIsr();
}

void TimerEvent::Stop(void) {
  if (true == is_running_) {
    if (TimerList.no_of_events_running_ > 0) {
      TimerList.no_of_events_running_--;
    }
  }

  is_running_ = false;
  elapsed_time_ms_ = 0;

  if (0 == TimerList.no_of_events_running_) {
    TimerList.DisableTimerIsr();
  }
}

void TimerEvent::Restart(void) {
  Stop();
  Start();
}

/*****************************************************************************/
/*!
 * @brief   Timer 1 interrupt ISR
 */
/*****************************************************************************/
ISR(TIMER1_COMPA_vect) {
  sTimerEventNode *cur = TimerList.head_;

  // Decrease 1ms from the elapsed_time of the running events
  while (cur != nullptr) {
    if (cur->timer_event->is_running_) {
      // Decrement 1ms
      cur->timer_event->elapsed_time_ms_--;
    }
    cur = cur->next;
  }

  // Now take out the expired Nodes and execute their callbacks
  cur = TimerList.head_;
  while (cur != nullptr) {
    if (0 == cur->timer_event->elapsed_time_ms_) {
      // Execute the callback
      if (cur->timer_event->cb_ != nullptr) {
        cur->timer_event->cb_();
      }

      if (cur->timer_event->repeat_) {
        cur->timer_event->elapsed_time_ms_ = cur->timer_event->interval_ms_;
      } else {
        // Remove the instance from the linkedList
        cur->timer_event->Stop();
      }
    }
    cur = cur->next;
  }
}

/*****************************************************************************/
/*!
 * @brief   Print timer instance
 *          Debug function to print all timer instances' address in
 *          the linked list
 */
/*****************************************************************************/
void Timer_PrintAllInstance(void) {
#if DEBUG
  sTimerEventNode *cur = TimerList.head_;
  DBG_Println("PrintAllInstance:");

  while (cur != nullptr) {
    uint32_t address = (uint32_t)cur;
    Serial.print(address, HEX);
    Serial.println();
    cur = cur->next;
  }

  DBG_Println(F("PrintAllInstance: Done"));
#endif
}
/*****END OF FILE****/
