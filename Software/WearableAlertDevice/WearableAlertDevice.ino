/*
* Main software file that handles all the functionality
* of the Wearable Alert Device.
*
* @author Alex Ho
* @2026 University of Oklahoma
*/

#include <stdio.h>
#include "config.h"
#include "DFRobot_GNSS.h"
#include "DFrobot_MSM261.h"
#include "GunshotDetection.h"
#include "WirelessDataTransmission.h"

/*
* Represents a single data packet containing all the 
* information from the GPS module and microphone. 
*
*/
struct DataPacket {
  char timestampCST[19];
  double latDegrees;
  double longDegrees;
  double altitudeM;
  int maxAudioReadingdB;
  bool gunshotDetected;
  bool callEmergencyResponders;
};

DFRobot_GNSS_I2C gpsModule(&Wire, GNSS_DEVICE_ADDR);
DFRobot_Microphone microphone(I2S_SCK, I2S_WS, I2S_DO);
int16_t timestamp[6];
DataPacket dataPacket;
volatile bool isButtonPressed;
bool isDeviceConnected;
bool isReadyToCollectData;

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
* Handles the state of the button when it is pressed.
* Button utilizes an interrupt. 
* Function is placed in internal RAM of ESP32, not flash. 
*
*/
void IRAM_ATTR handleButtonInterrupt() {
  // Toggle button state to true when it is pressed.
  isButtonPressed = true;
}

// --------------------------------------------
// Main Code
// --------------------------------------------

void setup() {
  Serial.begin(115200); // Baud rate set to 115200
  pinMode(BUTTON, INPUT_PULLUP);  // Button is active-low

  // Attach the interrupt function to the active-low button.
  attachInterrupt(digitalPinToInterrupt(BUTTON), handleButtonInterrupt, FALLING);

  isButtonPressed = false;
  isDeviceConnected = false;
  isReadyToCollectData = false;

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
  BLEDevice::startAdvertising();

  // GPS Module Initialization
  while(!gpsModule.begin()) {
    Serial.println("Failed to initialize GPS module.");
    delay(1000);
  }
  Serial.println("Successfully initialized GPS module.");

  gpsModule.enablePower();

  // Use GPS, Chinese system (Beidou), and Russian satellite system (GLONASS)
  // together. 
  gpsModule.setGnss(eGPS_BeiDou_GLONASS);

  gpsModule.setRgbOn();

  isReadyToCollectData = true;
}

void loop() {
  if (isReadyToCollectData) {

    // Collect timestamp.
    sTim_t utc = gpsModule.getUTC();
    sTim_t date = gpsModule.getDate();
    timestamp[0] = date.year;
    timestamp[1] = date.month;
    timestamp[2] = date.date;
    timestamp[3] = utc.hour + OFFSET_FROM_GMT;
    timestamp[4] = utc.minute;
    timestamp[5] = utc.second;

    // Format timestamp.
    sprintf(dataPacket.timestampCST, "%04d-%02d-%02d %02d:%02d:%02d", timestamp[0], timestamp[1], timestamp[2], timestamp[3], timestamp[4], timestamp[5]);

    // Collect latitude, longitude, and altitude from GPS.
    sLonLat_t latData = gpsModule.getLat();
    sLonLat_t longData = gpsModule.getLon();
    dataPacket.latDegrees = latData.latitudeDegree;
    dataPacket.longDegrees = longData.lonitudeDegree;
    dataPacket.altitudeM = gpsModule.getAlt();

    String dataRecord = "{\"timestamp_cst\":\"" + String(dataPacket.timestampCST) + "\"," +
                        "\"lat_degrees:\":" + String(dataPacket.latDegrees) + "," +
                        "\"long_degrees:\":" + String(dataPacket.longDegrees) + "," + 
                        "\"alt_meters:\":" + String(dataPacket.altitudeM) + ",}";

    if (isButtonPressed && isDeviceConnected) {
      Serial.println("Button pressed.");
      wadCharacteristic->setValue((unsigned char*)dataRecord.c_str(), dataRecord.length());
      wadCharacteristic->notify();
      isButtonPressed = false;
    }
  }

  delay(1000);
}
