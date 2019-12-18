//
// Created by Michal_Marszalek on 10.12.2019.
//

#ifndef REFLEX_TESTER_MENU_H
#define REFLEX_TESTER_MENU_H

typedef void (*ret)(void);

typedef struct Work {
    ret initialize;

    struct Work *(*work)(void);

    ret destruct;
} Work_t;

Work_t *menu_init();


void led_start();

Work_t *led_work();

void led_end();

void work_start();

Work_t *work_work();

void eeprom_start();

Work_t *eeprom_work();

void run_start();

Work_t *run_work();

void menu_start();

Work_t *menu_work();

#endif //REFLEX_TESTER_MENU_H
