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

extern double frequencyDataReal[NUM_BUFFER_SAMPLES];  // To contain the audio signal in frequency domain (real values).
extern double frequencyDataImag[NUM_BUFFER_SAMPLES];  // To contain the audio signal in frequency domain (imaginary values).
extern double zeroArray[NUM_BUFFER_SAMPLES];          // Array of 0's
extern ArduinoFFT<double> FFT;

// --------------------------------------------
// Function Declarations
// --------------------------------------------
void collectAudioSample(AudioCircularBuffer &bufferV, AudioCircularBuffer &bufferdB);
double findMaxDBReading(AudioCircularBuffer &bufferdB);
bool peakDetection(int dBReading);