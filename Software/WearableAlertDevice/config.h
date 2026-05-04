/*
* Configuration settings for the Wearable Alert Device.
* Note: GPIO numbers are used, not pin numbers.
*
* @author Alex Ho
* @2026 University of Oklahoma
*/

#define BUTTON 17
#define LED_PIN 18
// ----------------------------------
// I2C AND GPS MODULE SETTINGS
// ----------------------------------
#define YEAR_IDX 0
#define MONTH_IDX 1
#define DAY_IDX 2
#define HOUR_IDX 3
#define MINUTE_IDX 4
#define SECOND_IDX 5
#define OFFSET_FROM_GMT -5  // CST is 5 hours behind GMT (no daylight savings)
#define NUM_ALTITUDE_SAMPLES 20
#define MAX_GPS_UPDATE_COUNTER_VALUE 15

// ----------------------------------
// ANALOG MIC SETTINGS
// ----------------------------------
// NOTE: MIC Settings based on Fermion MEMS Microphone (SKU: SEN0487)
#define MIC_PIN 1
#define SAMPLING_RATE_HZ 44100
#define MICROSEC_PER_SEC 1000000
#define MIC_MAX_OUTPUT_V 3.3
#define BIT_RES 4095 // 12-bit resolution = 4096 possible analog readings (0-4095)
#define NUM_PEAK_VOLTAGE_SAMPLES 1000
#define MIC_SENSITIVITY_DBV_PER_PA -42
#define MIC_GAIN 66
#define MIC_DC_BIAS 1.47
#define SOUND_PRESSURE_REF_PA 0.00002
#define NOISE_FLOOR_LEVEL_DB 15
#define NUM_BUFFER_SAMPLES 16
#define GUNSHOT_THRESHOLD_DB 75
#define GUNSHOT_THRESHOLD_HZ 500.0

// Bluetooth
#define SERVICE_UUID "3098A182-02B0-4A35-889A-93B71E1748B9"
#define CHARACTERISTIC_UUID "B536BCCE-2C24-4F7C-BF9F-1DE558316263"
#define BLE_SERVER_NAME "Wearable_Alert_Device"
#define MTU_SIZE 256
