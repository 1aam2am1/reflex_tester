//
// Created by Michal_Marszalek on 11.12.2019.
//

#include <avr/io.h>
#include "led/led.h"

#define SET_DATA        PORT(LED_DATAPORT) |= (1<<LED_DATA)            // stan wysoki na linii RS
#define CLR_DATA        PORT(LED_DATAPORT) &= ~(1<<LED_DATA)        // stan niski na linii RS

#define SET_DISPLAY     PORT(LED_DISPLAYPORT) |= (1<<LED_DISPLAY)            // stan wysoki na RW - odczyt z LCD
#define CLR_DISPLAY     PORT(LED_DISPLAYPORT) &= ~(1<<LED_DISPLAY)        // stan niski na RW - zapis do LCD

#define SET_SCK         PORT(LED_SCKPORT) |= (1<<LED_SCK)            // stan wysoki na linii E
#define CLR_SCK         PORT(LED_SCKPORT) &= ~(1<<LED_SCK)            // stan niski na linii E

void shift_bit_out(uint16_t to_shift) {
    uint8_t i = 16;
    while (i--) {
        CLR_SCK;
        if (to_shift & 0x8000) { SET_DATA; } else { CLR_DATA; }
        to_shift <<= 1;
        SET_SCK;
        asm("nop");
    }
}

void LED_Init() {
    DDR(LED_DATAPORT) |= (1 << LED_DATA);
    DDR(LED_DISPLAYPORT) |= (1 << LED_DISPLAY);
    DDR(LED_SCKPORT) |= (1 << LED_SCK);

    CLR_DATA;
    CLR_DISPLAY;
    CLR_SCK;

    LED_down();
}

//OUT=IN=[8]
//DATA->OUT[1-5]->IN[1-5]->NULL
//x => out voltage height if up
//y => in voltage low if up

void LED_up(uint8_t x, uint8_t y) {
    uint16_t to_shift;

    to_shift = (1 << (x + 1));

    to_shift |= (~(1 << (y + 1)) << 8);

    CLR_DISPLAY;

    shift_bit_out(to_shift);

    SET_DISPLAY;
}

void LED_down() {
    CLR_DISPLAY;

    shift_bit_out(0b1111111100000000);

    SET_DISPLAY;
}