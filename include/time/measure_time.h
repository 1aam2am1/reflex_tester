//
// Created by Michal_Marszalek on 11.12.2019.
//

#ifndef REFLEX_TESTER_MEASURE_TIME_H
#define REFLEX_TESTER_MEASURE_TIME_H

#include <stdint.h>

void TIMER_Init();

void TIMER_start();

void TIMER_stop();

uint16_t TIMER_get();

#endif //REFLEX_TESTER_MEASURE_TIME_H
