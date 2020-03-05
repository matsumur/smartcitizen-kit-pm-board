#include "wiring_private.h"
#include <Arduino.h>
#include <Wire.h>

#include <PM_driver.h>

#define I2C_ADDRESS 0x03

// Hardware Serial UART PM-A
Uart SerialPMA_A (&sercom2, RX_A, TX_A, SERCOM_RX_PAD_1, UART_TX_PAD_0);
void SERCOM2_Handler() {
	SerialPMA_A.IrqHandler();
}

// Hardware Serial UART PM-B (already setup by arduino core as Serial1 and renamed in headers file)

// Hardware Serial UART GroveUART
Uart SerialGrove (&sercom1, RX0, TX0, SERCOM_RX_PAD_3, UART_TX_PAD_2);
void SERCOM1_Handler() {
	SerialGrove.IrqHandler();
}

ClickSensor clicksensor;

volatile uint8_t wichCommand = 99;

void setup()
{

	// RGB led
	pinMode(pinRED, OUTPUT);
	pinMode(pinGREEN, OUTPUT);
	pinMode(pinBLUE, OUTPUT);

	digitalWrite(pinBLUE, LOW);
	digitalWrite(pinGREEN, HIGH);
	digitalWrite(pinRED, HIGH);

	// PM_A connector
	pinMode(ENABLE_PMS_A, OUTPUT);
	pinMode(POWER_PMS_A, OUTPUT);
	pinMode(RESET_PMS_A, OUTPUT);

	digitalWrite(POWER_PMS_A, LOW);
	digitalWrite(RESET_PMS_A, HIGH);

	pinMode(PWM_PMS_A, INPUT);

	// PM_B connector
	pinMode(ENABLE_PMS_B, OUTPUT);
	pinMode(POWER_PMS_B, OUTPUT);
	pinMode(RESET_PMS_B, OUTPUT);

	digitalWrite(POWER_PMS_B, LOW);
	digitalWrite(RESET_PMS_B, HIGH);

	pinMode(PWM_PMS_B, INPUT);

	// ADC'S
	pinMode(ADC0, INPUT);
	pinMode(ADC1, INPUT);
	pinMode(ADC2, INPUT);
	pinMode(ADC3, INPUT);

	// GPIO as input
	pinMode(GPIO0, INPUT);
	pinMode(GPIO1, INPUT);

	// Groove UART
	SerialGrove.begin(115200);
	pinPeripheral(RX0, PIO_SERCOM);
	pinPeripheral(TX0, PIO_SERCOM);

	Wire.begin(I2C_ADDRESS);
	Wire.onReceive(receiveEvent);
	Wire.onRequest(requestEvent);

#ifdef debug_PM
	SerialUSB.begin(115200);
	while (!SerialUSB);
	SerialUSB.println("Starting...");
#endif

	clicksensor.begin();
}

void receiveEvent(int howMany)
{
	byte command = 99;
	if (Wire.available()) command = Wire.read();

	switch(command) {
		case CLICK_START:
		{
			clicksensor.begin();
			wichCommand = command;
			break;
		}
		case CLICK_STOP:
		{
			clicksensor.stop();
			break;
		}
		case CLICK_GET:
		{
			wichCommand = command;
			break;
		}
	}
}

void requestEvent()
{

	switch (wichCommand)
	{
		case CLICK_START:
		{
#ifdef debug_PM
			SerialUSB.println("Starting Click sensor...");
#endif
			Wire.write(1);
			break;
		}
		case CLICK_GET:
		{
#ifdef debug_PM
			SerialUSB.println("Sending click values...");
#endif
			if (clicksensor.active){
				for (uint8_t i=0; i<valuesSize; i++) Wire.write(clicksensor.values[i]);
				clicksensor.buttonCount = 0;
			}else{
				for (uint8_t i=0; i<valuesSize; i++) Wire.write(255);
			}
			break;
		}
	}
}

void loop()
{
	clicksensor.update();
}
