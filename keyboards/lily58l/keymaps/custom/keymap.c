#include QMK_KEYBOARD_H

#ifdef PROTOCOL_LUFA
  #include "lufa.h"
  #include "split_util.h"
#endif
#ifdef SSD1306OLED
  #include "ssd1306.h"
#endif

#ifdef RGBLIGHT_ENABLE
//Following line allows macro to read current RGB settings
extern rgblight_config_t rgblight_config;
#endif

extern uint8_t is_master;

enum custom_layers {
    _MAC = 0,
    _WIN,
    _WINSHIFT,
    _LOWER,
    _RAISE,
    _ADJUST
};
enum custom_keycodes {
  QWERTY = SAFE_RANGE,
  LOWER,
  RAISE,
  ADJUST,
  ENCODER,
  TOG_OS
};
enum encoder_modes {
    LEFTRIGHT = 0,
    UPDOWN,
    TABS,
    SCROLL,
};
uint8_t encoder_modes_len = 4;
uint8_t encoder_mode      = 0;

#define MODS_SHIFT  (get_mods() & MOD_BIT(KC_LSHIFT) || get_mods() & MOD_BIT(KC_RSHIFT))
#define MODS_CTRL   (get_mods() & MOD_BIT(KC_LCTL) || get_mods() & MOD_BIT(KC_RCTRL))
#define MODS_ALT    (get_mods() & MOD_BIT(KC_LALT) || get_mods() & MOD_BIT(KC_RALT))
#define MODS_GUI    (get_mods() & MOD_BIT(KC_LGUI) || get_mods() & MOD_BIT(KC_RGUI))


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

[_MAC] = LAYOUT( \
  KC_GESC,  KC_1,   KC_2,    KC_3,    KC_4,    KC_5,                     KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, \
  KC_TAB,   KC_Q,   KC_W,    KC_E,    KC_R,    KC_T,                     KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_EQL, \
  KC_LCTRL, KC_A,   KC_S,    KC_D,    KC_F,    KC_G,                     KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT, \
  KC_LSFT,  KC_Z,   KC_X,    KC_C,    KC_V,    KC_B,  ENCODER, KC_BSPC,  KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,  KC_RSFT, \
                             KC_LALT, KC_LGUI, LOWER, KC_SPC,  KC_ENT,   RAISE,   KC_RALT, KC_BSLS \
),
[_WIN] = LAYOUT( \
  KC_ESC,                   KC_1,   KC_2,    KC_3,    KC_4,    KC_5,                        KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, \
  KC_TAB,                   KC_Q,   KC_W,    KC_E,    KC_R,    KC_T,                        KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_EQL, \
  KC_LCTRL,                 KC_A,   KC_S,    KC_D,    KC_F,    KC_G,                        KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT, \
  LM(_WINSHIFT, MOD_LSFT),  KC_Z,   KC_X,    KC_C,    KC_V,    KC_B,    ENCODER,  KC_BSPC,  KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,  LM(_WINSHIFT, MOD_RSFT), \
                                             KC_LGUI, KC_LALT, LOWER,   KC_SPC,   KC_ENT,   RAISE,   KC_RGUI, KC_NUBS \
),
[_WINSHIFT] = LAYOUT( \
  _______, _______, KC_QUOT, _______, _______, _______,                   _______, _______, _______, _______, _______, _______, \
  _______, _______, _______, _______, _______, _______,                   _______, _______, _______, _______, _______, _______, \
  _______, _______, _______, _______, _______, _______,                   _______, _______, _______, _______, _______, KC_2, \
  _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,\
                             _______, _______, _______, _______, _______, _______, _______, _______ \
),
[_LOWER] = LAYOUT( \
  KC_GRV,  _______, _______, LALT(KC_3),_______,_______,                  _______, KC_P7,   KC_P8,   KC_P9,   _______, KC_PGUP, \
  _______, _______, _______, _______, _______, KC_LPRN,                   KC_RPRN, KC_P4,   KC_P5,   KC_P6,   KC_MPLY, KC_PGDN, \
  _______, _______, _______, _______, _______, KC_LBRC,                   KC_RBRC, KC_P1,   KC_P2,   KC_P3,   _______, _______, \
  _______, _______, _______, _______, _______, KC_LCBR, _______, KC_DEL,  KC_RCBR, KC_P0,   _______, _______, _______, _______, \
                             KC_HOME, _______, _______, _______, _______, _______, _______, KC_END \
),
[_RAISE] = LAYOUT( \
  KC_GRV,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,                     KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  LGUI(LSFT(KC_LBRC)), \
  _______, _______, _______, _______, _______, KC_LPRN,                   KC_RPRN, _______, KC_VOLD, KC_VOLU, KC_MPLY, LGUI(LSFT(KC_RBRC)), \
  XXXXXXX, KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, KC_LBRC,                   KC_RBRC, KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, _______, \
  _______, _______, _______, _______, _______, KC_LCBR, _______, KC_DEL,  KC_RCBR, _______, _______, _______, _______, _______, \
                             KC_F11,  _______, _______, _______, _______, _______, _______, KC_F12 \
),
[_ADJUST] = LAYOUT( \
  TOG_OS,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_BRMU, \
  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_BRMD, \
  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                   XXXXXXX, RGB_SAI, RGB_SPI, RGB_HUI, RGB_MOD, RGB_VAI, \
  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, RGB_SAD, RGB_SPD, RGB_HUD, RGB_RMOD,RGB_VAD,\
                             _______, _______, _______, _______, _______,  _______, _______, RGB_TOG \
)
};

