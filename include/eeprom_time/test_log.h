//
// Created by Michal_Marszalek on 18.12.2019.
//

#ifndef REFLEX_TESTER_TEST_LOG_H
#define REFLEX_TESTER_TEST_LOG_H

#include <button/button.h>
#include <avr/eeprom.h>

typedef struct {
    Button_t b;
    uint16_t time;
} Node_t;

typedef struct {
    enum {
        null = 0, ten = 10, twenty = 20, twenty_five = 25
    } mode;
    Node_t test[25];
} Record_t;

extern Record_t record[10] EEMEM;

void LOG_Init();

void LOG_clear();

void LOG_load(Record_t *, uint8_t);

void LOG_save(Record_t *);

uint8_t LOG_how_many();

#endif //REFLEX_TESTER_TEST_LOG_H
