/* A custom suite of effects.

   Author: Rob Haswell <rob@haswell.co.uk> */

// A smooth gradient from one RGB value to another

static RGB calc_rgb_gradient_left_right(uint8_t i, RGB kb_start_rgb, RGB kb_end_rgb, RGB ug_start_rgb, RGB ug_end_rgb) {
    uint8_t dx = g_led_config.point[i].x;

    // Calculate the channel values
    if (HAS_FLAGS(g_led_config.flags[i], LED_FLAG_UNDERGLOW)) {
        return (RGB){
            ug_start_rgb.g + (ug_end_rgb.g - ug_start_rgb.g) * dx / 224,
            ug_start_rgb.r + (ug_end_rgb.r - ug_start_rgb.r) * dx / 224,
            ug_start_rgb.b + (ug_end_rgb.b - ug_start_rgb.b) * dx / 224,
        };
    } else {
        return (RGB){
            kb_start_rgb.g + (kb_end_rgb.g - kb_start_rgb.g) * dx / 224,
            kb_start_rgb.r + (kb_end_rgb.r - kb_start_rgb.r) * dx / 224,
            kb_start_rgb.b + (kb_end_rgb.b - kb_start_rgb.b) * dx / 224,
        };
    }
}

static HSV calc_hsv_gradient_left_right(uint8_t i, HSV kb_start_hsv, HSV kb_end_hsv, HSV ug_start_hsv, HSV ug_end_hsv) {
    uint8_t dx = g_led_config.point[i].x;

    // Calculate the channel values
    HSV hsv;
    if (HAS_FLAGS(g_led_config.flags[i], LED_FLAG_UNDERGLOW)) {
        hsv.h = ug_start_hsv.h + (ug_end_hsv.h - ug_start_hsv.h) * dx / 224;
        hsv.s = ug_start_hsv.s + (ug_end_hsv.s - ug_start_hsv.s) * dx / 224;
        hsv.v = ug_start_hsv.v + (ug_end_hsv.v - ug_start_hsv.v) * dx / 224;
    } else {
        hsv.h = kb_start_hsv.h + (kb_end_hsv.h - kb_start_hsv.h) * dx / 224;
        hsv.s = kb_start_hsv.s + (kb_end_hsv.s - kb_start_hsv.s) * dx / 224;
        hsv.v = kb_start_hsv.v + (kb_end_hsv.v - kb_start_hsv.v) * dx / 224;
    }
    return hsv;
}

static bool rgb_gradient_left_right(effect_params_t* params, uint8_t kb_start[], uint8_t kb_end[], uint8_t ug_start[], uint8_t ug_end[]) {
    RGB_MATRIX_USE_LIMITS(led_min, led_max)

    const RGB kb_start_rgb = (RGB){kb_start[1], kb_start[0], kb_start[2]};
    const RGB kb_end_rgb = (RGB){kb_end[1], kb_end[0], kb_end[2]};
    const RGB ug_start_rgb = (RGB){ug_start[1], ug_start[0], ug_start[2]};
    const RGB ug_end_rgb = (RGB){ug_end[1], ug_end[0], ug_end[2]};

    for (uint8_t i = led_min; i < led_max; i++) {
        RGB_MATRIX_TEST_LED_FLAGS();

        RGB rgb = calc_rgb_gradient_left_right(i, kb_start_rgb, kb_end_rgb, ug_start_rgb, ug_end_rgb);

        // Scale the output brightness
        rgb.r = scale8(rgb.r, rgb_matrix_config.hsv.v);
        rgb.g = scale8(rgb.g, rgb_matrix_config.hsv.v);
        rgb.b = scale8(rgb.b, rgb_matrix_config.hsv.v);

        rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
    }
    return led_max < DRIVER_LED_TOTAL;
}

// A smooth gradient from one HSV value to another
static bool hsv_gradient_left_right(effect_params_t* params, uint8_t kb_start[], uint8_t kb_end[], uint8_t ug_start[], uint8_t ug_end[]) {
    RGB_MATRIX_USE_LIMITS(led_min, led_max)

    const HSV kb_start_hsv = (HSV){kb_start[0], kb_start[1], kb_start[2]};
    const HSV kb_end_hsv = (HSV){kb_end[0], kb_end[1], kb_end[2]};
    const HSV ug_start_hsv = (HSV){ug_start[0], ug_start[1], ug_start[2]};
    const HSV ug_end_hsv = (HSV){ug_end[0], ug_end[1], ug_end[2]};

    for (uint8_t i = led_min; i < led_max; i++) {
        RGB_MATRIX_TEST_LED_FLAGS();

        HSV hsv = calc_hsv_gradient_left_right(i, kb_start_hsv, kb_end_hsv, ug_start_hsv, ug_end_hsv);

        // Scale the output brightness
        hsv.v = scale8(hsv.v, rgb_matrix_config.hsv.v);
        RGB rgb = hsv_to_rgb(hsv);
        rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
    }
    return led_max < DRIVER_LED_TOTAL;
}

