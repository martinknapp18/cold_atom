#include "mbed.h"

#ifndef _SETTINGS_H_
#define _SETTINGS_H_

#define V_REF   10
#define RES     4095

// Function to convert voltage to decimal value for DAC
constexpr int16_t to_dac(double voltage)
{
    return static_cast<int16_t>((voltage / V_REF) * RES);
}

// Variables
constexpr uint8_t num_images = 5; // images to take during MOT Temp measurement
constexpr uint16_t ADC_samples = 127; // sample number when reading the ADC for detection


#endif // _SETTINGS_H_