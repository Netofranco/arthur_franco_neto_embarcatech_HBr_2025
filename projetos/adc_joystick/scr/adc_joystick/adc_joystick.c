/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

 #include <stdio.h>
 #include "pico/stdlib.h"
 #include "hardware/adc.h"
 
 int main() {
     stdio_init_all();
     adc_init();
     // Make sure GPIO is high-impedance, no pullups etc
     adc_gpio_init(26);
     adc_gpio_init(27);
 
     while (1) {
         adc_select_input(0);
         uint adc_y = adc_read();
         adc_select_input(1);
         uint adc_x = adc_read();
 
         printf("Eixo x %d e ", adc_x);
         printf("Eixo y %d \n", adc_y);
         sleep_ms(1000);
 
     }
 }