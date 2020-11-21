


#include <Arduino.h>


#include <Button.h>

Button::Button(int pin) 
{
	_btnPin = pin;
	_debounceTime = 0;
	_count = 0;
	_countMode = COUNT_FALLING;

	pinMode(_btnPin, INPUT_PULLUP);

	_previousSteadyState = digitalRead(_btnPin);
	_lastSteadyState = digitalRead(_btnPin);
	_lastFlickerableState = digitalRead(_btnPin);

	_lastDebounceTime = 0;
}



void Button::setDebounceTime(unsigned long time) {
	_debounceTime = time;
}

int Button::getState() {
	return _lastSteadyState;
}

int Button::getStateRaw() {
	return digitalRead(_btnPin);
}

bool Button::isPressed() {
	if(_previousSteadyState == HIGH && _lastSteadyState == LOW)
		return true;
	else
		return false;
}

bool Button::isReleased() {
	if(_previousSteadyState == LOW && _lastSteadyState == HIGH)
		return true;
	else
		return false;
}

bool Button::isRelaseedShortPress()
{
    return (isReleased() && ((_lastDebounceTime - millis()) < LONG_PRESS ));
}

bool Button::isRelaseedLongPress()
{
    return (isReleased() && ((_lastDebounceTime - millis()) >= LONG_PRESS ));
}

void Button::setCountMode(int mode) {
	_countMode = mode;
}

unsigned long Button::getCount() {
	return _count;
}

void Button::resetCount() {
	_count = 0;
}

void Button::loop() {
	// read the state of the switch/button:
	_currentState = digitalRead(_btnPin);

	// check to see if you just pressed the button
	// (i.e. the input went from LOW to HIGH), and you've waited long enough
	// since the last press to ignore any noise:

	// If the switch/button changed, due to noise or pressing:
	if (_currentState != _lastFlickerableState) {
		// reset the debouncing timer
		_lastDebounceTime = millis();
		// save the the last flickerable state
		_lastFlickerableState = _currentState;
	}

	if ((millis() - _lastDebounceTime) >= _debounceTime) {
		// whatever the reading is at, it's been there for longer than the debounce
		// delay, so take it as the actual current state:

		// save the the steady state
		_previousSteadyState = _lastSteadyState;
		_lastSteadyState = _currentState;
	}

	if(_previousSteadyState != _lastSteadyState){
		if(_countMode == COUNT_BOTH)
			_count++;
		else if(_countMode == COUNT_FALLING){
			if(_previousSteadyState == HIGH && _lastSteadyState == LOW)
				_count++;
		}
		else if(_countMode == COUNT_RISING){
			if(_previousSteadyState == LOW && _lastSteadyState == HIGH)
				_count++;
		}
	}
}

