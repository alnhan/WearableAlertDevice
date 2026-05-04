/*
* Main software file that handles all the functionality
* of the Wearable Alert Device.
*
* @author Alex Ho
* @2026 University of Oklahoma
*/
#pragma once
#include <stdio.h>
#include "config.h"
#include "DFRobot_GNSS.h"
#include "GunshotDetection.h"
#include "WirelessDataTransmission.h"

/*
* Represents a single data packet containing all the 
* information from the GPS module and microphone. 
*
*/
struct DataPacket {
  char timestampCST[20];
  double latDegrees;
  double longDegrees;
  double altitudeM;
  int maxAudioReadingdB;
  bool gunshotDetected;
  volatile bool callEmergencyResponders;
};

DFRobot_GNSS_I2C gpsModule(&Wire, GNSS_DEVICE_ADDR);
int16_t timestamp[6];
DataPacket dataPacket;
AudioCircularBuffer audioBufferV;
AudioCircularBuffer audioBufferdB;
volatile bool isButtonPressed;
bool isReadyToCollectData;
int gpsUpdateCounter;

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

/*
* Adjust the timestamp to be in CST instead of GMT. 
*
*/
void adjustTimestampToCST() {
  timestamp[HOUR_IDX] += OFFSET_FROM_GMT;

  if (timestamp[HOUR_IDX] < 0) {
    // Negative hours, add 24 hours to be within military time range.
    timestamp[HOUR_IDX] += 24;

    // Set the time back one day.
    timestamp[DAY_IDX] -= 1;

    // Check if time went back into previous month.
    if (timestamp[DAY_IDX] == 0) {
      // Time went back into previous month, day is now last day of that month.
      switch (timestamp[MONTH_IDX]) {
        case (1):
          // January, move back to December
          timestamp[DAY_IDX] = 31;
          timestamp[MONTH_IDX] = 12;
          timestamp[YEAR_IDX] -= 1;
          break;
        case (2):
          // February, move back to January
          timestamp[DAY_IDX] = 31;
          timestamp[MONTH_IDX] = 1;
        case (3):
          // March, move back to February
          if ((timestamp[YEAR_IDX] % 4 == 0) && (timestamp[YEAR_IDX] % 100 != 0 || timestamp[YEAR_IDX] % 400 == 0)) {
            // Leap year consideration.
            timestamp[DAY_IDX] = 29;
          }
          else {
            timestamp[DAY_IDX] = 28;
          }
          timestamp[MONTH_IDX] = 2;
          break;
        case (4):
          // April, move back to March
          timestamp[DAY_IDX] = 31;
          timestamp[MONTH_IDX] = 3;
          break;
        case (5):
          // May, move back to April
          timestamp[DAY_IDX] = 30;
          timestamp[MONTH_IDX] = 4;
          break;
        case (6):
          // June, move back to May
          timestamp[DAY_IDX] = 31;
          timestamp[MONTH_IDX] = 5;
          break;
        case (7):
          // July, move back to June
          timestamp[DAY_IDX] = 30;
          timestamp[MONTH_IDX] = 6;
          break;
        case (8):
          // August, move back to July
          timestamp[DAY_IDX] = 31;
          timestamp[MONTH_IDX] = 7;
          break;
        case (9):
          // September, move back to August
          timestamp[DAY_IDX] = 31;
          timestamp[MONTH_IDX] = 8;
          break;
        case (10):
          // October, move back to September
          timestamp[DAY_IDX] = 30;
          timestamp[MONTH_IDX] = 9;
          break;
        case (11):
          // November, move back to October
          timestamp[DAY_IDX] = 31;
          timestamp[MONTH_IDX] = 10;
          break;
        case (12):
          // December, move back to November
          timestamp[DAY_IDX] = 30;
          timestamp[MONTH_IDX] = 11;
          break;
        default:
          break; 
      }
    }
  }
}