// int RGB_current_mode;

// Setting ADJUST layer RGB back to default
void update_tri_layer_RGB(uint8_t layer1, uint8_t layer2, uint8_t layer3) {
  if (IS_LAYER_ON(layer1) && IS_LAYER_ON(layer2)) {
    layer_on(layer3);
  } else {
    layer_off(layer3);
  }
}

//SSD1306 OLED update loop, make sure to enable OLED_DRIVER_ENABLE=yes in rules.mk
#ifdef OLED_DRIVER_ENABLE

oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    if (!is_keyboard_master()) return OLED_ROTATION_180;  // flips the display 180 degrees if offhand
    return rotation;
}

// When you add source files to SRC in rules.mk, you can use functions.
const char *read_layer_state(void);
const char *read_logo(void);

// const char *read_mode_icon(bool swap);
// const char *read_host_led_state(void);
// void set_timelog(void);
// const char *read_timelog(void);

char default_layer_state_str[24];
const char *read_default_layer_state(void) {
    switch (get_highest_layer(default_layer_state)) {
    case _MAC:
        snprintf(default_layer_state_str, sizeof(default_layer_state_str), "OS:      Mac");
        break;
    case _WIN:
        snprintf(default_layer_state_str, sizeof(default_layer_state_str), "OS:      Windows");
        break;
    }

    return default_layer_state_str;
}

#define L_BASE 0
#define L_LOWER (1 << 3)
#define L_RAISE (1 << 4)
#define L_ADJUST (1 << 5)
#define L_ADJUST_TRI (L_ADJUST | L_RAISE | L_LOWER)

const char *read_layer_state(void) {
    static char layer_state_str[24];
    switch (layer_state) {
    case L_BASE:
        snprintf(layer_state_str, sizeof(layer_state_str), "Layer:   Default");
        break;
    case L_RAISE:
        snprintf(layer_state_str, sizeof(layer_state_str), "Layer:   Raise");
        break;
    case L_LOWER:
        snprintf(layer_state_str, sizeof(layer_state_str), "Layer:   Lower");
        break;
    case L_ADJUST:
    case L_ADJUST_TRI:
        snprintf(layer_state_str, sizeof(layer_state_str), "Layer:   Adjust");
        break;
    default:
        snprintf(layer_state_str, sizeof(layer_state_str), "Layer:   Undef-%ld", layer_state);
    }

    return layer_state_str;
}

const char *read_rgblight_state(void) {
    static char rgblight_state[24];
    snprintf(rgblight_state, sizeof(rgblight_state), "RGB:     v%-3d s%1d m%-d", rgblight_config.val, rgblight_config.speed, rgblight_config.mode);
    return rgblight_state;
}

const char *read_encoder_state(void) {
    static char encoder_state_str[24];
    switch (encoder_mode) {
    case LEFTRIGHT:
        snprintf(encoder_state_str, sizeof(encoder_state_str), "Encoder: Left-right");
        break;
    case UPDOWN:
        snprintf(encoder_state_str, sizeof(encoder_state_str), "Encoder: Up-down");
        break;
    case SCROLL:
        snprintf(encoder_state_str, sizeof(encoder_state_str), "Encoder: Scroll");
        break;
    case TABS:
        snprintf(encoder_state_str, sizeof(encoder_state_str), "Encoder: Cycle tabs");
        break;
    }

    return encoder_state_str;
}

const char *read_wpm(void) {
    static char wpm_str[24];
    static uint8_t  max_wpm = 0;
    static uint32_t max_wpm_timer;

    if (max_wpm && timer_elapsed32(max_wpm_timer) > 60000) // Expire maximum WPM record after 60 seconds
        max_wpm = 0;
    if (get_current_wpm() > max_wpm) {
        max_wpm = get_current_wpm();
        max_wpm_timer = timer_read32();
    }
    sprintf(wpm_str, "WPM:     %03d  (%03d)", get_current_wpm(), max_wpm);
    return wpm_str;
}

