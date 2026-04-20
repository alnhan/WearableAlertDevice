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

(insert pic here)

In the search bar, type "Arduino ESP32 Boards." This package will contain the settings to recognize Arduino Nano ESP32
hardware. Select the Install button if the package is not already installed.

(insert pic here)

Once the package has been installed, the Arduino IDE will automatically install all the common software libraries associated with 
ESP32 microcontrollers, such as WiFi, Bluetooth, etc. No additional steps are needed to install these libraries.

Back to [Table of Contents](#table-of-contents)

## Pin Numbering Setting Configuration
NOTE: The software developed 

Back to [Table of Contents](#table-of-contents)

## Required Library Dependencies

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
