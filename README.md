# Senior Capstone Project: Wearable Alert Device
University of Oklahoma

School of Electrical and Computer Engineering

Semester: Spring 2026

<ins>Team Members:<ins>

Project Manager - Jonas Wollman

Hardware Engineer - Ryan Kiesau

Hardware Engineer - Cody Chan

Software Engineer - Alex Ho

README written by Alex Ho

# Overview
This repository contains all project files pertaining to the Wearable Alert Device project. The following information
provides guidance for setting up the software development environment as well as a general description of each
of the major components of the software.

# Table of Contents
1. [Downloading Arduino IDE](#downloading-arduino-ide)
2. [Initial Environment Setup](#initial-environment-setup)
   - [Arduino Nano ESP32 Board Package Installation](#arduino-nano-esp32-board-package-installation)
   - [Pin Numbering Setting Configuration](#pin-numbering-setting-configuration)
   - [Required Library Dependencies](#required-library-dependencies)
   - [Verifying and Uploading Code to a Microcontroller](#verifying-and-uploading-code-to-a-microcontroller)
4. [Software Development](#software-development)
   - [WearableAlertDevice.ino](#wearablealertdeviceino)
   - [config.h](#configh)
   - [WirelessDataTransmission.h/.cpp](#wirelessdatatransmissionhcpp)
   - [GunshotDetection.h/.cpp](#gunshotdetectionhcpp)

# Downloading Arduino IDE
For this Capstone project, the Arduino IDE was used as the software development environment.
The Arduino IDE can be downloaded [here](https://www.arduino.cc/en/software/). Ensure that 
some version of Arduino IDE 2.0 is downloaded, not the legacy Arduino IDE 1.0. 

Back to [Table of Contents](#table-of-contents)

# Initial Environment Setup
Once the Arduino IDE has been installed, some initial setup in its environment is needed before code can be correctly
uploaded into the Arduino Nano ESP32 microcontroller. There are three important parts to the setup: installing the Arduino
Nano ESP32 board package, setting the pin numbering to GPIO numbers, and installing library dependencies. 

Back to [Table of Contents](#table-of-contents)

## Arduino Nano ESP32 Board Package Installation
The first stage of the environment setup is installing the proper board package so that the environment can correctly compile
code and upload programs for Arduino Nano ESP32 microcontrollers. In the Arduino IDE, look on the left for the Boards Manager 
tab. Click on the Boards Manager tab to open it up.

![Screenshot of Boards Manager tab in the Arduino IDE (circled in red).](./readme_images/boards-manager-tab.png)

Screenshot of Boards Manager tab in the Arduino IDE (circled in red).

In the search bar, type "Arduino ESP32 Boards." This package will contain the settings to recognize Arduino Nano ESP32
hardware. Select the Install button if the package is not already installed.

![Screenshot of the Arduino ESP32 Boards package not installed yet.](./readme_images/esp32-board-package.png)

Screenshot of the Arduino ESP32 Boards package not installed yet.

Once the package has been installed, the Arduino IDE will automatically install all the common software libraries associated with 
ESP32 microcontrollers, such as WiFi, Bluetooth, etc. No additional steps are needed to install these libraries.

Back to [Table of Contents](#table-of-contents)

## Pin Numbering Setting Configuration
The software developed for this project has been written specifically to use the Arduino Nano ESP32's GPIO numbers, not the 
standard Arduino pin numbers that are labeled on the physical device. **NOTE: IF THE PIN NUMBERING IS SET TO ARDUINO PIN NUMBERS 
INSTEAD OF GPIO NUMBERS, THERE WILL BE A GOOD CHANCE THAT INCORRECT PINS ON THE MICROCONTROLLER WILL BE USED BY THE SOFTWARE,
WHICH WILL MOST LIKELY PREVENT THE HARDWARE FROM FUNCTIONING PROPERLY.**

To change the pin numbering setting in the Arduino IDE, navigate to `Tools >> Pin Numbering` at the ribbon near the top of the IDE. 
In the `Pin Numbering` setting, there will be two options: `By Arduino pin (default)` and `By GPIO number (legacy)`. Set the `Pin Numbering`
setting to the `By GPIO number (legacy)` option.

![Pin Numbering setting](./readme_images/gpio-pin-numbering.png)

Screenshot of Pin Numbering setting being set to the "By GPIO number (legacy)" option. The Pin Numbering setting can be found in the
Tools tab in the ribbon near the top of the IDE.

Back to [Table of Contents](#table-of-contents)

## Required Library Dependencies
There are a few open-source libraries that the software relies on for its functionality. These external libraries are not automatically
installed by the Arduino IDE and will have to be installed manually. Below is a list of the open-source libraries:

1. [DFRobot_GNSS](https://github.com/DFRobot/DFRobot_GNSS) by DFRobot
2. [arduinoFFT](https://github.com/kosme/arduinoFFT) by kosme

The libraries can be downloaded by using the GitHub links above, or they can also be found in the `Software/dependencies` folder for
this project.

Once these libraries are downloaded, they need to be installed manually into the Arduino IDE. Navigate to `Sketch >> Include Library >> 
Add .ZIP Library...` at the ribbon near the top of the IDE. After clicking the `Add .ZIP Library` option, find the local `.zip` files 
of the libraries and add them to the Arduino IDE. 

![Add .ZIP Library](./readme_images/add-zip-library.png)

Screenshot of the navigation to the "Add .ZIP Library..." option in the Arduino IDE.

Back to [Table of Contents](#table-of-contents)

## Verifying and Uploading Code to a Microcontroller

# Software Development

Back to [Table of Contents](#table-of-contents)

## WearableAlertDevice.ino

Back to [Table of Contents](#table-of-contents)

## config.h

Back to [Table of Contents](#table-of-contents)

## WirelessDataTransmission.h/.cpp

Back to [Table of Contents](#table-of-contents)

## GunshotDetection.h/.cpp

Back to [Table of Contents](#table-of-contents)
