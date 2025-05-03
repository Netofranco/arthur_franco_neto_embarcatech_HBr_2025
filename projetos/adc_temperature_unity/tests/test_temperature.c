#include "Unity/unity.h"
#include "src/temperature.h"
#include "pico/stdlib.h"
#include <stdio.h>             // Biblioteca padrão para entrada e saída, utilizada para printf.

void setUp(void) {
    // Configuração inicial antes de cada teste
}

void tearDown(void) {
    // Limpeza após cada teste
}

void test_adc_to_temperature_know_level(void){
    TEST_ASSERT_FLOAT_WITHIN(0.2, 27.0, adc_to_temperature(876)); //876 valor correspondente a 27 C
}

int main(void) {

    stdio_init_all();
    sleep_ms(5000); 

    UNITY_BEGIN();
    printf("Iniciando...\n");
    printf("Testando...\n");
    RUN_TEST(test_adc_to_temperature_know_level);
    sleep_ms(2000);
    
    printf("Resultado:\n");
    return UNITY_END();
}