#include "alt.h"

static bool idle;
static uint32_t idle_timeout = IDLE_TIMEOUT * 1000;
static uint32_t key_timer;
static uint8_t user_hsv_v;

rgb_config_t rgb_matrix_alt_config;

void keyboard_post_init_kb(void) {
    rgb_matrix_alt_config.hsv.h = RGB_MATRIX_STARTUP_ALT_HUE;
    rgb_matrix_alt_config.hsv.s = RGB_MATRIX_STARTUP_ALT_SAT;
    rgb_matrix_alt_config.hsv.v = UINT8_MAX;
    rgb_matrix_alt_config.speed = RGB_MATRIX_STARTUP_ALT_SPD;

    return keyboard_post_init_user();
}

void matrix_scan_kb(void) {
    // put your looping keyboard code here
    // runs every cycle (a lot)

    if (!idle && timer_elapsed32(key_timer) > idle_timeout) { // Detect idleness
        idle = true;
        user_hsv_v = rgb_matrix_config.hsv.v;
        rgb_matrix_config.hsv.v = IDLE_BRIGHTNESS;
    }

    return matrix_scan_user();
}

bool process_record_kb(uint16_t keycode, keyrecord_t *record) {
    // put your per-action keyboard code here
    // runs for every action, just before processing by the firmware
    key_timer = timer_read32();

    if (idle) { // Detect return from idle
        idle = false;
        rgb_matrix_config.hsv.v = user_hsv_v;
    }

    #ifdef CONSOLE_ENABLE
        if (!record->event.pressed) {
            dprintf("    [x] = {{ .enable=%u, .mode=%u, .hsv={ %u, %u, %u }, .speed=%u },\n"\
                    "           { .enable=%u, .mode=%u, .hsv={ %u, %u, %u }, .speed=%u }},\n",
           rgb_matrix_config.enable, rgb_matrix_config.mode, rgb_matrix_config.hsv.h, rgb_matrix_config.hsv.s, rgb_matrix_config.hsv.v, rgb_matrix_config.speed,
           rgb_matrix_alt_config.enable, rgb_matrix_alt_config.mode, rgb_matrix_alt_config.hsv.h, rgb_matrix_alt_config.hsv.s, rgb_matrix_alt_config.hsv.v, rgb_matrix_alt_config.speed);
            // dprintf("RGB: hue: %u, sat: %u, val: %u, spd: %u, mode: %u\n", rgb_matrix_config.hsv.h, rgb_matrix_config.hsv.s, rgb_matrix_config.hsv.v, rgb_matrix_config.speed, rgb_matrix_config.mode);
            // dprintf("ALT: hue: %u, sat: %u, val: %u, spd: %u\n", rgb_matrix_alt_config.hsv.h, rgb_matrix_alt_config.hsv.s, rgb_matrix_alt_config.hsv.v, rgb_matrix_alt_config.speed);
        }
    #endif

    return process_record_user(keycode, record);
}

