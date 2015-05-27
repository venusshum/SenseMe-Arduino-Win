/**
* \addtogroup SenseMeButton
*
* This is the driver code for the button on the SenseMe. This
* uses an ISR to debounce the button - it uses edges rather than
* the value of the digital pin.
* 
* The debounce time can be set in the begin routine, but defaults
* to the DEBOUNCE_DELAY value defined in the header.
*
* @{
*/

/**
* \file 
* 		SenseMe Button driver
* \author
* 		Venus Shum
*/

#include "pins_arduino.h"
#include "SenseMeButton.h"



/*---------------------------------------------------------------------------*/
/**
* \brief Constructor
* 
* C++ constructor for this class. Empty.
*/
SenseMeButtonClass::SenseMeButtonClass()
{
}

/*---------------------------------------------------------------------------*/
/**
* \brief begin function - must be called before using other functions
*
* This function enables the button. It can be called with a parameter that
* represents the debounce delay, but this defaults to a sensible value if it
* is not provided.
*  
*/
void SenseMeButtonClass::begin(long debounceDelay) 
{	
	lastChangeTime   = 0;   	// The last time the button input changed
	buttonState      = false; 	// Whether the button is currently pressed
	fallingEdge      = true;	// Whether we're looking for a falling or rising edge in the ISR
	wasPressedState  = false;	// Whether the button has been pressed since we last checked
	wasReleasedState = false;	// Whether the button has been released since we last checked
	
	
	// Store the given debounce delay
	//
	debounceDelayTime = debounceDelay;
	
	// Set the button to be an input
	//
	pinMode(BUTTON, INPUT);
	
	// The button is connected to PE6/INT.6
	//
	// And set up for an interrupt on INT.6 to be taken on either a falling
	// (pressed) or rising (released) edge, since we allow users to watch
	// either. 
	//
	cli();
	EICRB &= ~((1<<ISC61) | (1<<ISC60)); 	// Clear bits
	EICRB |= (1<<ISC61); 					// And set to interrupt on a falling transition
	EIMSK |= (1<<INT6); 					// External pin interrupt enable.
	sei();

	// Reset the wasPressed/wasReleased state
	//
	reset();
}


/*---------------------------------------------------------------------------*/
/**
* \brief end function - switch off the button
*
* Switch off the interrupt and reset state.
* 
*/
void SenseMeButtonClass::end() 
{
	// Disable the button pin interrupt
	//
	EIMSK &= ~(1<<INT6); 					// External pin interrupt disable.

	// Reset the wasPressed/wasReleased state
	//
	reset();	
}

/*---------------------------------------------------------------------------*/
/**
* \brief Check to see if the button is pressed.
*
* Check to see if the button is pressed. If it is not pressed, it is, by
* definition, released.
*
*/
bool SenseMeButtonClass::isPressed()
{	
	// The button state when it was pressed or released will have been set
	// in the ISR.
	//
	return buttonState;
}

/*---------------------------------------------------------------------------*/
/**
* \brief Wait until the button is pressed. Blocking call.
*
* Just loops indefinitely, polling the button state until the button is
* pressed. The event we wait for is the button going down. If the button is
* currently pressed, we wait for it to be released and then wait for it to
* be pressed again
*
*/
void SenseMeButtonClass::waitUntilPressed()
{	
	// If the button is pressed, first wait for it to be
	// released so we get the edge of it going down
	//
	if (isPressed())
		waitUntilReleased();
	
	while (!isPressed());
}

/*---------------------------------------------------------------------------*/
/**
* \brief Wait until the button is released. Blocking call.
*
* Just loops indefinitely, polling the button state until the button is
* released. The event we wait for is the button going up. If the button is
* currently released, we wait for it to be pressed and then wait for it to
* be released again
*
*/
void SenseMeButtonClass::waitUntilReleased()
{	
	// If the button is not pressed, first wait for it
	// to be pressed so we get the edge of it going up
	//
	if (!isPressed())
		waitUntilPressed();
	
	while (isPressed());
}


