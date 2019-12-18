//
// Created by Michal_Marszalek on 18.12.2019.
//

#include "eeprom_time/test_log.h"

uint8_t head EEMEM;
Record_t record[10] EEMEM;

uint8_t head_ram;

void LOG_Init() {
    head_ram = eeprom_read_byte(&head);
}

void LOG_clear() {
    Record_t r = {null, {{{.x = 0, .y = 0}, .time = 0},}};
    for (head_ram = 0; head_ram < 10; ++head_ram) {
        eeprom_busy_wait();
        eeprom_update_block(&r, &(record[head_ram]), sizeof(Record_t));
    }

    head_ram = 0;
    eeprom_busy_wait();
    eeprom_update_byte(&head, head_ram);
}

void LOG_load(Record_t *r, uint8_t i) {
    eeprom_busy_wait();
    eeprom_read_block(r, &(record[(head_ram + i) % 10]), sizeof(Record_t));
}

void LOG_save(Record_t *r) {
    head_ram = (head_ram + 1) % 10;
    eeprom_busy_wait();
    eeprom_update_byte(&head, head_ram);

    eeprom_busy_wait();
    eeprom_update_block(r, &(record[head_ram]), sizeof(Record_t));
}

uint8_t LOG_how_many() {
    uint8_t result = 0;
    uint8_t i;
    for (i = 0; i < 10; ++i) {
        if (eeprom_read_byte(&(record[i].mode)) != null) {
            ++result;
        }
    }
    return result;
}