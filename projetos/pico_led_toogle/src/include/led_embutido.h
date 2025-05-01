// led_embutido.h
#ifndef LED_EMBUTIDO_H  // Guarda de inclusão para evitar duplicação
#define LED_EMBUTIDO_H

#include <stdbool.h>   // Para usar o tipo 'bool'
#include "pico/cyw43_arch.h"  // Dependência do Pico SDK

// Declaração da função
int led_pico_init(void);
void led_pico_output(bool level);

#endif // LED_EMBUTIDO_H