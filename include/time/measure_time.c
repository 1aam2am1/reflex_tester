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

volatile uint16_t time_ms;

void TIMER_Init() {
    TCCR0 |= (1 << WGM01); //CTC MODE compare
    TIMSK |= (1 << OCIE0); //enable compare interruption

    TCNT0 = 0; //reset counter

    OCR0 = TIMER_CMP; //1ms TIMER_CMP
}

void TIMER_start() {
    TCNT0 = 0; //reset counter
    time_ms = 0;
    TCCR0 |= (1 << CS01) | (1 << CS00); //64 prescaler
}

void TIMER_stop() {
    TCCR0 &= ~((1 << CS02) | (1 << CS01) | (1 << CS00)); //64 prescaler
}

uint16_t TIMER_get() {
    return time_ms;
}

//compare match 1ms
ISR(TIMER0_COMP_vect) {
    ++time_ms;
}