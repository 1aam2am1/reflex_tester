#include "lcd/lcd44780.h"
#include "menu/Menu.h"
#include "uart/uart.h"
#include "uart/uart_response.h"
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>

static ret work;

int main() {

    lcd_init();
    lcd_str("Init");
    ///////////////////////////Init buttons etc/////////////////////
    work = rstry;
    char event_table[128];



    /////////////////////////Transmission///////////////////////////

    UART_Init();
    register_uart_str_rx_event_callback(_uart_str_rx_event_callback);
    lcd_cls();
    lcd_str("Transmission");
    lcd_locate(1, 0);
    lcd_str("enabled");
    _delay_ms(500);

    //////////////////////////Timers////////////////////////////////

    lcd_cls();
    lcd_str("Timers on");
    _delay_ms(500);


    /////////////////////////Led check//////////////////////////////
    lcd_cls();
    lcd_str("Led check");

    while (0/*10sekund z timera lub button*/) {

    }


    sei();

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
    while (1) {
        work = work();
        UART_RX_STR_EVENT(event_table);
    }
#pragma clang diagnostic pop

    return 0;
}
