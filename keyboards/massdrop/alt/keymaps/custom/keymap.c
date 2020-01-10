#include QMK_KEYBOARD_H

enum alt_keycodes {
    U_T_AUTO = SAFE_RANGE, //USB Extra Port Toggle Auto Detect / Always Active
    U_T_AGCR,              //USB Toggle Automatic GCR control
    DBG_TOG,               //DEBUG Toggle On / Off
    DBG_MTRX,              //DEBUG Toggle Matrix Prints
    DBG_KBD,               //DEBUG Toggle Keyboard Prints
    DBG_MOU,               //DEBUG Toggle Mouse Prints
    MD_BOOT,               //Restart into bootloader after hold timeout
    RH_DEMO,               // Demo mode toggle
};

keymap_config_t keymap_config;

//Tap Dance Declarations
enum {
  CTRL = 0
};

//Tap Dance Definitions
qk_tap_dance_action_t tap_dance_actions[] = {
  [CTRL] = ACTION_TAP_DANCE_DOUBLE(KC_LCTL, KC_CAPS),
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT_65_ansi_blocker(
        KC_GESC, KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSPC, KC_HOME, \
        KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_NUBS, KC_END, \
        KC_LCTL, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,          KC_ENT,  KC_PGUP, \
        LM(1, MOD_LSFT), KC_Z, KC_X, KC_C,   KC_V,   KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT,          KC_UP,   KC_PGDN, \
        MO(2),   KC_LGUI, KC_LALT,                            KC_SPC,                             KC_RALT, MO(2),   KC_LEFT, KC_DOWN, KC_RGHT  \
    ),
    [1] = LAYOUT_65_ansi_blocker(
        _______, _______, KC_QUOT, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, \
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, \
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_2,             _______, _______, \
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______, _______, \
        _______, _______, _______,                            _______,                            _______, _______, _______, _______, _______  \
    ),
    [2] = LAYOUT_65_ansi_blocker(
        KC_GRV,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  KC_DEL,  KC_MPLY, \
        _______, RGB_SPD, RGB_VAI, RGB_SPI, RGB_HUI, RGB_SAI, _______, U_T_AUTO,U_T_AGCR,_______, KC_PSCR, KC_SLCK, KC_PAUS, KC_BSLS, KC_END, \
        TD(CTRL),RGB_RMOD,RGB_VAD, RGB_MOD, RGB_HUD, RGB_SAD, _______, _______, _______, _______, _______, _______,          _______, KC_VOLU, \
        _______, RGB_TOG, _______, _______, _______, MD_BOOT, NK_TOGG, DBG_TOG, DBG_MTRX,DBG_KBD, _______, _______,          KC_PGUP, KC_VOLD, \
        RH_DEMO, _______, _______,                            _______,                            _______, RH_DEMO, KC_HOME, KC_PGDN, KC_END  \
    ),
    [3] = LAYOUT_65_ansi_blocker( // A demo keymap
        KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   \
        KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   \
        KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,               KC_NO,   KC_NO,   \
        KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,            KC_NO,   KC_NO,   \
        MO(4),   KC_NO,   KC_NO,                              KC_NO,                              KC_NO,   MO(4),   KC_NO,   KC_NO,   KC_NO    \
    ),
    [4] = LAYOUT_65_ansi_blocker( // Return from demo keymap
        KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   \
        KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   \
        KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,               KC_NO,   KC_NO,   \
        KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,            KC_NO,   KC_NO,   \
        RH_DEMO, KC_NO,   KC_NO,                              KC_NO,                              KC_NO,   RH_DEMO, KC_NO,   KC_NO,   KC_NO    \
    ),
    /*
    [X] = LAYOUT(
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, \
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, \
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______, _______, \
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______, _______, \
        _______, _______, _______,                            _______,                            _______, _______, _______, _______, _______  \
    ),
    */
};

static uint32_t flash_timer;
static uint32_t key_timer;

static bool idle;
static bool reset;
static bool demo_toggle;
static bool rgb_flash;
// Special win+3 handling
static bool lgui_pressed;
static bool lgui_registered;
static bool bsls_registered;

static uint8_t user_hsv_v;


// Runs just one time when the keyboard initializes.
void matrix_init_user(void) {
};

// Runs constantly in the background, in a loop.
void matrix_scan_user(void) {
    if (reset && timer_elapsed32(key_timer) > 500) {
        reset_keyboard();
    }

    if (demo_toggle && timer_elapsed32(key_timer) > 1000) {
        dprintf("Demo mode toggle\n");
        demo_toggle = false;

        if (layer_state_is(3)) {
            layer_off(3);
        } else {
            layer_on(3);
        }

        rgb_matrix_config.hsv.h += 128;
        flash_timer = timer_read32();
        rgb_flash = true;
    }

    if (rgb_flash && timer_elapsed32(flash_timer) > 500) {
        rgb_flash = false;
        rgb_matrix_config.hsv.h += 128;
    }

    static uint32_t idle_timeout = IDLE_TIMEOUT * 1000;

    if (!idle && timer_elapsed32(key_timer) > idle_timeout) { // Detect idleness
        idle = true;
        user_hsv_v = rgb_matrix_config.hsv.v;
        rgb_matrix_config.hsv.v = IDLE_BRIGHTNESS;
    }
};

