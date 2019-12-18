//
// Created by Michal_Marszalek on 11.12.2019.
//

#include "measure_time.h"
#include <avr/io.h>
#include <avr/interrupt.h>

#if F_CPU != 8000000
#define TIMER_CMP ((F_CPU * 0.001) / 64)
#else
#define TIMER_CMP 125
#endif

volatile uint8_t start_measure;
volatile uint16_t time_ms;

volatile uint8_t button_timer;

void TIMER_Init() {
    TCCR0 |= (1 << WGM01); //CTC MODE compare
    TIMSK |= (1 << OCIE0); //enable compare interruption

    TCNT0 = 0; //reset counter

    OCR0 = TIMER_CMP; //1ms TIMER_CMP

    TCCR0 |= (1 << CS01) | (1 << CS00); //64 prescaler //start timers
}

void TIMER_start() {
    TCNT0 = 0; //reset counter
    start_measure = 1;
    time_ms = 0;
}

void TIMER_stop() {
    start_measure = 0;
    //TCCR0 &= ~((1 << CS02) | (1 << CS01) | (1 << CS00)); //64 prescaler //stop timer
}

uint8_t TIMER_working() {
    return start_measure;
}

uint16_t TIMER_get() {
    return time_ms;
}

//compare match 1ms
ISR(TIMER0_COMP_vect) {
    if (start_measure) { ++time_ms; }

    if (button_timer) { --button_timer; }
}