// --------------------------------------------
// Main Code
// --------------------------------------------
void setup() {
  Serial.begin(115200);                 // Baud rate set to 115200
  pinMode(BUTTON, INPUT_PULLUP);        // Button is active-low
  pinMode(MIC_PIN, INPUT_PULLDOWN);     // Microphone
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  // Attach the interrupt function to the active-low button.
  attachInterrupt(digitalPinToInterrupt(BUTTON), handleButtonInterrupt, FALLING);

  isButtonPressed = false;
  isDeviceConnected = false;
  isReadyToCollectData = false;
  dataPacket.gunshotDetected = false;
  dataPacket.callEmergencyResponders = false;
  gpsUpdateCounter = MAX_GPS_UPDATE_COUNTER_VALUE;

  // Initialize wireless data tranmission for Wearable Alert Device.
  initWirelessDataTransmission();

  // GPS Module Initialization
  while(!gpsModule.begin()) {
    Serial.println("Failed to initialize GPS module.");
    delay(1000);
  }
  Serial.println("Successfully initialized GPS module.");
  gpsModule.enablePower();
  gpsModule.setGnss(eGPS_BeiDou_GLONASS);  // Use GPS, Chinese system (Beidou), and Russian satellite system (GLONASS) together. 
  gpsModule.setRgbOn();

  // Set data collection flag to true. 
  isReadyToCollectData = true;
}

