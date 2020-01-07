#include "alt.h"

void matrix_init_kb(void) {
    // Define initial values for the RGB configuration.
    rgb_matrix_config.hsv.h = 132;
    rgb_matrix_config.speed = 44;
    eeconfig_init_user();
}

#ifdef CONSOLE_ENABLE
bool process_record_kb(uint16_t keycode, keyrecord_t *record) {
    // put your per-action keyboard code here
    // runs for every action, just before processing by the firmware

    if (!record->event.pressed) {
        dprintf("RGB: hue: %u, sat: %u, val: %u, spd: %u, mode: %u\n", rgb_matrix_config.hsv.h, rgb_matrix_config.hsv.s, rgb_matrix_config.hsv.v, rgb_matrix_config.speed, rgb_matrix_config.mode);
    }

    return process_record_user(keycode, record);
}
#endif
