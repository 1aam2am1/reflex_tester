//
// Created by Michal_Marszalek on 10.12.2019.
//

#include <stddef.h>
#include "button/button.h"
#include "eeprom_time/test_log.h"
#include "rand_numbers/my_rand.h"
#include "menu/Menu.h"
#include "uart/uart.h"
#include "lcd/lcd44780.h"
#include "led/led.h"
#include "time/measure_time.h"

static Work_t led = {led_start, led_work, led_end};
static Work_t menu = {menu_start, menu_work, NULL};
static Work_t eeprom = {eeprom_start, eeprom_work, NULL};
static Work_t work_mode = {work_start, work_work, NULL};
static Work_t run = {run_start, run_work, NULL};

static const Button_t u = {2, 1};
static const Button_t s = {2, 2};
static const Button_t d = {2, 3};
static const Button_t l = {1, 2};
static const Button_t r = {3, 2};

static Record_t work_record;
static int8_t menu_variable;
static int8_t menu_variable_2;

Work_t *menu_init() {
    return &led;
}

/////////////////////////////led////////////////////////////////////
void led_start() {
    lcd_cls();
    lcd_str("Led check");
    LED_up(0, 0);
    TIMER_start();
}

Work_t *led_work() {
    Button_t b;
    if (KEY_down_event(&b)) {
        if (b.x == 0 && b.y == 0) {
            lcd_str("OK");
        }
    } else if (KEY_up_event(&b)) {
        if (b.x == 0 && b.y == 0) {
            return &menu;
        }
    }
    if (TIMER_get() >= 10000) {
        return &menu;
    }

    return NULL;
}

void led_end() {
    LED_down();
    TIMER_stop();
}

/////////////////////////////work///////////////////////////////////
void work_start() {
    work_record.mode = ten;

    lcd_cls();
    lcd_str("Work mode: ");
    lcd_int(work_record.mode);
}

Work_t *work_work() {
    if (KEY_is_up_event(r)) {
        if (work_record.mode == ten) {
            return &menu;
        } else if (work_record.mode == twenty) {
            work_record.mode = ten;

            lcd_cls();
            lcd_str("Work mode: ");
            lcd_int(work_record.mode);

        } else if (work_record.mode == twenty_five) {
            work_record.mode = twenty;

            lcd_cls();
            lcd_str("Work mode: ");
            lcd_int(work_record.mode);
        }

    } else if (KEY_is_up_event(l)) {
        if (work_record.mode == ten) {
            work_record.mode = twenty;

            lcd_cls();
            lcd_str("Work mode: ");
            lcd_int(work_record.mode);
        } else if (work_record.mode == twenty) {
            work_record.mode = twenty_five;

            lcd_cls();
            lcd_str("Work mode: ");
            lcd_int(work_record.mode);

        } else if (work_record.mode == twenty_five) {

        }
    } else if (KEY_is_up_event(s)) {
        return &run;
    }

    return NULL;
}


/////////////////////////////eeprom/////////////////////////////////
void eeprom_start() {
    menu_variable = 0;
    menu_variable_2 = 0;
    lcd_cls();
    lcd_str("Records: ");
    lcd_int(LOG_how_many());
}

static inline void display_record() {
    lcd_cls();
    lcd_str("Record nr: ");
    lcd_int(menu_variable);

    if (work_record.mode != null) {
        lcd_locate(1, 0);
        lcd_str("Time: ");
        uint32_t all_time = 0;
        uint8_t i;
        for (i = 0; i < work_record.mode; ++i) {
            all_time += work_record.test[i].time;
        }
        lcd_time(all_time);
        lcd_str("s");
    }
}

static inline void display_record_one() {
    lcd_cls();
    lcd_str("Button: (");
    lcd_int(work_record.test[menu_variable_2].b.x);
    lcd_str(", ");
    lcd_int(work_record.test[menu_variable_2].b.y);
    lcd_str(")");
    lcd_locate(1, 0);
    lcd_str("Time: ");
    lcd_time(work_record.test[menu_variable_2].time);
    lcd_str("s");
}

