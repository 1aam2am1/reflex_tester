//
// Created by Michal_Marszalek on 11.12.2019.
//

#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include "uart/uart_response.h"
#include "uart/uart.h"
#include "eeprom_time/test_log.h"
#include "lcd/lcd44780.h"

const char mkbootloader_rest[] PROGMEM = "AT+RST?";
const char get_eeprom[] PROGMEM = "AT+EEPROM";
const char clear_eeprom[] PROGMEM = "AT+ECLEAR";

void _uart_str_rx_event_callback(char *pBuf) {
    if (strcmp_P(pBuf, mkbootloader_rest) == 0) {
        lcd_cls();
        lcd_str("HEX");
        cli();
        asm("jmp 0x7E00");
    }

    char *mode = pBuf;
    while (*++(mode)) {
        if (*mode == ' ') {
            *mode++ = 0;
        }
        if (*mode == '?') {
            *mode++ = 0;
            *mode = '?';
            *(mode + 1) = 0;
            break;
        }
    }

    if (strcmp_P(pBuf, get_eeprom) == 0) {
        uint8_t i = 0;
        if (*mode == '?') {
            uart_puts_P(get_eeprom);
            uart_puts(" M: N (x, y, t), ...");

            return;
        } else if (mode != 0) {
            i = atoi(mode);
            if (i > 10) { i = 0; }
        }

        Record_t r;
        LOG_load(&r, i);
        uart_puts_P(get_eeprom);
        uart_puts(" M: ");
        uart_putint(r.mode, 10);

        for (i = 0; i < r.mode; ++i) {
            uart_puts(", (");
            uart_putint(r.test[i].b.x, 10);
            uart_puts(", ");
            uart_putint(r.test[i].b.y, 10);
            uart_puts(", ");
            uart_putulint(r.test[i].time);
            uart_puts(")");
        }

    } else if (strcmp_P(pBuf, clear_eeprom) == 0) {
        LOG_clear();
    }
}