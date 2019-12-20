//
// Created by Michal_Marszalek on 10.12.2019.
//

#ifndef REFLEX_TESTER_UART_H
#define REFLEX_TESTER_UART_H

#include <stdint.h>

extern volatile uint8_t ascii_line;

#define UART_RX_BUF_SIZE 128
#define UART_RX_BUF_MASK ( UART_RX_BUF_SIZE - 1)

#define UART_TX_BUF_SIZE 128
#define UART_TX_BUF_MASK ( UART_TX_BUF_SIZE - 1)


void UART_Init();

int16_t uart_getc(void);

void uart_putc(char c);

void uart_puts(char *s);

void uart_puts_P(const char *s);

void uart_putint(int16_t value, int16_t radix);

void uart_putulint(uint32_t value);

char *uart_get_str(char *buf);

void UART_RX_STR_EVENT(char *rbuf);

void register_uart_str_rx_event_callback(void (*callback)(char *pBuf));

#endif //REFLEX_TESTER_UART_H
