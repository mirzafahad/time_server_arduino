#include "time_server.h"


// Callback functions for timer events
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
  
    LedTimer.Start();
    D8Timer.Start();
    D9Timer.Start();
}

void loop() 
{
    // Simulating a long work happening in the loop
    Serial.println("Hello");
    delay(5000);
}

void onLedTimerEvent(void)
{
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
}

void onD8TimerEvent(void)
{
  digitalWrite(8, !digitalRead(8));
}

void onD9TimerEvent(void)
{
  digitalWrite(9, !digitalRead(9));
}
