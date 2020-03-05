#include "PM_driver.h"

bool ClickSensor::begin(){
	active = true;
	return true;
}
bool ClickSensor::stop(){
	active = false;
	return true;
}
void ClickSensor::reset(){
	currentButtonState = false;
	lastButtonState = false;
	buttonCount = 0;
}
bool ClickSensor::update(){
	if(!active){
		return false;
	}else{
		//read button state
		currentButtonState = digitalRead(GPIO0);

		//check if button state changed
		if(lastButtonState != currentButtonState){
			// check if button released
			if(currentButtonState){
				buttonCount++;
				// Prepare values for I2C retransmission
				values[0] = buttonCount >> 8;
				values[1] = buttonCount & 0x00FF;

		#ifdef debug_PM
					SerialUSB.println("button: " + String(buttonCount));
		#endif
			}
			lastButtonState = currentButtonState;
		}
	}
	return true;
}
