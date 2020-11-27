


#include <Arduino.h>


#include <Button.h>

Button::Button(int pin) 
{
    Serial.println("Button : "+pin);
    pinMode(pin, INPUT_PULLUP);
	_btnPin = pin;

}

    bool Button::isPressed()
    {
        return _isPressed;
    }
    bool Button::isReleased()
    {
        return _isReleased;
    }
    bool Button::isReleasedShortPress()
    {
        return _isReleasedShortPress;
    }
    bool Button::isReleasedLongPress()
    {
        return _isReleasedLongPress;
    }

void Button::loop() {
    //setup default 
    if (!_isPressed)
    {
        _isReleased = false ; 
        _isReleasedShortPress = false ; 
        _isReleasedLongPress = false ; 
    }

    // read the state of the switch/button:
    _currentState = digitalRead(_btnPin);
    if(_lastState == HIGH && _currentState == LOW)        // button is pressed
    {
        _isPressed = true ; 
        _isReleased = false ; 
        _isReleasedShortPress = false ; 
        _isReleasedLongPress = false ; 

        _pressedTime = millis();
    }else if(_lastState == LOW && _currentState == HIGH)  // button is released
    {
        _isPressed = false ; 
        _isReleased = true ; 
        _releasedTime = millis();
        long pressDuration = _releasedTime - _pressedTime;

        if(pressDuration < LONG_PRESS )
        {
            _isReleasedShortPress = true;
        }else {
            _isReleasedLongPress = true ;
        }
    }
    // save the the last state
    _lastState = _currentState;
}

