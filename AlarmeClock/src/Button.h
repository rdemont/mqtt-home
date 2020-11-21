#ifndef Button_h
#define Button_h


#include <Arduino.h>


#define COUNT_FALLING 0
#define COUNT_RISING  1
#define COUNT_BOTH    2
#define LONG_PRESS    3000

class Button
{
    public:
        Button(int pin);
        void loop();
        int getState();
        bool isPressed();
		bool isReleased();
        bool isRelaseedShortPress();
        bool isRelaseedLongPress();
        void setDebounceTime(unsigned long time);
        int getStateRaw();
        void setCountMode(int mode);
		unsigned long getCount();
		void resetCount();

    private : 
        int _btnPin ;
		int _previousSteadyState;  // the previous steady state from the input pin, used to detect pressed and released event
		int _lastSteadyState;      // the last steady state from the input pin
		int _lastFlickerableState; // the last flickerable state from the input pin
		int _currentState;         // the current reading from the input pin
		unsigned long _debounceTime;
		unsigned long _count;
		int _countMode;        
        unsigned long _lastDebounceTime; // the last time the output pin was toggled
};

#endif