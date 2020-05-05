#include "time_server.h"


// Callback functions for timer events
static void onBlinkTimerEvent(void);
static TimerEvent BlinkTimer(500, onBlinkTimerEvent, true);

void setup() 
{
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);

  BlinkTimer.start();
}

void loop() 
{
  Timer_Handler();
}

void onBlinkTimerEvent(void)
{
  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
}
