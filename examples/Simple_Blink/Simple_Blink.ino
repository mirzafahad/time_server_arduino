/***********************************************************************
 * @file       : Simple_Blink.ino
 * @author     : Fahad Mirza (fahadmirza8@gmail.com)
 * @brief      : A Simple Timer Server Example
 *               This example shows how to blink an LED every
 *               250ms using the time server library.
 ***********************************************************************/
 
#include "time_server.h"


// Callback functions for LED timer event
static void onBlinkTimerEvent(void);


// TimerEvent(Callback, interval_ms, repeat)
// Repeat the event every 250ms
static TimerEvent BlinkTimer(onBlinkTimerEvent, 250, true); 

void setup() 
{
    // Set the D13 pin as output. Arduino on-board LED is connected to D13.
    pinMode(LED_BUILTIN, OUTPUT);

    // Start the timer event
    BlinkTimer.Start();
}

void loop() 
{
    // Simulating a time consuming application
    delay(5000);
}

void onBlinkTimerEvent(void)
{
    // Toggle the LED state
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
}
