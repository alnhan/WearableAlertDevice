/*
* Software that handles the wireless data transmission
* of the Wearable Alert Device.
*
* @author Alex Ho
* @2026 University of Oklahoma
*/

#pragma once
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h>

BLEServer *wadServer;
BLEService *wadService;
BLECharacteristic *wadCharacteristic;
BLEAdvertising *wadAdvertising;