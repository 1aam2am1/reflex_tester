//
// Created by Michal_Marszalek on 11.12.2019.
//

#include "button/button.h"
#include "time/measure_time.h"
#include <avr/io.h>

Button_t down_b;
enum {
    idle, down, debounce
} state;
enum {
    E_used, E_up, E_down
} event;

//x => pull up /low if down         => output port
//y => pull down /height if down    => input port

//[0-7] (8 if null)
uint8_t plece_of_one(uint8_t p) {
    uint8_t result = 0;
    uint8_t i;
    for (i = 0; i < 8 && !(p & 0b1); ++i) {
        result += 1;
        p >>= 1;
    }
    return result;
}

void KEY_Init() {
    DDR(KEY_INPORT) &= ~0b11111; //input
    DDR(KEY_OUTPORT) &= ~0b11111; //input

    PORT(KEY_INPORT) &= ~0b11111; //low
    PORT(KEY_OUTPORT) |= 0b11111; //hight pull up
}

void KEY_check() {
    uint8_t x = ~(PIN(KEY_OUTPORT)) & 0b11111;
    uint8_t y = PIN(KEY_INPORT) & 0b11111;

    x = plece_of_one(x);
    y = plece_of_one(y);

    if (x != 8 && y != 8) {
        if (state == idle) {
            down_b.x = x;
            down_b.y = y;

            state = down;
            event = E_down;
        }
    } else {
        if (state == down) {
            button_timer = KEY_DELEAY;
            state = debounce;
            event = E_up;
        }
        if (state == debounce && !button_timer) {
            state = idle;
        }
    }

}

uint8_t KEY_down_event(Button_t *b) {
    *b = down_b;
    if (event == E_down) {
        event = E_used;
        return 1;
    }

    return 0;
}

uint8_t KEY_up_event(Button_t *b) {
    *b = down_b;
    if (event == E_up) {
        event = E_used;
        return 1;
    }

    return 0;
}

uint8_t KEY_is_down(Button_t b) {
    if (down_b.x == b.x && down_b.y == b.y && state == down) { return 1; }
    return 0;

    /*
    //pin number np 0b00000010
    uint8_t x = (1 << b.x);

    //pin number np 0b00000010
    uint8_t y = (1 << b.y);


    if (~(PIN(KEY_OUTPORT)) & x) {
        if(PIN(KEY_INPORT) & y) {
            return 1;
        }
    }

    return 0;
     */
}

uint8_t KEY_is_up_event(Button_t b) {
    Button_t p;
    if (KEY_up_event(&p)) {
        if (p.x == b.x && p.y == b.y) { return 1; }
    }
    return 0;
}