//Nome: Arthur Franco Neto
/*Refaça o Programa Prático 00 presente no Ebook do Capítulo de ADC (Capítulo 8), 
mas implementando no código a conversão da unidade de medida da temperatura de Celsius para Fahrenheit.
*/

//Inclusao de bibiliotecas
#include <stdio.h>             // Biblioteca padrão para entrada e saída, utilizada para printf.
#include "pico/stdlib.h"       // Biblioteca padrão para funções básicas do Pico, como GPIO e temporização.
#include "hardware/adc.h"      // Biblioteca para controle do ADC (Conversor Analógico-Digital).

// Definições
#define ADC_TEMPERATURE_CHANNEL 4   // Canal ADC que corresponde ao sensor de temperatura interno

// Função para converter o valor lido do ADC para temperatura em graus Celsius
float adc_to_temperature(uint16_t adc_value) {
    // Constantes fornecidas no datasheet do RP2040
    const float conversion_factor = 3.3f / (1 << 12);  // Conversão de 12 bits (0-4095) para 0-3.3V
    float voltage = adc_value * conversion_factor;     // Converte o valor ADC para tensão
    float temperature = 27.0f - (voltage - 0.706f) / 0.001721f;  // Equação fornecida para conversão no datasheet
    //printf("Tensao: %.5f" , voltage);                 //debug valor da tensao calculada
    return temperature;
}

// Função para converter a temperatura de Celsius para Fahrenheit
float Celsius_to_Fahreinheit(float temp_celsius){
    float temp_fahr = temp_celsius * 1.8f + 32;
    return temp_fahr;
}

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
        float temp_fahrenheit = Celsius_to_Fahreinheit(temp_celsius);

        // Imprime a temperatura na comunicação serial
        printf("Temperatura: %.2f °C / %.2f °F \n", temp_celsius, temp_fahrenheit);

        // Atraso de 1000 milissegundos (1 segundo) entre as leituras
        sleep_ms(1000);
    }

    return 0;
}