void loop() {
  if (isReadyToCollectData) {
    if (gpsUpdateCounter == MAX_GPS_UPDATE_COUNTER_VALUE) {
      // It has been about 1 second since GPS data last updated.
      // Update GPS data.
      // Collect timestamp.
      sTim_t utc = gpsModule.getUTC();
      sTim_t date = gpsModule.getDate();
      timestamp[YEAR_IDX] = date.year;
      timestamp[MONTH_IDX] = date.month;
      timestamp[DAY_IDX] = date.date;
      timestamp[HOUR_IDX] = utc.hour;
      timestamp[MINUTE_IDX] = utc.minute;
      timestamp[SECOND_IDX] = utc.second;

      adjustTimestampToCST();

      // Format timestamp.
      sprintf(dataPacket.timestampCST, "%04d-%02d-%02d %02d:%02d:%02d", timestamp[0], timestamp[1], timestamp[2], timestamp[3], timestamp[4], timestamp[5]);

      // Collect latitude, longitude, and altitude from GPS.
      sLonLat_t latData = gpsModule.getLat();
      sLonLat_t longData = gpsModule.getLon();

      // Northern and Southern Hemisphere checking for coordinates.
      if (latData.latDirection == 'N') {
        // Northern hemisphere, so latitude is positive.
        dataPacket.latDegrees = latData.latitudeDegree;
      }
      else if (latData.latDirection == 'S') {
        // Southern hemisphere, so latitude is negative.
        dataPacket.latDegrees = -1 * latData.latitudeDegree;
      }

      // Eastern and Western Hemisphere checking for coordinates.
      if (longData.lonDirection == 'E') {
        // Eastern hemisphere, so longitude is positive.
        dataPacket.longDegrees = longData.lonitudeDegree;
      }
      else if (longData.lonDirection == 'W'){
        // Western hemisphere, so longituide is negative.
        dataPacket.longDegrees = -1 * longData.lonitudeDegree;
      }

      // Algorithm for averaging altitude measurement for better accuracy.
      dataPacket.altitudeM = 0.0;
      for (int sample = 0; sample < NUM_ALTITUDE_SAMPLES; sample++) {
        dataPacket.altitudeM += gpsModule.getAlt();
      }
      dataPacket.altitudeM /= NUM_ALTITUDE_SAMPLES;

      // Reset update counter.
      gpsUpdateCounter = 0;
    }
    // Collect a new audio sample.
    collectAudioSample(audioBufferV, audioBufferdB);

    // Determine the new max audio reading.
    dataPacket.maxAudioReadingdB = (int)(findMaxDBReading(audioBufferdB));

    //Serial.println(dataPacket.maxAudioReadingdB);

    // Peak Detection.
    if(peakDetection(dataPacket.maxAudioReadingdB)) {
      // dB level spiked above threshold, possible gunshot detected.
      // Perform further analysis to determine if it is actually a gunshot.

      // Clear all values from the frequency data (imaginary values).
      memcpy(frequencyDataImag, zeroArray, NUM_BUFFER_SAMPLES * sizeof(double));

      // Copy raw RMS voltage from audio buffer to the array of frequency data (real values).
      if (audioBufferV.headIdx == 0 && audioBufferV.tailIdx == NUM_BUFFER_SAMPLES - 1) {
        // No array partitions needed, just copy the entire array.
        memcpy(frequencyDataReal, audioBufferV.buffer, NUM_BUFFER_SAMPLES * sizeof(double));
      }
      else if (audioBufferV.tailIdx < audioBufferV.headIdx) {
        // Array partitions needed to properly copy data.
        int firstPartitionSize = NUM_BUFFER_SAMPLES - audioBufferV.headIdx + 1;
        int secondPartitionSize = NUM_BUFFER_SAMPLES - firstPartitionSize;

        memcpy(&frequencyDataReal[0], &audioBufferV.buffer[audioBufferV.headIdx], (firstPartitionSize) * sizeof(double));
        memcpy(&frequencyDataReal[firstPartitionSize - 1], &audioBufferV.buffer[0], (secondPartitionSize) * sizeof(double));
      }
      else if (audioBufferV.headIdx < audioBufferV.tailIdx) {
        // Audio buffer is not entirely full yet.
        int numElements = audioBufferV.tailIdx - audioBufferV.headIdx + 1;
        memcpy(&frequencyDataReal[0], &audioBufferV.buffer[0], (numElements) * sizeof(double));
      }

      // Apply Hamming window function.
      FFT.windowing(FFTWindow::Hamming, FFTDirection::Forward);

      // Compute the Fast Fourier Transform.
      FFT.compute(FFTDirection::Forward);

      // Calculate the magnitudes of the complex numbers in the frequency content.
      FFT.complexToMagnitude();

      // Extract the dominant frequency in Hz.
      double dominantFrequencyHz = FFT.majorPeak();
      
      // Check if dominant frequency is under the threshold frequency for a muzzle blast. 
      if (0.0 < dominantFrequencyHz && dominantFrequencyHz < GUNSHOT_THRESHOLD_HZ) {
        Serial.print("Dominant Frequency: ");
        Serial.println(dominantFrequencyHz, 5);
        dataPacket.gunshotDetected = true;
        dataPacket.callEmergencyResponders = true;
        digitalWrite(LED_PIN, HIGH);
      }
      else {
        dataPacket.gunshotDetected = false;
        dataPacket.callEmergencyResponders = false;
        digitalWrite(LED_PIN, LOW);
      }
    }
    else {
      // Gunshot threshold was not exceeded.
      digitalWrite(LED_PIN, LOW);
    }

    // Format data in JSON format.
    String dataRecord = "{\"timestamp_cst\":\"" + String(dataPacket.timestampCST) + "\"," +
                        "\"lat_degrees:\":" + String(dataPacket.latDegrees, 5) + "," +
                        "\"long_degrees:\":" + String(dataPacket.longDegrees, 5) + "," + 
                        "\"alt_meters:\":" + String(dataPacket.altitudeM, 2) + "," +
                        "\"max_dB_reading:\":" + String(dataPacket.maxAudioReadingdB) + ",";

    if (dataPacket.gunshotDetected) {
      dataRecord += "\"gunshot_detected\":true,"; 
    }
    else {
      dataRecord += "\"gunshot_detected\":false,";
    }

    if (dataPacket.callEmergencyResponders) {
      dataRecord += "\"call_responders\":true}";
    }
    else {
      dataRecord += "\"call_responders\":false}";
    }

    // Send data if button is pressed or gunshot has been detected.
    if ((isButtonPressed || dataPacket.gunshotDetected) && isDeviceConnected) {
      Serial.println("Data Transmitted.");
      dataPacket.callEmergencyResponders = true;
      wadCharacteristic->setValue((unsigned char*)dataRecord.c_str(), dataRecord.length());
      wadCharacteristic->notify();
      isButtonPressed = false;
    }
    dataPacket.callEmergencyResponders = false;

    // Increment GPS update counter.
    gpsUpdateCounter += 1;

    delayMicroseconds((int)((1.0 / SAMPLING_RATE_HZ) * MICROSEC_PER_SEC));
  }
}
