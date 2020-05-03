#include "time_server.h"


static sTimerEvent_t BlinkTimer;

// Callback functions for timer events
static void onBlinkTimerEvent(void);

void setup() 
{
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  Timer_Init(&BlinkTimer, onBlinkTimerEvent);
  Timer_SetTime(&BlinkTimer, 500);
  Timer_Start(&BlinkTimer);

}

void loop() 
{
  Timer_Handler();
}

void onBlinkTimerEvent(void)
{
  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
}
