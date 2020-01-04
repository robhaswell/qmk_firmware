#include "alt.h"

bool process_record_kb(uint16_t keycode, keyrecord_t *record) {
    // put your per-action keyboard code here
    // runs for every action, just before processing by the firmware

#ifdef CONSOLE_ENABLE
    if (!record->event.pressed) {
        dprintf("RGB: hue: %u, sat: %u, val: %u, spd: %u, mode: %u\n", rgb_matrix_config.hsv.h, rgb_matrix_config.hsv.s, rgb_matrix_config.hsv.v, rgb_matrix_config.speed, rgb_matrix_config.mode);
    }
#endif

    return process_record_user(keycode, record);
}
