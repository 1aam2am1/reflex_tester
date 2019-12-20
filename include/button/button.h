//
// Created by Michal_Marszalek on 11.12.2019.
//

#ifndef REFLEX_TESTER_BUTTON_H
#define REFLEX_TESTER_BUTTON_H

#include <stdint.h>

#define KEY_INPORT C

#define KEY_OUTPORT B

#define KEY_DELEAY 20;

#define PORT(x) SPORT(x)
#define SPORT(x) (PORT##x)
// *** PIN
#define PIN(x) SPIN(x)
#define SPIN(x) (PIN##x)
// *** DDR
#define DDR(x) SDDR(x)
#define SDDR(x) (DDR##x)

typedef struct Button {
    uint8_t x; //[0-4]
    uint8_t y; //[0-4]
} Button_t;

void KEY_Init();

void KEY_check();

uint8_t KEY_down_event(Button_t *);

uint8_t KEY_up_event(Button_t *);

uint8_t KEY_is_down(Button_t b);

uint8_t KEY_is_up_event(Button_t b);

#endif //REFLEX_TESTER_BUTTON_H
