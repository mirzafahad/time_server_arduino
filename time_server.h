/***********************************************************************
 * @file       : time_server.h
 * @author     : Fahad Mirza (fahadmirza8@gmail.com)
 * @version    : V1.0
 * @brief      : Time server driver
 ***********************************************************************/

#ifndef TIME_SERVER_H_
#define TIME_SERVER_H_

#include <stdbool.h>

/*** Public Functions Declarations *************************************/


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


// Function pointer typedef. To use inside TimerEvent class
typedef void (*Callback)(void);

/**************************************************************************/
/*! 
    @brief  Class that stores state and functions for interacting with Time Server
*/
/**************************************************************************/
class TimerEvent
{
  public:
    uint32_t ElapsedTime_ms; // How much of Interval_ms is passed?
    uint32_t Interval_ms;    // Interval before executing callback()
    boolean  IsRunning;      // Is the timer currently running?
    boolean  Repeat;         // Is this event needs to be repeated
    Callback Cb;             // Timer event callback function
    
    /* 
     * Timer event initialization.
     * Param :   interval - Time interval for callback,
     *           cb       - Callback function pointer
     *           repeat   - Should the event repeat after executing callback
     * Return:   none
     */
    TimerEvent(Callback cb, uint32_t interval_ms = 0, boolean repeat = false);
    
    /*
     * Set timer event's interval
     * Param :   interval_ms - interval in milliseconds
     * Return:   none
     */
    void setInterval(uint32_t interval_ms);

    /* 
     * Start a particular timer event.
     * Param :   interval_ms - in milliseconds
     * Return:   none
     */
    void start(uint32_t interval_ms);
    void start(void);

    /* 
     * Stop the timer event and remove from the linked list.
     * Param :   none
     * Return:   none
     */
    void stop(void);

    /*
     * Restart a timer event by stopping the timer event and 
     * then starting it again
     * Param :   none
     * Return:   none
     */
    void restart(void);
};
#endif // TIME_SERVER_H_
