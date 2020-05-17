/***********************************************************************
 * @file       : Button_LED.ino
 * @author     : Fahad Mirza (fahadmirza8@gmail.com)
 * @brief      : Multiple GPIO example
 *
 * In this example we will check a button state every 250ms
 * and if the button is pressed D13 LED will be turned ON 
 * for 2s.
 * 
 * Button will be connected to pin D2 and will be active low.
 ***********************************************************************/

#include "time_server.h"


// Callback functions for timer events
static void onLedTimerEvent(void);
static void onButtonTimerEvent(void);

/**** Private Variables ******************************/
const uint8_t BUTTON_PIN = 2;

// TimerEvent(Callback, Interval_ms)
static TimerEvent LedTimer(onLedTimerEvent, 2000);
static TimerEvent ButtonTimer(onButtonTimerEvent, 250, true);



void setup() 
{
    Serial.begin(9600);
    pinMode(LED_BUILTIN, OUTPUT);

    // initialize the pushbutton pin as an input:
    pinMode(BUTTON_PIN, INPUT_PULLUP);
  
    ButtonTimer.Start();
}

void loop() 
{
    // Simulating a long work happening in the loop
    Serial.println("Hello");
    delay(5000);
}

void onLedTimerEvent(void)
{
    // Turn OFF the LED
    digitalWrite(LED_BUILTIN, LOW);
}

void onButtonTimerEvent(void)
{
    // Read the pushbutton value into a variable
    uint8_t buttonState = digitalRead(BUTTON_PIN);

    if (LOW == buttonState)
    {
        digitalWrite(LED_BUILTIN, HIGH);
        LedTimer.Start();   
    }

    /*
     * NOTE: For this example we don't need debounce.
     * Even if there are an oscillation dutring a button
     * press, LedTimer.Start() will only execute once.
     * Once a timer event is start working, you cannot
     * start it again. If your event is already started
     * and you call the Start() again it will check and
     * see it is already running and will return immediately.
     * 
     * In any case if you want restart an event after it is
     * already started, use Restart().
     * 
     */
}