Work_t *eeprom_work() {
    if (KEY_is_up_event(u)) {
        if (menu_variable == -1) {
            menu_variable = 0;

            lcd_cls();
            lcd_str("Records: ");
            lcd_int(LOG_how_many());
        } else {
            if (menu_variable > 0) {
                if (menu_variable_2 > 0) {
                    --menu_variable_2;
                }
                if (menu_variable_2 > 0) {
                    display_record_one();
                } else {
                    display_record();
                }
            }
        }
    } else if (KEY_is_up_event(d)) {
        if (menu_variable == 0) {
            menu_variable = -1;

            lcd_cls();
            lcd_str("Clear?");
            lcd_locate(1, 0);
            lcd_str("Ok");
        } else {
            if (menu_variable_2 < (work_record.mode - 1)) {
                ++menu_variable_2;
            }
            display_record_one();
        }
    } else if (KEY_is_up_event(s)) {
        if (menu_variable == -1) {
            LOG_clear();
            work_record.mode = null;

            return &menu;
        }
    } else if (KEY_is_up_event(r)) {
        if (menu_variable < 10) {
            ++menu_variable;
            menu_variable_2 = 0;
        }

        LOG_load(&work_record, menu_variable - 1);
        display_record();

    } else if (KEY_is_up_event(l)) {
        if (menu_variable > 0) {
            --menu_variable;
            menu_variable_2 = 0;
        } else {
            return &menu;
        }
        if (menu_variable == 0) {
            lcd_cls();
            lcd_str("Records: ");
            lcd_int(LOG_how_many());
        } else {
            LOG_load(&work_record, menu_variable - 1);
            display_record();
        }
    }

    return NULL;
}


/////////////////////////////run////////////////////////////////////
void run_start() {
    lcd_cls();
    lcd_str("Time to start: ");
    TIMER_start();

    lcd_int(10 - (TIMER_get() / 1000));

    menu_variable = 0;
    menu_variable_2 = 0;
}

Work_t *run_work() {
    if (menu_variable == 0) {
        if (TIMER_get() > 10000) {
            menu_variable = 1;
            TIMER_stop();
            uint8_t x = my_random() % 5;
            uint8_t y = my_random() % 5;

            work_record.test[menu_variable_2].b.x = x;
            work_record.test[menu_variable_2].b.x = y;
            LED_up(x, y);
            TIMER_start();
        } else {
            lcd_cls();
            lcd_str("Time to start: ");
            lcd_int(10 - (TIMER_get() / 1000));
        }
    } else if (menu_variable != 0) {
        if (TIMER_get() > 32767) { menu_variable = 2; }
        Button_t b;
        if (KEY_down_event(&b)) {
            if (b.x == work_record.test[menu_variable_2].b.x &&
                b.y == work_record.test[menu_variable_2].b.y) {
                TIMER_stop();
                LED_down();

                work_record.test[menu_variable_2].time = TIMER_get();
                if (menu_variable == 3) { work_record.test[menu_variable_2].time = UINT16_MAX; }
            }
        }
        if (KEY_up_event(&b)) {
            if (b.x == work_record.test[menu_variable_2].b.x &&
                b.y == work_record.test[menu_variable_2].b.y) {
                ++menu_variable_2;
                if (menu_variable_2 >= work_record.mode) { return &menu; }

                menu_variable = 1;
                uint8_t x = my_random() % 5;
                uint8_t y = my_random() % 5;

                work_record.test[menu_variable_2].b.x = x;
                work_record.test[menu_variable_2].b.x = y;
                LED_up(x, y);
                TIMER_start();
            }
        }
    }
    if (menu_variable == 2) {
        if (TIMER_get() < 32767) { menu_variable = 3; }
    }

    return NULL;
}


/////////////////////////////menu///////////////////////////////////
void menu_start() {
    lcd_cls();
    lcd_str("Start");
}

Work_t *menu_work() {
    if (KEY_is_up_event(r)) {
        return &eeprom;
    } else if (KEY_is_up_event(l)) {
        return &work_mode;
    } else if (KEY_is_up_event(u)) {
        lcd_cls();
        lcd_str("Start");
    } else if (KEY_is_up_event(d)) {
        display_record();
    }

    return NULL;
}