void oled_task_user(void) {
    static uint32_t oled_task_timer;
    // Only update the OLED at 10Hz. It is quite expensive and this greatly improves the scan rate.
    if (timer_elapsed32(oled_task_timer) > 100) {
        oled_task_timer = timer_read32();
    } else {
        return;
    }

    if (is_keyboard_master()) {
        // If you want to change the display of OLED, you need to change here
        oled_write_ln(read_default_layer_state(), false);
        // oled_write_ln(read_layer_state(), false);
        oled_write_ln(read_encoder_state(), false);
        oled_write_ln(read_wpm(), false);
        oled_write_ln(read_rgblight_state(), false);

        // oled_write_ln(read_mode_icon(keymap_config.swap_lalt_lgui), false);
        // oled_write_ln(read_host_led_state(), false);
        // oled_write_ln(read_timelog(), false);
    } else {
        oled_write(read_logo(), false);
    }
}
#endif // OLED_DRIVER_ENABLE

void keyboard_post_init_user(void) {
    #ifdef CONSOLE_ENABLE
        debug_enable = true; // If debugging is compiled in then enable it to begin with
        // debug_matrix = true;
        // debug_keyboard = true;
    #endif
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        // set_timelog();
    }

    // Cycle through the encoder modes
    if (keycode == ENCODER) {
        if (record->event.pressed) {
            encoder_mode++;
            if (encoder_mode == encoder_modes_len) encoder_mode = 0;
        }
        return false;
    }

    // Reset the encoder mode if any alpha is pressed
    if (record->event.pressed) {
        switch (keycode) {
            case KC_LSFT:
            case KC_RSFT:
            case KC_LCTRL:
            case KC_LALT:
            case KC_LGUI:
            case LOWER:
            case RAISE:
                break;
            default:
                if (MODS_SHIFT || MODS_ALT || MODS_CTRL || MODS_GUI)
                    // Do not reset if any modifiers are held
                    break;
                encoder_mode = 0;
        }
    }

    switch (keycode) {
        case QWERTY:
            if (record->event.pressed) {
                set_single_persistent_default_layer(_MAC);
            }
            return false;
            break;
        case TOG_OS:
            if (!record->event.pressed) return false;
            switch (get_highest_layer(default_layer_state)) {
            case _MAC:
                set_single_persistent_default_layer(_WIN);
                break;
            case _WIN:
                set_single_persistent_default_layer(_MAC);
                break;
            }
            return false;
        case LOWER:
            if (record->event.pressed) {
                layer_on(_LOWER);
                update_tri_layer_RGB(_LOWER, _RAISE, _ADJUST);
            } else {
                layer_off(_LOWER);
                update_tri_layer_RGB(_LOWER, _RAISE, _ADJUST);
            }
            return false;
            break;
        case RAISE:
            if (record->event.pressed) {
                layer_on(_RAISE);
                update_tri_layer_RGB(_LOWER, _RAISE, _ADJUST);
            } else {
                layer_off(_RAISE);
                update_tri_layer_RGB(_LOWER, _RAISE, _ADJUST);
            }
            return false;
            break;
        case ADJUST:
            if (record->event.pressed) {
                layer_on(_ADJUST);
            } else {
                layer_off(_ADJUST);
            }
            return false;
            break;
    }
    return true;
}

#ifdef ENCODER_ENABLE
void encoder_update_user(uint8_t index, bool clockwise) {
    uint8_t _encoder_mode = encoder_mode;

    if (layer_state_is(_ADJUST)) {
        _encoder_mode = SCROLL;
        encoder_mode  = _encoder_mode; // persist
    } else if (layer_state_is(_LOWER)) {
        _encoder_mode = TABS;
        encoder_mode  = _encoder_mode; // persist
    } else if (layer_state_is(_RAISE)) {
        _encoder_mode = UPDOWN;
    }

    switch (_encoder_mode) {
    case LEFTRIGHT:
        if (clockwise) {
            tap_code(KC_RGHT);
        } else {
            tap_code(KC_LEFT);
        }
        break;
    case UPDOWN:
        if (clockwise) {
            tap_code(KC_DOWN);
        } else {
            tap_code(KC_UP);
        }
        break;
    case SCROLL:
        if (clockwise) {
            tap_code(KC_MS_WH_DOWN);
            tap_code(KC_MS_WH_DOWN);
        } else {
            tap_code(KC_MS_WH_UP);
            tap_code(KC_MS_WH_UP);
        }
        break;
    case TABS:
        if (clockwise) {
            register_code(KC_LGUI);
            register_code(KC_LSFT);
            tap_code(KC_RBRC);
            unregister_code(KC_LSFT);
            unregister_code(KC_LGUI);
        } else {
            register_code(KC_LGUI);
            register_code(KC_LSFT);
            tap_code(KC_LBRC);
            unregister_code(KC_LSFT);
            unregister_code(KC_LGUI);
        }
        break;
    }
}
#endif
