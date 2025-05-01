#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"            //Biblioteca default para utilizacao do led da raspberry
#include "../include/led_embutido.h"    //include para controle do driver do led
#include "../include/hal_led.h"         //include para manipulacao do led

int tempo_led = 500; //Cria uma variavel para armazenar o tempo gasto de cada piscada

int main() {
    stdio_init_all();  

    if (led_pico_init()){
        return -1;
    }

    while (true) {
        hal_led_toogle(tempo_led);   //chama função que faz inversão do estado do led
    }
}