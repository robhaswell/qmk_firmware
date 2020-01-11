// !!! DO NOT ADD #pragma once !!! //

// Step 1.
// Declare custom effects using the RGB_MATRIX_EFFECT macro
// (note the lack of semicolon after the macro!)
RGB_MATRIX_EFFECT(RGB_CUSTOM)
RGB_MATRIX_EFFECT(RGB_CUSTOM_MULTICROSS)
RGB_MATRIX_EFFECT(RGB_CUSTOM_SOLID_REACTIVE_SIMPLE)

// Step 2.
// Define effects inside the `RGB_MATRIX_CUSTOM_EFFECT_IMPLS` ifdef block
#ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

static RGB rgb_sqrt_blend(RGB a, RGB b, uint8_t t) {
    RGB rgb;
    // Blend RGB channels
    rgb.r = sqrt16(scale16by8(a.r * a.r, (255 - t)) + scale16by8(b.r * b.r, t));
    rgb.g = sqrt16(scale16by8(a.g * a.g, (255 - t)) + scale16by8(b.g * b.g, t));
    rgb.b = sqrt16(scale16by8(a.b * a.b, (255 - t)) + scale16by8(b.b * b.b, t));
    return rgb;
}

static RGB static_color(uint8_t i, HSV hsv, uint8_t speed) {
    /* Calculate the static keyboard colour. */

    // The x range will be 0..224, map this to 0..7
    // Relies on hue being 8-bit and wrapping
    if (HAS_FLAGS(g_led_config.flags[i], LED_FLAG_UNDERGLOW)) {
        // Shift the underglow colour.
        hsv.h += RGB_MATRIX_UNDERGLOW_OFFSET_HUE;
        speed += RGB_MATRIX_UNDERGLOW_OFFSET_SPD;
    }
    uint8_t scale = scale8(64, speed);

    #ifdef RGB_MATRIX_BLEND_RGB
        // Calculate start and end colours
        RGB rgb_a = hsv_to_rgb(hsv);
        hsv.h    += scale * 224 >> 5;
        RGB rgb_b = hsv_to_rgb(hsv);
        return rgb_sqrt_blend(rgb_a, rgb_b, dx);
    #else
        hsv.h += scale * g_led_config.point[i].x >> 5;
        return hsv_to_rgb(hsv);
    #endif // #ifdef RGB_MATRIX_BLEND_RGB
}

static bool RGB_CUSTOM(effect_params_t* params) {
    RGB_MATRIX_USE_LIMITS(led_min, led_max);

    HSV hsv       = rgb_matrix_config.hsv;
    uint8_t speed = rgb_matrix_config.speed;

    for (uint8_t i = led_min; i < led_max; i++) {
        RGB_MATRIX_TEST_LED_FLAGS();

        RGB rgb = static_color(i, hsv, speed);
        rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
    }
    return led_max < DRIVER_LED_TOTAL;
}

typedef RGB (*custom_reactive_splash_f)(RGB rgb, int16_t dx, int16_t dy, uint8_t dist, uint16_t tick);

static bool custom_effect_runner_reactive_splash(uint8_t start, effect_params_t* params, custom_reactive_splash_f effect_func) {
    RGB_MATRIX_USE_LIMITS(led_min, led_max);

    HSV hsv       = rgb_matrix_config.hsv;
    uint8_t speed = rgb_matrix_config.speed;

    uint8_t count = g_last_hit_tracker.count;
    for (uint8_t i = led_min; i < led_max; i++) {
        RGB_MATRIX_TEST_LED_FLAGS();

        // Calculate the base gradient colour.
        RGB rgb = static_color(i, hsv, speed);

        for (uint8_t j = start; j < count; j++) {
            int16_t  dx   = g_led_config.point[i].x - g_last_hit_tracker.x[j];
            int16_t  dy   = g_led_config.point[i].y - g_last_hit_tracker.y[j];
            uint8_t  dist = sqrt16(dx * dx + dy * dy);
            uint16_t tick = scale16by8(g_last_hit_tracker.tick[j], RGB_MATRIX_REACTIVE_CROSS_SPEED);
            rgb           = effect_func(rgb, dx, dy, dist, tick);
        }

        rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
    }
    return led_max < DRIVER_LED_TOTAL;
}

typedef RGB (*custom_reactive_f)(RGB rgb, uint16_t offset);

static bool custom_effect_runner_reactive(effect_params_t* params, custom_reactive_f effect_func) {
    RGB_MATRIX_USE_LIMITS(led_min, led_max);

    HSV hsv       = rgb_matrix_config.hsv;
    uint8_t speed = rgb_matrix_config.speed;

    uint16_t max_tick = 65535 / RGB_MATRIX_REACTIVE_SIMPLE_SPEED;
    for (uint8_t i = led_min; i < led_max; i++) {
        RGB_MATRIX_TEST_LED_FLAGS();
        uint16_t tick = max_tick;
        // Reverse search to find most recent key hit
        for (int8_t j = g_last_hit_tracker.count - 1; j >= 0; j--) {
            if (g_last_hit_tracker.index[j] == i && g_last_hit_tracker.tick[j] < tick) {
                tick = g_last_hit_tracker.tick[j];
                break;
            }
        }
        RGB rgb = static_color(i, hsv, speed);

        uint16_t offset = scale16by8(tick, RGB_MATRIX_REACTIVE_SIMPLE_SPEED);
        rgb             = effect_func(rgb, offset);
        rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
    }
    return led_max < DRIVER_LED_TOTAL;
}

static RGB CUSTOM_SOLID_REACTIVE_CROSS_math(RGB bg, int16_t dx, int16_t dy, uint8_t dist, uint16_t tick) {
    uint16_t effect = tick - dist;
    // effect = 0 fully highlighted, 255 fully faded
    if (effect >= 255 || dist > 72) return bg;
    if ((dx > 8 || dx < -8) && (dy > 8 || dy < -8)) return bg;


    // Blend highlight over the gradient colour
    RGB fg = RGB_MATRIX_REACTIVE_HIGHLIGHT;
    // Compress the effect so the highlight is more pronounced
    bg     = rgb_sqrt_blend(fg, bg, effect * effect / 255);
    return bg;
}

static bool RGB_CUSTOM_MULTICROSS(effect_params_t* params) {
    return custom_effect_runner_reactive_splash(qsub8(g_last_hit_tracker.count, 8),  // Use only the previous 8 key samples for performance
                                                params, &CUSTOM_SOLID_REACTIVE_CROSS_math);
}

static RGB CUSTOM_SOLID_REACTIVE_SIMPLE_math(RGB bg, uint16_t effect) {
    // effect = 0 fully highlighted, 255 fully faded
    if (effect >= 255) return bg;

    // Hold the effect after initial keypress for a short time
    uint8_t shifted = qsub8(effect, RGB_MATRIX_REACTIVE_SIMPLE_THRESHOLD);
    effect          = shifted * 255 / (255 - RGB_MATRIX_REACTIVE_SIMPLE_THRESHOLD);

    // Blend highlight over the gradient colour
    RGB fg = RGB_MATRIX_REACTIVE_HIGHLIGHT;
    // Compress the effect so the highlight is more pronounced
    bg = rgb_sqrt_blend(fg, bg, effect * effect / 255);
    return bg;
}

static bool RGB_CUSTOM_SOLID_REACTIVE_SIMPLE(effect_params_t* params) { return custom_effect_runner_reactive(params, &CUSTOM_SOLID_REACTIVE_SIMPLE_math); }

#endif  // RGB_MATRIX_CUSTOM_EFFECT_IMPLS
