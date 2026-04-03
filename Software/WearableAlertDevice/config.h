/*
* Configuration settings for the Wearable Alert Device.
* Note: GPIO numbers are used, not pin numbers.
*
* @author Alex Ho
* @2026 University of Oklahoma
*/

#define BUTTON 17
// ----------------------------------
// DATA PACKET SETTINGS
// ----------------------------------

// ----------------------------------
// I2C AND GPS MODULE SETTINGS
// ----------------------------------
#define OFFSET_FROM_GMT -5  // CST is 5 hours behind GMT (no daylight savings)

// ----------------------------------
// I2S AND MIC SETTINGS
// ----------------------------------
// Note: Using standard mode for I2S
#define I2S_WS 8
#define I2S_SEL 7
#define I2S_SCK 6
#define I2S_DO 5
#define MIC_LEFT_CH 0
#define MIC_RIGHT_CH 1
#define MIC_SAMPLING_RATE 44100

// Bluetooth
#define SERVICE_UUID "3098A182-02B0-4A35-889A-93B71E1748B9"
#define CHARACTERISTIC_UUID "B536BCCE-2C24-4F7C-BF9F-1DE558316263"
#define BLE_SERVER_NAME "Wearable_Alert_Device"
#define MTU_SIZE 256