#define MODS_SHIFT  (get_mods() & MOD_BIT(KC_LSHIFT) || get_mods() & MOD_BIT(KC_RSHIFT))
#define MODS_CTRL  (get_mods() & MOD_BIT(KC_LCTL) || get_mods() & MOD_BIT(KC_RCTRL))
#define MODS_ALT  (get_mods() & MOD_BIT(KC_LALT) || get_mods() & MOD_BIT(KC_RALT))

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    key_timer = timer_read32();

    if (idle) { // Detect return from idle
        idle = false;
        rgb_matrix_config.hsv.v = user_hsv_v;
    }

    if (lgui_pressed && record->event.pressed) {
        // Another key has been pressed with LGUI
        if (!lgui_registered) {
            // If LGUI has already been sent we can't really do anything
            if (keycode == KC_3) {
                register_code(KC_BSLS);
                bsls_registered = true;
                return false; // no further key handling
            } else {
                register_code(KC_LGUI);
                lgui_registered = true;
            }
        }
    }

    switch (keycode) {
        // case KC_3:
        //     if (MODS_SHIFT) return true; // Ignore if shift is held.
        //     if (record->event.pressed && (get_mods() & MOD_BIT(KC_LALT))) {
        //         SEND_STRING(SS_UP(X_LALT) SS_TAP(X_BSLS) SS_DOWN(X_LALT));
        //         return false;
        //     }
        //     return true;
        case RH_DEMO:
            demo_toggle = record->event.pressed;
            return false;
        case KC_LGUI:
            // When the windows key is pressed, wait before sending the keycode in case KC_3 is pressed, in which case send KC_BSLS
            lgui_pressed = record->event.pressed;
            if (!lgui_pressed) {
                // Keyup
                if (lgui_registered) {
                    unregister_code(KC_LGUI);
                } else if (!bsls_registered) {
                    // No other key was pressed so just send LGUI up and down
                    tap_code(KC_LGUI);
                }
                // LGUI is up now so let's reset the state
                bsls_registered = false;
                lgui_registered = false;
            }
            return false;
        case U_T_AUTO:
            if (record->event.pressed && MODS_SHIFT && MODS_CTRL) {
                TOGGLE_FLAG_AND_PRINT(usb_extra_manual, "USB extra port manual mode");
            }
            return false;
        case U_T_AGCR:
            if (record->event.pressed && MODS_SHIFT && MODS_CTRL) {
                TOGGLE_FLAG_AND_PRINT(usb_gcr_auto, "USB GCR auto mode");
            }
            return false;
        case DBG_TOG:
            if (record->event.pressed) {
                TOGGLE_FLAG_AND_PRINT(debug_enable, "Debug mode");
            }
            return false;
        case DBG_MTRX:
            if (record->event.pressed) {
                TOGGLE_FLAG_AND_PRINT(debug_matrix, "Debug matrix");
            }
            return false;
        case DBG_KBD:
            if (record->event.pressed) {
                TOGGLE_FLAG_AND_PRINT(debug_keyboard, "Debug keyboard");
            }
            return false;
        case DBG_MOU:
            if (record->event.pressed) {
                TOGGLE_FLAG_AND_PRINT(debug_mouse, "Debug mouse");
            }
            return false;
        case MD_BOOT:
            reset = record->event.pressed;
            return false;
        case RGB_TOG:
            if (record->event.pressed) {
              switch (rgb_matrix_get_flags()) {
                case LED_FLAG_ALL: {
                    rgb_matrix_set_flags(LED_FLAG_KEYLIGHT);
                    rgb_matrix_set_color_all(0, 0, 0);
                  }
                  break;
                case LED_FLAG_KEYLIGHT: {
                    rgb_matrix_set_flags(LED_FLAG_UNDERGLOW);
                    rgb_matrix_set_color_all(0, 0, 0);
                  }
                  break;
                case LED_FLAG_UNDERGLOW: {
                    rgb_matrix_set_flags(LED_FLAG_NONE);
                    rgb_matrix_disable_noeeprom();
                  }
                  break;
                default: {
                    rgb_matrix_set_flags(LED_FLAG_ALL);
                    rgb_matrix_enable_noeeprom();
                  }
                  break;
              }
            }
            return false;
        default:
            return true; //Process all other keycodes normally
    }
}
