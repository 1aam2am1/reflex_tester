//
// Created by Michal_Marszalek on 11.12.2019.
//

#ifndef REFLEX_TESTER_LED_H
#define REFLEX_TESTER_LED_H

#include <stdint.h>

#define LED_DATAPORT D
#define LED_DATA 4

#define LED_DISPLAYPORT D
#define LED_DISPLAY 5

#define LED_SCKPORT D
#define LED_SCK 6

#define PORT(x) SPORT(x)
#define SPORT(x) (PORT##x)
// *** PIN
#define PIN(x) SPIN(x)
#define SPIN(x) (PIN##x)
// *** DDR
#define DDR(x) SDDR(x)
#define SDDR(x) (DDR##x)

void LED_Init();

//[0-4][0-4]
void LED_up(uint8_t x, uint8_t y);

void LED_down();

#endif //REFLEX_TESTER_LED_H