/*---------------------------------------------------------------------------*/
/**
* \brief Check to see if the button was pressed since our last check.
* \return Whether or not is has been pressed since we last checked.
*
* This function checks to see if the button has been pressed in some
* intervening period when we were not checking it actively. On each check
* we return a boolean that shows whether the button has been pressed, and 
* we reset the state so one can look for a new button press at some later
* point.
*
*/
bool SenseMeButtonClass::wasPressed()
{   bool rv = wasPressedState;

	wasPressedState = false;
	return(rv);
}


/*---------------------------------------------------------------------------*/
/**
* \brief Check to see if the button was released since our last check.
* \return Whether or not is has been released since we last checked.
*
* This function checks to see if the button has been released in some
* intervening period when we were not checking it actively. On each check
* we return a boolean that shows whether the button has been released, and 
* we reset the state so one can look for a new button release at some later
* point.
*
*/
bool SenseMeButtonClass::wasReleased()
{	bool rv = wasReleasedState;

	wasReleasedState = false;
	return(rv);
}


/*---------------------------------------------------------------------------*/
/**
* \brief Reset the wasPressed/wasReleased state.
*
* This function resets the wasPressed/wasReleased state to false - it can
* be used to start a period of looking for a button press, ignoring what
* happened before reset was called. Note, however, that calling wasPressed
* or wasReleased also resets the appropriate part of the state.
*
*/
void SenseMeButtonClass::reset()
{	
	wasPressedState  = false;	
	wasReleasedState = false;
}


/*---------------------------------------------------------------------------*/
/**
* \brief ISR for INT.6 on the ATMega32U4. This is connected to the button.
*
* This ISR is set to go off on either edge, but we change which edge
* depending on whether we last saw a falling edge or a rising edge. This
* seems overkill, but trying to implement this ISR to react to a change of
* any description doesn't work well, because we can't tell which direction
* the change is in.
* Given we have an edge, set the state of the button and whether it was
* pressed or released, but only if the switch isn't bouncing. We determine
* whether it is bouncing by looking at how long it's been stable for; if this
* is long enough, then we regard this as being the first edge (button not
* bouncing) and change state. If it is not, we simply record the current
* time as the time of the last change.
* 
*/
ISR(INT6_vect)
{	
	if (SenseMeButton.fallingEdge) {
		if ((millis() - SenseMeButton.lastChangeTime) > SenseMeButton.debounceDelayTime) {
			// We have a falling edge and we're not bouncing
			SenseMeButton.buttonState      = true;
			SenseMeButton.wasPressedState  = true;
		}
		if (SenseMeButton.buttonState) {
			// Now we should look for a non-bouncing rising edge
			SenseMeButton.fallingEdge = false;
			EICRB |= ((1<<ISC61) | (1<<ISC60)); 	// Set to interrupt on rising transition		
		}
	}
	else {
		if ((millis() - SenseMeButton.lastChangeTime) > SenseMeButton.debounceDelayTime) {
			// We have a rising edge and we're not bouncing
			SenseMeButton.buttonState 	 = false;
			SenseMeButton.wasReleasedState = true;
		}
		if (!SenseMeButton.buttonState) {
			// Now we should look for a non-bouncing falling edge
			SenseMeButton.fallingEdge = true;
			EICRB &= ~((1<<ISC61) | (1<<ISC60)); 	// Clear bits
			EICRB |= (1<<ISC61); 					// And set to interrupt on falling transition
		}
	}
	
	// This is a change, so we record that fact.
	//
	SenseMeButton.lastChangeTime = millis();
}


/*---------------------------------------------------------------------------*/
/*
 * Preinstantiate Objects
 */ 
SenseMeButtonClass SenseMeButton = SenseMeButtonClass();

/** @} */
