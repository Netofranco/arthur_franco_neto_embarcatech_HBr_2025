#include "temperature.h"

float adc_to_temperature(uint16_t adc_value) {
    const float conversion_factor = 3.3f / (1 << 12);
    float voltage = adc_value * conversion_factor;
    return 27.0f - (voltage - 0.706f) / 0.001721f;
}

float celsius_to_fahrenheit(float temp_celsius) {
    return temp_celsius * 1.8f + 32;
}