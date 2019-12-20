//
// Created by Michal_Marszalek on 11.12.2019.
//

#include "button/button.h"
#include "time/measure_time.h"
#include <avr/io.h>
#include <util/delay.h>
#include "lcd/lcd44780.h"

Button_t down_b;
Button_t loop_b;
enum {
    idle, down, debounce
} state;
enum {
    E_used, E_up, E_down
} event;
uint8_t read_segemnt;

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

#define IN_PORT_LOW DDR(KEY_INPORT) |= 0b11111
#define IN_PORT_TRISTATE DDR(KEY_INPORT) &= ~0b11111

void KEY_Init() {
    MCUCSR |= (1 << JTD); //jtag off it destroys C port
    MCUCSR |= (1 << JTD);

    IN_PORT_LOW;

    DDR(KEY_OUTPORT) &= ~0b11111; //input

    PORT(KEY_INPORT) &= ~0b11111; //low
    PORT(KEY_OUTPORT) |= 0b11111; //hight pull up

    read_segemnt = 0;
}

void KEY_check() {
    if (read_segemnt == 0) {//in low
        loop_b.x = plece_of_one(~(PIN(KEY_OUTPORT)) & 0b11111);

        IN_PORT_TRISTATE;
        read_segemnt++;
    } else if (read_segemnt == 1) { //in tristate
        loop_b.y = plece_of_one(PIN(KEY_INPORT) & 0b11111);

        IN_PORT_LOW;
        read_segemnt--;
    }

/*
    lcd_locate(1, 0);
    lcd_str("D: (");
    lcd_int(loop_b.x);
    lcd_str(", ");
    lcd_int(loop_b.y);
    lcd_str(")");
    lcd_int(state);
    lcd_str(", ");
    lcd_int(event);
    lcd_str("         ");
    //_delay_ms(20);
*/

    if (loop_b.x != 8 && loop_b.y != 8) {
        if (state == idle) {
            down_b.x = loop_b.x;
            down_b.y = loop_b.y;
            state = down;
            event = E_down;
        } else if (state == debounce) {
            button_timer = KEY_DELEAY;
        }
    } else if (loop_b.x != 8) {
        if (state == debounce) {
            button_timer = KEY_DELEAY;
        }
    } else if (loop_b.x == 8) {
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
}

uint8_t KEY_is_up_event(Button_t b) {
    if (event == E_up) {
        if (down_b.x == b.x && down_b.y == b.y) {
            event = E_used;
            return 1;
        }
    }
    return 0;
}