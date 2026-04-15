/*
* Software that handles the gunshot detection functionality
* of the Wearable Alert Device.
*
* @author Alex Ho
* @2026 University of Oklahoma
*/

#include "GunshotDetection.h"

// --------------------------------------------
// Function Definitions
// --------------------------------------------
/*
* Collect a new audio sample from the microphone.
* Find the raw RMS voltage and corresponding dB SPL reading.
* Feed these new readings into their respective audio buffers.
* 
* @param bufferV a circular buffer containing all the audio samples (voltage)
* @param bufferdB a circular buffer containing all the audio samples (dB SPL)
*/
void collectAudioSample(AudioCircularBuffer &bufferV, AudioCircularBuffer &bufferdB) {
  
  // Collect numerous voltage samples and remove the DC bias from the signal.
  // Compute the raw RMS voltage.
  double rmsVoltage = 0.0;
  for (int sample = 0; sample < NUM_PEAK_VOLTAGE_SAMPLES; sample++) {
    double analogVoltage = (MIC_MAX_OUTPUT_V * analogRead(MIC_PIN) / BIT_RES) - MIC_DC_BIAS;
    rmsVoltage += analogVoltage * analogVoltage;
  }
  rmsVoltage /= NUM_PEAK_VOLTAGE_SAMPLES;
  rmsVoltage = sqrt(rmsVoltage);

  // Remove amplifier gain from the RMS voltage.
  double rawRMSVoltage = rmsVoltage / MIC_GAIN;

  // Compute the reference volts per Pascal. 
  double voltsPerPa = pow(10.0, MIC_SENSITIVITY_DBV_PER_PA / 20.0);

  // Compute the Sound Pressure Level in dB.
  double dBSPL = 20 * log10((rawRMSVoltage) / (voltsPerPa * SOUND_PRESSURE_REF_PA));

  // Remove noise floor influence from the dB levels
  dBSPL -= NOISE_FLOOR_LEVEL_DB;

  // Feed audio sample (voltage) into the buffer at the tail end.
  if (bufferV.headIdx == -1 && bufferV.tailIdx == -1) {
    // First audio sample.
    bufferV.headIdx = 0;
    bufferV.tailIdx = 0;
    bufferV.bufferSize += 1;
  }
  else if (bufferV.bufferSize != NUM_BUFFER_SAMPLES) {
    // Buffer is not full yet, no need to remove old data yet.
    bufferV.tailIdx += 1;
    bufferV.bufferSize += 1;
  }
  else if (bufferV.headIdx == NUM_BUFFER_SAMPLES - 1) {
    // Head is at end of the array, loop back to index 0.
    bufferV.headIdx = 0;
    bufferV.tailIdx += 1;
  }
  else if (bufferV.tailIdx == NUM_BUFFER_SAMPLES - 1) {
    // Tail is at end of the array, loop back to index 0.
    bufferV.headIdx += 1;
    bufferV.tailIdx = 0;
  }
  else {
    // No special cases, increment head and tail as normal.
    bufferV.headIdx += 1;
    bufferV.tailIdx += 1;
  }
  bufferV.buffer[bufferV.tailIdx] = rawRMSVoltage;

  // Feed audio sample (dB SPL) into the buffer at the tail end.
  if (bufferdB.headIdx == -1 && bufferdB.tailIdx == -1) {
    // First audio sample.
    bufferdB.headIdx = 0;
    bufferdB.tailIdx = 0;
    bufferdB.bufferSize += 1;
  }
  else if (bufferdB.bufferSize != NUM_BUFFER_SAMPLES) {
    // Buffer is not full yet, no need to remove old data yet.
    bufferdB.tailIdx += 1;
    bufferdB.bufferSize += 1;
  }
  else if (bufferdB.headIdx == NUM_BUFFER_SAMPLES - 1) {
    // Head is at end of the array, loop back to index 0.
    bufferdB.headIdx = 0;
    bufferdB.tailIdx += 1;
  }
  else if (bufferdB.tailIdx == NUM_BUFFER_SAMPLES - 1) {
    // Tail is at end of the array, loop back to index 0.
    bufferdB.headIdx += 1;
    bufferdB.tailIdx = 0;
  }
  else {
    // No special cases, increment head and tail as normal.
    bufferdB.headIdx += 1;
    bufferdB.tailIdx += 1;
  }
  bufferdB.buffer[bufferdB.tailIdx] = dBSPL;
}

/*
* Traverse through all the current dB readings and 
* find the max dB SPL reading.
*
* @param bufferdB a circular buffer containing all the audio samples (dB)
* @return the max dB SPL reading
*/
double findMaxDBReading(AudioCircularBuffer &bufferdB) {
  int index = bufferdB.headIdx; // Index to traverse all audio samples
  double maxdB = bufferdB.buffer[index];  // Variable for storing the current max dB reading.

  // Loop through all audio samples and update the max
  // dB reading when larger readings are found.
  while (index != bufferdB.tailIdx) {
    if (index + 1 == NUM_BUFFER_SAMPLES) {
      // Index at the end of array, loop back to index 0.
      index = 0;
    }
    else {
      // Increment the index.
      index += 1;
    }

    if (maxdB < bufferdB.buffer[index]) {
      // Larger reading found, set as new max value.
      maxdB = bufferdB.buffer[index];
    }
  }

  // Return the max dB reading.
  return maxdB;
}

/*
* Determine whether or not a gunshot may
* possible be detected.
*
* @param the dB SPL reading to examine
* @return whether or not a gunshot may possibly be detected
*/
bool peakDetection(int dBReading) {
  if (dBReading > GUNSHOT_THRESHOLD_DB) {
    // dB SPL reading exceeded gunshot threshold, so return true.
    return true;
  }
  
  // No gunshot detected.
  return false;
}