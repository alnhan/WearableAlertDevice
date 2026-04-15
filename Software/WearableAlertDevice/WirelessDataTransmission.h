/*
* Software that handles the wireless data transmission
* of the Wearable Alert Device.
*
* @author Alex Ho
* @2026 University of Oklahoma
*/

#pragma once
#include "config.h"
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h>
#include <Arduino.h>

extern BLEServer *wadServer;
extern BLEService *wadService;
extern BLECharacteristic *wadCharacteristic;
extern BLEAdvertising *wadAdvertising;
extern bool isDeviceConnected;

// --------------------------------------------
// Function Declarations
// --------------------------------------------
void initWirelessDataTransmission();