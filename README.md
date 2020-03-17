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