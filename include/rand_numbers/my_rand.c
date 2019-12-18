//
// Created by Michal_Marszalek on 18.12.2019.
//

#include "rand_numbers/my_rand.h"
#include <stdlib.h>
#include <avr/pgmspace.h>

int16_t seed;
uint16_t mask;

void my_srandom(int16_t seed_, uint16_t mask_) {
    //srand(seed);
    seed = seed_;
    mask = mask_;
}

uint8_t my_random() {
    //return 0;//rand();

    seed = (seed + 1) % mask;
    return pgm_read_byte(seed);
}