//
// Created by Michal_Marszalek on 10.12.2019.
//

#include "menu/Menu.h"
#include "uart/uart.h"
#include "lcd/lcd44780.h"


void *rstry() {
/*
   if(UART_newline){
        while(UART_data){
            lcd_char(uart_read());
        }
        UART_newline = 0;
    }
*/

//    if(UART_data){
//        asm("jmp 0x7E00");
//    }


    return rstry;
}