__For the instruction of the Smart Citizen Kit 2.0 PM Board, please refer to [the repository](https://github.com/fablabbcn/smartcitizen-kit-pm-board).__
## A Guide to making an external sensor for SCK 2.1
[![PRs Welcome](https://img.shields.io/badge/PRs-welcome-brightgreen.svg)]()

In this guide, we will make an external sensor for SCK 2.1. We use an auxiliary port on SCK 2.1 and connect an external sensor. Since the auxiliary port works as an I2C master by default, we will build the external sensor as an I2C slave. 

![Alt](https://raw.githubusercontent.com/matsumur/smartcitizen-kit-pm-board/master/externalsensor.png "External Sensor")

You may use any sensors that uses the I2C communication. The only restriction is the I2C address. The address should not be used already. The complete definition of the I2C addresses used in SCK 2.1 can be found on the [code](https://github.com/fablabbcn/smartcitizen-kit-21/blob/f9d7513/sam/src/SckAux.h#L59-L134).

In this guide, we will use an Arduino device, [the Smart Citizen Kit 2.0 PM Board](https://github.com/fablabbcn/smartcitizen-kit-pm-board), as an I2C slave device. The board is compatible to [Arduino Zero](https://store.arduino.cc/arduino-zero) and has capability to act as I2C slave. Many Arduino devices can act as I2C slave using the [Wire library](https://www.arduino.cc/en/Reference/Wire). So you may use your own Arduino compatible devices for making your external sensor.

## A Button Counting Sensor 
In this guide, we will develop a simple button counter as an external sensor as an example. The counting sensor counts how many times is the button clicked in a certain period. We use mainly three devices in this project as follows:
1. a [Smart Citizen Kit 2.1](https://github.com/fablabbcn/smartcitizen-kit-21/)
2. a [Smart Citizen Kit 2.0 PM Board](https://github.com/fablabbcn/smartcitizen-kit-pm-board)
3. a [Grove Button](https://www.seeedstudio.com/Grove-Button-p-2809.html)

![Alt](https://raw.githubusercontent.com/matsumur/smartcitizen-kit-pm-board/master/connection.png "External Sensor")
These three devices will be connected as shown in the figure above. 


## Fork SCK2.1 github repository. 
Go to https://github.com/fablabbcn/smartcitizen-kit-21 and find Fork button on the top right of the webpage. The forking process requires only a few minutes. 

After forking the repository, you can see your forked project on the browser. Then you can clone the project. Click the Clone or download button and copy the location of the project. 

You open your terminal and change your current directory. Then you paste the location after the command `git clone --recursive`. For example, in my case the complete command is like `git clone --recursive git@github.com:matsumur/smartcitizen-kit-21.git`. Note that the project repository includes submodule; Do not forget `--recursive` option when cloning your repository. 

## Code your sensor
Making an external sensor requires two parts of the implementation:
1. Code for SCK 2.1
2. Code for your sensor (I2C slave device)

### Code for SCK 2.1
We firstly add several lines in the forked repository of SCK2.1.  You need to modify these three files listed below:
1. lib/Sensors/Sensors.h
2. sam/src/SckAux.h
3. sam/src/SckAux.cpp

Let's start coding :) 

#### lib/Sensors/Sensors.h
lib/Sensors/Sensors.h has a list of all sensors working with SCK2.1. You may find `enum SensorType` in the file. In this guide, we add a click counter as a new SensorType as `SENSOR_CLICK` in Sensor.h. Add a line just before the definition `SENSOR_COUNT`. The `SENSOR_COUNT` should always be placed on the last line because it counts the number of sensors.

We then add a definition of the new sensor as an instance of the class `OneSensor`. All sensor should be an instance of the `OneSensor` class. `OneSensor` class requires ten parameters to create an instance. 

We define our `SENSOR_CLICK` sensor with following parameters. 

|  SensorLocation  |  priority  | SensorType | shortTitle |     title     | id  | enabled | controllable | everyNintervals | unit |
| ---- | ---- | ---- | ---- | ---- | ---- | ---- | ---- | ---- | ---- |
|  BOARD_AUX  |  100  |  SENSOR_CLICK  |  "CLICK"  |  "Click Count"  |  121  |  false  |  false  |  1  |  "Clicks"  |

- `SensorLocation` will be `BOARD_AUX` because we use AUX connecter on the SCK board. 
- `SensorType` should match  the definition on `enum SensorType`. In this time, it will be `SENSOR_CLICK`.
- `id` will be provided from the administrator of smartcitizen.me platform. You will need to ask them with your blueprint (c.f. https://docs.smartcitizen.me/Guides/Third%20party%20sensors/#publishing-data-using-custom-devices).
-  `enabled` will be `false` because your sensor will not be attached with the SCK board by default. 
- If your device has function to calibrate any parameters and/or has any actuators to control, `controllable` should be `true`. Since our sensor has no functionality, it will be `false`.
- `everyNintervals` defines the interval of fetching data from your sensor. At this time, the SCK fetches and send data every single minute. If you set `everyNintervals` to 10, your sensor data will be fetched every 10 minutes. 

We add the following line for our `SENSOR_CLICK` just before the definition of `SENSOR_COUNT`. The order of the list should be as same as the `enum SensorType`. 

`OneSensor { BOARD_AUX, 100, SENSOR_CLICK, "CLICK", "Click Count", 121, false, false, 1, "Clicks"},`

```diff
diff --git a/lib/Sensors/Sensors.h b/lib/Sensors/Sensors.h
index 5398378..c84b8e6 100644
--- a/lib/Sensors/Sensors.h
+++ b/lib/Sensors/Sensors.h
@@ -115,6 +115,10 @@ enum SensorType
 	// Actuators (This is temp)
 	SENSOR_GROOVE_OLED,
 
+	SENSOR_CLICK,
+	// Add New Sensor Here!!!
+
+	//SENSOR_COUNT should always be placed on the last line.
 	SENSOR_COUNT
 };
 
@@ -273,10 +277,14 @@ class AllSensors
 			// Later this will be moved to a Actuators.h file
 			// Groove I2C Oled Display 96x96
 			OneSensor { BOARD_AUX,		250,	SENSOR_GROOVE_OLED,			"GR_OLED",		"Groove OLED",					0,		false,		false,		1,			},
-			OneSensor { BOARD_BASE, 	0,	SENSOR_COUNT,				"NOT_FOUND",		"Not found",					0,		false,		false,		1,			}
+			
+			OneSensor { BOARD_AUX,		100,	SENSOR_CLICK,				"CLICK",		"Click Count",					121,		false,		false,		1,			"Clicks"},
 
 			// Add New Sensor Here!!!
 
+			OneSensor { BOARD_BASE, 	0,	SENSOR_COUNT,				"NOT_FOUND",		"Not found",					0,		false,		false,		1,			}
+			
+
 		};
 
 		OneSensor & operator[](SensorType type) {
```

#### sam/src/SckAux.h
The AUX connecter is controlled by the MCU SAMD21. So, the target codes are in the sam/src directory in the repository. 

**I2C address Definition** We firstly add a definition of I2C address in the list of `byte devAddress[]`. As all I2C address used in SCK are listed in the array, all you need is to find an address that is not in use on SCK. This time, we use `0x03` as the I2C address for our sensor. We add a line `0x03		// SENSOR_CLICK,` in the array `byte devAddress[]`.



```diff
diff --git a/sam/src/SckAux.h b/sam/src/SckAux.h
index 73301d5..5716efd 100644
--- a/sam/src/SckAux.h
+++ b/sam/src/SckAux.h
@@ -130,7 +130,9 @@ class AuxBoards
 			0x77,			// SENSOR_BME680_PRESSURE,
 			0x77,			// SENSOR_BME680_VOCS,
 
-			0x3c		// SENSOR_GROOVE_OLED,
+			0x3c,		// SENSOR_GROOVE_OLED,
+
+			0x03		// SENSOR_CLICK,
 		};
 
 		bool start(SensorType wichSensor);
@@ -555,5 +557,26 @@ class Sck_BME680
 		Adafruit_BME680 bme;
 };
 
+class Click
+{
+	public:
+		byte deviceAddress = 0x03;
+		uint16_t count;
+		bool start();
+		bool stop();
+		bool getReading();
+	private:
+		bool started = false;
+		bool failed = false;
+		
+		static const uint8_t valuesSize = 18;
+		uint8_t values[valuesSize];
+		enum Clickcommands{
+			CLICK_START,
+			CLICK_STOP,
+			CLICK_GET
+		};
+};
+
 void writeI2C(byte deviceAddress, byte instruction, byte data);
 byte readI2C(byte deviceAddress, byte instruction);
```

#### sam/src/SckAux.cpp

```diff
diff --git a/sam/src/SckAux.cpp b/sam/src/SckAux.cpp
index b1e2fc7..e4ed9a7 100644
--- a/sam/src/SckAux.cpp
+++ b/sam/src/SckAux.cpp
@@ -16,6 +16,7 @@ Sck_DallasTemp 		dallasTemp;
 Sck_SHT31 		sht31 = Sck_SHT31(&auxWire);
 Sck_Range 		range;
 Sck_BME680 		bme680;
+Click click;
 
 // Eeprom flash emulation to store I2C address
 FlashStorage(eepromAuxData, EepromAuxData);
@@ -100,6 +101,7 @@ bool AuxBoards::start(SensorType wichSensor)
 		case SENSOR_BME680_HUMIDITY:		return bme680.start(); break;
 		case SENSOR_BME680_PRESSURE:		return bme680.start(); break;
 		case SENSOR_BME680_VOCS:		return bme680.start(); break;
+		case SENSOR_CLICK: return click.start(); break;
 		default: break;
 	}
 
@@ -170,6 +172,7 @@ bool AuxBoards::stop(SensorType wichSensor)
 		case SENSOR_BME680_HUMIDITY:		return bme680.stop(); break;
 		case SENSOR_BME680_PRESSURE:		return bme680.stop(); break;
 		case SENSOR_BME680_VOCS:		return bme680.stop(); break;
+		case SENSOR_CLICK: return click.stop(); break;
 		default: break;
 	}
 
@@ -241,6 +244,8 @@ void AuxBoards::getReading(OneSensor *wichSensor)
 		case SENSOR_BME680_HUMIDITY:		if (bme680.getReading()) 			{ wichSensor->reading = String(bme680.humidity); return; } break;
 		case SENSOR_BME680_PRESSURE:		if (bme680.getReading()) 			{ wichSensor->reading = String(bme680.pressure); return; } break;
 		case SENSOR_BME680_VOCS:		if (bme680.getReading()) 			{ wichSensor->reading = String(bme680.VOCgas); return; } break;
+		case SENSOR_CLICK: if (click.getReading()) 			{ 
+		wichSensor->reading = String(click.count); return; } break;
 		default: break;
 	}
 
@@ -1288,6 +1293,56 @@ bool Sck_BME680::getReading()
 	return true;
 }
 
+bool Click::start()
+{
+	if (started) return true;
+
+	if (!I2Cdetect(&auxWire, deviceAddress)) return false;
+
+	auxWire.beginTransmission(deviceAddress);
+	auxWire.write(CLICK_START);
+	auxWire.endTransmission();
+	auxWire.requestFrom(deviceAddress, 1);
+
+	bool result = auxWire.read();
+
+	if (result == 0) failed = true;
+	else started = true;
+
+	return result;
+}
+
+bool Click::stop()
+{
+	if (!I2Cdetect(&auxWire, deviceAddress)) return false;
+
+	auxWire.beginTransmission(deviceAddress);
+	auxWire.write(CLICK_STOP);
+	auxWire.endTransmission();
+
+	started = false;
+	return true;
+}
+
+bool Click::getReading()
+{
+	auxWire.beginTransmission(deviceAddress);
+	auxWire.write(CLICK_GET);
+	auxWire.endTransmission();
+
+	// Get the reading
+	auxWire.requestFrom(deviceAddress, valuesSize);
+	uint32_t start = millis();
+	while (!auxWire.available()) if ((millis() - start)>500) return false;
+
+	for (uint8_t i=0; i<valuesSize; i++) {
+		values[i] = auxWire.read();
+	}
+	count = (values[0]<<8) + values[1];
+
+	return true;
+}
+
 void writeI2C(byte deviceaddress, byte instruction, byte data )
 {
   auxWire.beginTransmission(deviceaddress);
```