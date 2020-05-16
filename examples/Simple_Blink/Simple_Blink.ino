#include "time_server.h"


// Callback functions for timer events
static void onBlinkTimerEvent(void);


// TimerEvent(Callback, interval_ms, repeat)
// Repeat the event every 250ms
static TimerEvent BlinkTimer(onBlinkTimerEvent, 500, true);


void setup() 
{
    pinMode(LED_BUILTIN, OUTPUT);
    BlinkTimer.Start();
}

void loop() 
{
}

void onBlinkTimerEvent(void)
{
  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
}
