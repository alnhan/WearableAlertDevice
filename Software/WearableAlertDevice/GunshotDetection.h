/*
* Software that handles the gunshot detection functionality
* of the Wearable Alert Device.
*
* @author Alex Ho
* @2026 University of Oklahoma
*/

#pragma once
#include "config.h"
#include <Arduino.h>
#include <arduinoFFT.h>
#include <math.h>

/*
* Data structure representing a circular buffer
* for audio samples.
*/
struct AudioCircularBuffer {
  int headIdx = -1;
  int tailIdx = -1;
  int bufferSize = 0;
  double buffer[NUM_BUFFER_SAMPLES];
};

// --------------------------------------------
// Function Declarations
// --------------------------------------------
void collectAudioSample(AudioCircularBuffer &bufferV, AudioCircularBuffer &bufferdB);
double findMaxDBReading(AudioCircularBuffer &bufferdB);
bool peakDetection(int dBReading);