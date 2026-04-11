/*
* Software that handles the wireless data transmission
* of the Wearable Alert Device.
*
* @author Alex Ho
* @2026 University of Oklahoma
*/
#include "WirelessDataTransmission.h"

extern BLEServer *wadServer = nullptr;
extern BLEService *wadService = nullptr;
extern BLECharacteristic *wadCharacteristic = nullptr;
extern BLEAdvertising *wadAdvertising = nullptr;
extern bool isDeviceConnected = false;

/*
* Tracks whether a phone or computer is connected to the Wearable Alert Device.
*
*/
class WADServerCallbacks : public BLEServerCallbacks {
  
  // A device has connected to WAD.
  // Set flag to true to indicate connection.
  void onConnect(BLEServer* pServer) {
    Serial.println("A device has connected to the WAD.");
    isDeviceConnected = true;
  }

  // Device has disconnected from WAD.
  // Set flag to false to indicate disconnection and restart BLE advertising.
  void onDisconnect(BLEServer* pServer) {
    Serial.println("A device has disconnected from the WAD.");
    isDeviceConnected = false;
    BLEDevice::startAdvertising();
  }
};

/*
* Initialize the wireless data transmission
* functionality for the Wearable Alert Device.
*
*/
void initWirelessDataTransmission() {
  // Bluetooth Initialization
  BLEDevice::init(BLE_SERVER_NAME);
  BLEDevice::setMTU(MTU_SIZE);

  // Create BLE Server.
  wadServer = BLEDevice::createServer();

  // Create BLE Service.
  wadService = wadServer->createService(SERVICE_UUID);

  // Add BLE callback functions to the server.
  wadServer->setCallbacks(new WADServerCallbacks());

  // Create BLE Characteristics.
  wadCharacteristic = wadService->createCharacteristic(CHARACTERISTIC_UUID, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY);
  wadCharacteristic->addDescriptor(new BLE2902());

  // Start the BLE service.
  wadService->start();

  // Start advertising the WAD as a connectable BLE device.
  wadAdvertising = BLEDevice::getAdvertising();
  wadAdvertising->addServiceUUID(SERVICE_UUID);
  wadAdvertising->setScanResponse(true);
  BLEDevice::startAdvertising();
}