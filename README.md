# A Time Server Library for Arduino Uno
---
This time server doesn't represent the traditional [time server](https://en.wikipedia.org/wiki/Time_server) that you use through a computer network. This time server takes care of all your time-sensitive piece-of-code. You give the Time Server a function in the form of callback function and tell it to execute within a certain time, periodically, or one shot, and it will take care of it.

#### Under-the-hood
Time Server uses ATMega328’s Timer 1. That timer generates an interrupt every 1ms. So, if you provide a callback to the time server and asked the server to execute it every 500ms, timer ISR will subtract 1ms from callback’s “Interval” until it reaches zero. If you have more than one TimerEvent, the time server will keep track of all using Linked List.
Checkout examples to learn how to utilize the library. A blog about it can be found [here]( https://mirzafahad.github.io/2020-04-30-time-server/).

#### Caution
As this library uses Timer 1, you cannot use the **Servo** library and **analogWrite()** on pins 9 and 10.

## Installation
1.	Download the repo or [click here]( https://github.com/mirzafahad/Time_Server_Arduino/archive/master.zip).
2.	Uncompress the ZIP file after it’s finished downloading.
3.	The resulting folder should contain the files **time\_server.cpp**, **time\_server.h**, and an **“examples”** sub-folder. Sometimes in Windows you’ll get an intermediate-level folder and need to move things around.
4.	Rename the folder (containing the .cpp and .h files) to TimeServer, and place it alongside your other Arduino libraries, typically in your (home folder)/Documents/Arduino/Libraries folder. Libraries should never be installed in the “Libraries” folder alongside the Arduino application itself…put them in the subdirectory of your home folder.
5.	Re-start the Arduino IDE if it’s currently running.

[Here’s a tutorial]( https://learn.adafruit.com/adafruit-all-about-arduino-libraries-install-use/how-to-install-a-library) that walks through the process of correctly installing Arduino libraries manually.


## Features
* Simple to use  
One time server to control all your timer needs. All the complexities are abstracted so that you can focus on the fun and interesting bits.  
* Supported Chipsets  
Currently the library support only ATMega328 i.e. Arduino Uno, Nano, and Pro (5V, 16MHz).


## Functions
* SetInterval ()
* Start ()
* Stop ()
* Restart()

## Examples
There are several examples available in the examples folder. One of the simple examples is below.

```

	#include "time_server.h"
	
	// Callback functions for timer events
	static void onBlinkTimerEvent(void);
	
	// TimerEvent(Callback, interval_ms, repeat)
	// Repeat the event every 250ms
	static TimerEvent BlinkTimer(onBlinkTimerEvent, 250, true);
	
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
```