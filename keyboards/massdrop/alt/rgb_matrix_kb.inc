/* A custom suite of effects.

   Author: Rob Haswell <rob@haswell.co.uk> */
// #include "rgb_effects.c"

RGB_MATRIX_EFFECT(RGB_LEFT_RIGHT)

#ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS
static bool rgb_gradient_left_right(effect_params_t* params, uint8_t kb_start[], uint8_t kb_end[], uint8_t ug_start[], uint8_t ug_end[]) {
    RGB_MATRIX_USE_LIMITS(led_min, led_max);

    const RGB kb_start_rgb = (RGB){kb_start[1], kb_start[0], kb_start[2]};
    const RGB kb_end_rgb = (RGB){kb_end[1], kb_end[0], kb_end[2]};
    const RGB ug_start_rgb = (RGB){ug_start[1], ug_start[0], ug_start[2]};
    const RGB ug_end_rgb = (RGB){ug_end[1], ug_end[0], ug_end[2]};

    for (uint8_t i = led_min; i < led_max; i++) {
        RGB_MATRIX_TEST_LED_FLAGS();

        uint8_t dx = g_led_config.point[i].x;

        // Calculate the channel values
        RGB rgb;
        if (HAS_FLAGS(g_led_config.flags[i], LED_FLAG_UNDERGLOW)) {
            rgb = (RGB){
                ug_start_rgb.g + (ug_end_rgb.g - ug_start_rgb.g) * dx / 224,
                ug_start_rgb.r + (ug_end_rgb.r - ug_start_rgb.r) * dx / 224,
                ug_start_rgb.b + (ug_end_rgb.b - ug_start_rgb.b) * dx / 224,
            };
        } else {
            rgb = (RGB){
                kb_start_rgb.g + (kb_end_rgb.g - kb_start_rgb.g) * dx / 224,
                kb_start_rgb.r + (kb_end_rgb.r - kb_start_rgb.r) * dx / 224,
                kb_start_rgb.b + (kb_end_rgb.b - kb_start_rgb.b) * dx / 224,
            };
        }

        // Scale the output brightness
        rgb.r = scale8(rgb.r, rgb_matrix_config.hsv.v);
        rgb.g = scale8(rgb.g, rgb_matrix_config.hsv.v);
        rgb.b = scale8(rgb.b, rgb_matrix_config.hsv.v);

        rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
    }
    return led_max < DRIVER_LED_TOTAL;
}

static bool RGB_LEFT_RIGHT(effect_params_t* params) {
    uint8_t KB_LEFT[] = {RGB_GREEN};
    uint8_t KB_RIGHT[] = {RGB_MAGENTA};
    uint8_t UG_LEFT[] = {RGB_BLUE};
    uint8_t UG_RIGHT[] = {RGB_CYAN};
    return rgb_gradient_left_right(params, KB_LEFT, KB_RIGHT, UG_LEFT, UG_RIGHT);
}
#endif