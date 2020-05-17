/***********************************************************************
 * @file       : Multi_Gpio.ino
 * @author     : Fahad Mirza (fahadmirza8@gmail.com)
 * @brief      : Multiple GPIO example
 *
 * In this example we will change the state of three pins, 
 * periodically, using the time server library.
 * 
 * In the Simple_Blink example we saw how to change state of
 * D13/on-board LED using the library. We will expand on that
 * and add two more pins D8 and D9.
 * 
 * You can hook up two LED on D8 and D9 or just simply use a
 * logic analyzer/ oscilloscope to see the changes.
 ***********************************************************************/
#include "time_server.h"


// Callback declarations for timer events
static void onLedTimerEvent(void);
static void onD8TimerEvent(void);
static void onD9TimerEvent(void);

// TimerEvent(Callback, interval_ms, repeat)
static TimerEvent LedTimer(onLedTimerEvent, 250, true);
static TimerEvent D8Timer(onD8TimerEvent, 500, true);
static TimerEvent D9Timer(onD9TimerEvent, 1000, true);


void setup() 
{
    Serial.begin(9600);
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(8, OUTPUT);
    pinMode(9, OUTPUT);

    // Start the timer events
    LedTimer.Start();
    D8Timer.Start();
    D9Timer.Start();
}

void loop() 
{
    // Simulating a time consuming piece-of-code in the loop
    Serial.println("Hello");
    delay(5000);
}

/**** Callbacks Definitions **********************************/
void onLedTimerEvent(void)
{
    // Toggle the pin state
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
}

void onD8TimerEvent(void)
{
    // Toggle the pin state
    digitalWrite(8, !digitalRead(8));
}

void onD9TimerEvent(void)
{
    // Toggle the pin state
    digitalWrite(9, !digitalRead(9));
}
