//Inclusao de bibiliotecas
#include <stdio.h>             // Biblioteca padrão para entrada e saída, utilizada para printf.
#include "pico/stdlib.h"       // Biblioteca padrão para funções básicas do Pico, como GPIO e temporização.
#include "hardware/adc.h"      // Biblioteca para controle do ADC (Conversor Analógico-Digital).
#include "scr/temperature.h"

// Definições
#define ADC_TEMPERATURE_CHANNEL 4   // Canal ADC que corresponde ao sensor de temperatura interno

int main() {
    // Inicializa a comunicação serial para permitir o uso de printf
    stdio_init_all();

    // Inicializa o módulo ADC do Raspberry Pi Pico
    adc_init();

    // Seleciona o canal 4 do ADC (sensor de temperatura interno)
    adc_set_temp_sensor_enabled(true);                  // Habilita o sensor de temperatura interno
    adc_select_input(ADC_TEMPERATURE_CHANNEL);          // Seleciona o canal do sensor de temperatura

    // Loop infinito para leitura contínua do valor de temperatura
    while (true) {
        // Lê o valor do ADC no canal selecionado (sensor de temperatura)
        uint16_t adc_value = adc_read();
        //printf("Adc %d", adc_value); //debug valor adc em decimal
        // Converte o valor do ADC para temperatura em graus Celsius
        float temp_celsius = adc_to_temperature(adc_value);

        // Converte o valor de Celisus para Fahrenheit
        float temp_fahrenheit = celsius_to_fahrenheit(temp_celsius);

        // Imprime a temperatura na comunicação serial
        printf("Temperatura: %.2f °C / %.2f °F \n", temp_celsius, temp_fahrenheit);

        // Atraso de 1000 milissegundos (1 segundo) entre as leituras
        sleep_ms(1000);
    }

    return 0;
}