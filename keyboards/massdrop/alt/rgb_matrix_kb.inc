// !!! DO NOT ADD #pragma once !!! //

// Step 1.
// Declare custom effects using the RGB_MATRIX_EFFECT macro
// (note the lack of semicolon after the macro!)
RGB_MATRIX_EFFECT(CUSTOM_EFFECT)
RGB_MATRIX_EFFECT(CUSTOM_EFFECT_NEXUS)

// Step 2.
// Define effects inside the `RGB_MATRIX_CUSTOM_EFFECT_IMPLS` ifdef block
#ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

HSV calc_rgb_gradient(uint8_t i) {
    // The y range will be 0..224, map this to 0..5
    // Relies on hue being 8-bit and wrapping

    HSV     hsv   = rgb_matrix_config.hsv;
    uint8_t hue   = rgb_matrix_config.hsv.h;
    uint8_t speed = rgb_matrix_config.speed;

    // Shift hue and speed for edge lighting
    if (HAS_FLAGS(g_led_config.flags[i], LED_FLAG_UNDERGLOW)) {
        hue = 0;
        speed = 16;
    }
    uint8_t scale = scale8(64, speed);

    hsv.h   = hue + (scale * g_led_config.point[i].x >> 5);
    return hsv;
}

// e.g: A simple effect, self-contained within a single method
static bool CUSTOM_EFFECT(effect_params_t* params) {
    RGB_MATRIX_USE_LIMITS(led_min, led_max);

    for (uint8_t i = led_min; i < led_max; i++) {
        RGB_MATRIX_TEST_LED_FLAGS();
        HSV hsv = calc_rgb_gradient(i);
        RGB rgb = hsv_to_rgb(hsv);
        rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
    }
    return led_max < DRIVER_LED_TOTAL;
}

static HSV CUSTOM_EFFECT_NEXUS_math(HSV hsv, int16_t dx, int16_t dy, uint8_t dist, uint16_t tick) {
    uint16_t effect = tick - dist;
    if (effect > 255) effect = 255;
    if (dist > 72) effect = 255;
    if ((dx > 8 || dx < -8) && (dy > 8 || dy < -8)) effect = 255;

    // effect = 0 at full effect -> 255 fully faded
    hsv.v = qadd8(hsv.v, 255 - effect);
    // TODO: Use this effect value to blend a highlight colour and the background
    return hsv;
}

typedef HSV (*custom_reactive_splash_f)(HSV hsv, int16_t dx, int16_t dy, uint8_t dist, uint16_t tick);

bool custom_effect_runner_reactive_splash(uint8_t start, effect_params_t* params, custom_reactive_splash_f effect_func) {
    RGB_MATRIX_USE_LIMITS(led_min, led_max);

    uint8_t count = g_last_hit_tracker.count;
    for (uint8_t i = led_min; i < led_max; i++) {
        RGB_MATRIX_TEST_LED_FLAGS();
        // TODO: Starting value for the LED goes here
        HSV hsv = calc_rgb_gradient(i);
        hsv.v   = 0;
        for (uint8_t j = start; j < count; j++) {
            int16_t  dx   = g_led_config.point[i].x - g_last_hit_tracker.x[j];
            int16_t  dy   = g_led_config.point[i].y - g_last_hit_tracker.y[j];
            uint8_t  dist = sqrt16(dx * dx + dy * dy);
            uint16_t tick = scale16by8(g_last_hit_tracker.tick[j], rgb_matrix_config.speed);
            hsv           = effect_func(hsv, dx, dy, dist, tick);
        }
        // TODO: Scale brightness
        hsv.v   = scale8(hsv.v, rgb_matrix_config.hsv.v);
        RGB rgb = hsv_to_rgb(hsv);
        rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
    }
    return led_max < DRIVER_LED_TOTAL;
}

bool CUSTOM_EFFECT_NEXUS(effect_params_t* params) { return custom_effect_runner_reactive_splash(0, params, &CUSTOM_EFFECT_NEXUS_math); }


#endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS
