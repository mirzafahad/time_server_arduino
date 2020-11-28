/*****************************************************************************/
/*!
 * @file    time_server.h
 * author   Fahad Mirza (fahadmirza8@gmail.com)
 * version  V1.0
 * brief    Time server header file
 * license  MIT License
 */
/*****************************************************************************/

#ifndef TIME_SERVER_H_
#define TIME_SERVER_H_

/*--- Includes --------------------------------------------------------------*/
#include <Arduino.h>

/*--- Public Typedefs -------------------------------------------------------*/
/** Callback Function pointer typedef. To use inside TimerEvent class.*/
typedef void (*Callback)(void);

/*--- Class Definition ------------------------------------------------------*/
/*****************************************************************************/
/*!
 * @brief Time Server helps you to keep running time sensitive
 *        functions.
 *
 *		  Every 1ms time server will check which TimerEvents objects are
 *ready to execute. It is advisable to keep callback functions short.
 *
 *        Note:
 *        The class also assumes any TimerEvent objects that you will
 *        create will last until the device is turned off. This is
 *        important. The class keep tracks of it's objects through
 *        LinkedList and doesn't delete any object even if the event
 *        is not active. The class just skips that object during
 *        traversing. So, if any TimerEvent object is no longer
 *        available in the application the class won't know about it
 *        and will crash.
 *
 *        Example:
 *            static void onBlinkEvent(void);
 *            static TimerEvent BlinkTimer(onBlinkEvent, 250, true);
 *            BlinkTimer.Start();
 */
/*****************************************************************************/
class TimerEvent {
public:
  uint32_t interval_ms_;     ///< Interval before executing callback()
  uint32_t elapsed_time_ms_; ///< How much of interval_ms_ is passed?
  boolean is_running_;       ///< Is the event currently running?
  boolean repeat_;           ///< Is this event needs to be repeated
  Callback cb_;              ///< Callback function pointer

  /*************************************************************************/
  /*!
   * @brief  Class Constructor
   * @param  cb
   *         Pointer of Callback function.
   * @param  interval_ms
   *         When to call the callback function.
   * @param  repeat
   *         If this event a one shot or needs to be predically repeated.
   * Note:   Interval can be set later, if the callback doesn't need to be
   *         executed in regular interval.
   ***********************************************************************/
  explicit TimerEvent(Callback cb, uint32_t interval_ms = 0,
                      boolean repeat = false);

  /*************************************************************************/
  /*!
   * @brief   Set timer event's interval.
   * @param   interval_ms - Interval in milliseconds
   */
  /*************************************************************************/
  void SetInterval(uint32_t interval_ms);

  /*************************************************************************/
  /*!
* @brief   Start the timer event. If interval wasn't set before,
              can be set here.
* @param   interval_ms - in milliseconds
*/
  /*************************************************************************/
  void Start(uint32_t interval_ms);
  /*!
   * @brief   Same as before, but without the interval.
   */
  void Start(void);

  /*************************************************************************/
  /*!
   * @brief  Stop the timer event.
   */
  /*************************************************************************/
  void Stop(void);

  /*************************************************************************/
  /*!
   * @brief  Restart a timer event by stopping and then starting it again.
   */
  /*************************************************************************/
  void Restart(void);
};
#endif // TIME_SERVER_H_
