#include "unity.h"
#include "../src/temperature.h"

void setUp(void) {
    // Configuração inicial antes de cada teste
}

void tearDown(void) {
    // Limpeza após cada teste
}

void test_adc_to_temperature(void) {
    // Teste com valor ADC mínimo (0)
    TEST_ASSERT_FLOAT_WITHIN(0.1, -236.0, adc_to_temperature(0));
    
    // Teste com valor ADC médio (2048)
    TEST_ASSERT_FLOAT_WITHIN(0.1, 27.0 - ((1.65 - 0.706)/0.001721), adc_to_temperature(2048));
    
    // Teste com valor ADC máximo (4095)
    TEST_ASSERT_FLOAT_WITHIN(0.1, 27.0 - ((3.3 - 0.706)/0.001721), adc_to_temperature(4095));
}

void test_celsius_to_fahrenheit(void) {
    // Teste com 0°C
    TEST_ASSERT_FLOAT_WITHIN(0.1, 32.0, celsius_to_fahrenheit(0.0));
    
    // Teste com 100°C
    TEST_ASSERT_FLOAT_WITHIN(0.1, 212.0, celsius_to_fahrenheit(100.0));
    
    // Teste com valor negativo
    TEST_ASSERT_FLOAT_WITHIN(0.1, -40.0, celsius_to_fahrenheit(-40.0));
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_adc_to_temperature);
    RUN_TEST(test_celsius_to_fahrenheit);
    return UNITY_END();
}