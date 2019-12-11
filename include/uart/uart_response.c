//
// Created by Michal_Marszalek on 11.12.2019.
//

#include <avr/pgmspace.h>
#include "uart/uart_response.h"

const char mkbootloader_rest[] PROGMEM = "AT+RST?";

void _uart_str_rx_event_callback(char *pBuf) {
    if (strcmp_P(pBuf, mkbootloader_rest) == 0) {
        asm("jmp 0x7E00");
    }
}