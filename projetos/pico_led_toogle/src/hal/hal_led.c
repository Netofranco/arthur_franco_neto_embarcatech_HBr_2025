#include "../include/led_embutido.h"
#include "../include/hal_led.h"

//funcao que faz o led piscar conforme tempo indicado
void hal_led_toogle(int ms){
    led_pico_output(1);
    sleep_ms(ms);
    led_pico_output(0);
    sleep_ms(ms);
}

