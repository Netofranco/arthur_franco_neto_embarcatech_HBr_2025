//#include "pico/cyw43_arch.h"

// led_embutido.c
#include "../include/led_embutido.h"  // Inclui o cabeçalho

int led_pico_init(){                //faz a inicializacao do cyw
    if (cyw43_arch_init()) {
        return -1;
    }
}

void led_pico_output (bool level){      //faz o controle de saida de nivel do led
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, level);
}

