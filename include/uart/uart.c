//
// Created by Michal_Marszalek on 10.12.2019.
//
#define USART_BAUDRATE 9600
#define BAUD_PRESCALE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include "uart.h"


volatile uint8_t ascii_line;


volatile char UART_RxBuf[UART_RX_BUF_SIZE];
volatile uint8_t UART_RxHead;
volatile uint8_t UART_RxTail;

volatile char UART_TxBuf[UART_TX_BUF_SIZE];
volatile uint8_t UART_TxHead;
volatile uint8_t UART_TxTail;


// wskaźnik do funkcji callback dla zdarzenia UART_RX_STR_EVENT()
static void (*uart_rx_str_event_callback)(char *pBuf);


// funkcja do rejestracji funkcji zwrotnej w zdarzeniu UART_RX_STR_EVENT()
void register_uart_str_rx_event_callback(void (*callback)(char *pBuf)) {
    uart_rx_str_event_callback = callback;
}

// Zdarzenie do odbioru danych łańcucha tekstowego z bufora cyklicznego
void UART_RX_STR_EVENT(char *rbuf) {

    if (ascii_line) {
        if (uart_rx_str_event_callback) {
            uart_get_str(rbuf);
            (*uart_rx_str_event_callback)(rbuf);
        } else {
            UART_RxHead = UART_RxTail;
        }
    }
}

void UART_Init() {
    UART_RxTail = 0;
    UART_RxHead = 0;
    UART_TxTail = 0;
    UART_TxHead = 0;

    UBRRH = (BAUD_PRESCALE >> 8);        //wpisanie starszego bajtu
    UBRRL = BAUD_PRESCALE;             //wpisanie mlodszego bajtu

    UCSRB = (1 << RXEN) | (1 << TXEN);   // Turn on the transmission and reception circuitry
    UCSRB |= (1 << RXCIE);   // Turn on the Receive Complete Interrupt Enable
    UCSRC = (1 << URSEL) | (1 << UCSZ0) | (1 << UCSZ1); // Use 8-bit character sizes

    //UCSRB |= (1 << UDRIE);  //Turn on Data Register Empty Interrupt Enable
    //UCSRB &= ~(1 << UDRIE);  //Turn off Data Register Empty Interrupt Enable
}

void uart_putc(char data) {
    uint8_t tmp_head;

    tmp_head = (UART_TxHead + 1) & UART_TX_BUF_MASK;

    // pętla oczekuje jeżeli brak miejsca w buforze cyklicznym na kolejne znaki
    while (tmp_head == UART_TxTail) {}

    UART_TxBuf[tmp_head] = data;
    UART_TxHead = tmp_head;

    // inicjalizujemy przerwanie występujące, gdy bufor jest pusty, dzięki
    // czemu w dalszej części wysyłaniem danych zajmie się już procedura
    // obsługi przerwania
    UCSRB |= (1 << UDRIE);
}


void uart_puts(char *s)        // wysyła łańcuch z pamięci RAM na UART
{
    register char c;
    // dopóki nie napotkasz 0 wysyłaj znak
    while ((c = *s++)) { uart_putc(c); }
}

void uart_puts_P(const char *s)        // wysyła łańcuch z pamięci RAM na UART
{
    register char c;
    while ((c = pgm_read_byte(s++))) { uart_putc(c); }
}


void uart_putint(int value, int radix)    // wysyła na port szeregowy tekst
{
    char string[17];            // bufor na wynik funkcji itoa
    itoa(value, string, radix);        // konwersja value na ASCII
    uart_puts(string);            // wyślij string na port szeregowy
}

void uart_putulint(uint32_t val) {
    char bufor[11];
    uart_puts(ultoa(val, bufor, 10));
}


int16_t uart_getc() {
    // sprawdzamy czy indeksy są równe
    if (UART_RxHead == UART_RxTail) return -1;

    // obliczamy i zapamiętujemy nowy indeks „ogona węża” (może się zrównać z głową)
    UART_RxTail = (UART_RxTail + 1) & UART_RX_BUF_MASK;
    // zwracamy bajt pobrany z bufora  jako rezultat funkcji
    return UART_RxBuf[UART_RxTail];
}


char *uart_get_str(char *buf) {
    char c;
    char *wsk = buf;
    if (ascii_line) {
        while ((c = uart_getc())) {
            if ('\n' == c || c < 0) break;
            *buf++ = c;
        }
        *buf = 0;
        ascii_line--;
    }
    return wsk;
}

ISR(USART_RXC_vect)        //przerwanie od odbioru danej
{
    register uint8_t tmp_head;
    register char a;
    static uint8_t new_line_before = 0;

    a = UDR; //pobieramy natychmiast bajt danych z bufora sprzętowego

    // obliczamy nowy indeks „głowy węża”
    tmp_head = (UART_RxHead + 1) & UART_RX_BUF_MASK;

    switch (a) {
        case 0:
            break;                  // ignorujemy bajt = 0
        case 13:                    // '\r'
            a = '\n';
        case 10:                    // '\n'
            if (new_line_before) {
                break;
            } else {
                new_line_before = 1;
            }
            ascii_line++;
        default :
            if (a != '\n') { new_line_before = 0; }
            UART_RxHead = tmp_head;
            UART_RxBuf[tmp_head] = a;
            if (((tmp_head + 1) & UART_RX_BUF_MASK) == UART_RxTail) {
                // tutaj możemy w jakiś wygodny dla nas sposób obsłużyć  błąd spowodowany
                // próbą nadpisania danych w buforze, mogłoby dojść do sytuacji gdzie
                // nasz wąż zacząłby zjadać własny ogon
                if (a != 13) { ascii_line++; }
            }
    }
}

ISR(USART_UDRE_vect)        //przerwanie od wyslania danej
{
    // sprawdzamy czy indeksy są różne
    if (UART_TxHead != UART_TxTail) {
        // obliczamy i zapamiętujemy nowy indeks ogona węża (może się zrównać z głową)
        UART_TxTail = (UART_TxTail + 1) & UART_TX_BUF_MASK;
        // zwracamy bajt pobrany z bufora  jako rezultat funkcji
        UDR = UART_TxBuf[UART_TxTail];
    } else {
        // zerujemy flagę przerwania występującego gdy bufor pusty
        UCSRB &= ~(1 << UDRIE);
    }
}
