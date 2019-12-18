#include "lcd/lcd44780.h"
#include "menu/Menu.h"
#include "uart/uart.h"
#include "uart/uart_response.h"
#include "time/measure_time.h"
#include "led/led.h"
#include "button/button.h"
#include "eeprom_time/test_log.h"
#include "rand_numbers/my_rand.h"
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stddef.h>

static Work_t work;

int main() {
    char event_table[128];

    lcd_init();
    lcd_str("Init");

    LED_Init();
    TIMER_Init();


    UART_Init();
    register_uart_str_rx_event_callback(_uart_str_rx_event_callback);

    KEY_Init();
    LOG_Init();

    sei();

    TIMER_start();
    _delay_ms(500);

    my_srandom(TIMER_get(), 5380);
    TIMER_stop();

    work.initialize = NULL;
    work.work = menu_init;
    work.destruct = NULL;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
    while (1) {
        UART_RX_STR_EVENT(event_table);
        KEY_check();

        Work_t *new = work.work();

        if (new != NULL) {
            if (work.destruct) { work.destruct(); }
            work = *new;
            if (work.initialize) { work.initialize(); }
        }
    }
#pragma clang diagnostic pop

    return 0;
}
