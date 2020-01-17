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
    hsv.v   = 255; // Overall brightness will be calculated later
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

static RGB CUSTOM_EFFECT_NEXUS_math(RGB rgb, int16_t dx, int16_t dy, uint8_t dist, uint16_t tick) {
    uint16_t effect = tick - dist;
    if (effect > 255) effect = 255;
    if (dist > 72) effect = 255;
    if ((dx > 8 || dx < -8) && (dy > 8 || dy < -8)) effect = 255;

    // Define our highlight colour
    RGB hl;
    hl.r = 255;
    hl.g = 100;
    hl.b = 0;

    // effect = 0 at full effect -> 255 fully faded
    rgb.r = blend8(hl.r, rgb.r, effect); // rgb.r = hl.r + (rgb.r - hl.r) / 255;
    rgb.g = blend8(hl.g, rgb.g, effect);
    rgb.b = blend8(hl.b, rgb.b, effect);
    return rgb;
}

typedef RGB (*custom_reactive_splash_f)(RGB rgb, int16_t dx, int16_t dy, uint8_t dist, uint16_t tick);

bool custom_effect_runner_reactive_splash(uint8_t start, effect_params_t* params, custom_reactive_splash_f effect_func) {
    RGB_MATRIX_USE_LIMITS(led_min, led_max);

    uint8_t count = g_last_hit_tracker.count;
    for (uint8_t i = led_min; i < led_max; i++) {
        RGB_MATRIX_TEST_LED_FLAGS();
        HSV hsv = calc_rgb_gradient(i);
        RGB rgb = hsv_to_rgb(hsv);
        for (uint8_t j = start; j < count; j++) {
            int16_t  dx   = g_led_config.point[i].x - g_last_hit_tracker.x[j];
            int16_t  dy   = g_led_config.point[i].y - g_last_hit_tracker.y[j];
            uint8_t  dist = sqrt16(dx * dx + dy * dy);
            uint16_t tick = scale16by8(g_last_hit_tracker.tick[j], rgb_matrix_config.speed);
            rgb           = effect_func(rgb, dx, dy, dist, tick);
        }
        // Calculate the brightness
        rgb.r = scale8(rgb.r, rgb_matrix_config.hsv.v); // rgb.r = rgb.r * rgb_matrix_config.hsv.v / 255;
        rgb.g = scale8(rgb.g, rgb_matrix_config.hsv.v);
        rgb.b = scale8(rgb.b, rgb_matrix_config.hsv.v);
        
        rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
    }
    return led_max < DRIVER_LED_TOTAL;
}

bool CUSTOM_EFFECT_NEXUS(effect_params_t* params) { return custom_effect_runner_reactive_splash(0, params, &CUSTOM_EFFECT_NEXUS_math); }


#endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS
