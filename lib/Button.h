#ifndef Button_h
#define Button_h


#include <Arduino.h>

#define LONG_PRESS 250


class Button
{
    public:
        Button(int pin);
        void loop();
        bool isPressed();
        bool isReleased();
        bool isReleasedShortPress();
        bool isReleasedLongPress();
        
    private : 
        int _btnPin ;
        int _lastState = HIGH;  // the previous state from the input pin
        int _currentState = HIGH;     // the current reading from the input pin
        unsigned long _pressedTime  = 0;
        unsigned long _releasedTime = 0;       
        bool _isPressed = false ; 
        bool _isReleased = false ; 
        bool _isReleasedShortPress = false ;  
        bool _isReleasedLongPress = false ; 
        
};

#endif