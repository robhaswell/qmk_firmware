// !!! DO NOT ADD #pragma once !!! //

// Step 1.
// Declare custom effects using the RGB_MATRIX_EFFECT macro
// (note the lack of semicolon after the macro!)
RGB_MATRIX_EFFECT(RGB_CUSTOM)
RGB_MATRIX_EFFECT(RGB_CUSTOM_MULTICROSS)
RGB_MATRIX_EFFECT(RGB_CUSTOM_SOLID_REACTIVE_SIMPLE)
RGB_MATRIX_EFFECT(RGB_CUSTOM_CYCLE)

// Step 2.
// Define effects inside the `RGB_MATRIX_CUSTOM_EFFECT_IMPLS` ifdef block
#ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

extern rgb_config_t rgb_matrix_alt_config;

static uint8_t gradient_hue(uint8_t i) {
    /* Calculate the custom gradient, and apply an offset gradient to the underglow. */

    // The x range will be 0..224, map this to 0..7
    // Relies on hue being 8-bit and wrapping
    uint8_t hue   = rgb_matrix_config.hsv.h;
    uint8_t speed = rgb_matrix_config.speed;

    if (HAS_FLAGS(g_led_config.flags[i], LED_FLAG_UNDERGLOW)) {
        // Shift the underglow colour.
        hue   += rgb_matrix_alt_config.hsv.h;
        speed += rgb_matrix_alt_config.speed;
        #ifdef RGB_MATRIX_BLEND_SQRT
            uint8_t scale = scale8(64, speed);
            return hue + (scale * g_led_config.point[i].x >> 5);
        #endif
    }
    uint8_t scale = scale8(64, speed);

    #ifdef RGB_MATRIX_BLEND_SQRT
        int16_t offset16 = g_led_config.point[i].x - 112;  // -112 -> 112
        offset16         = offset16 * 112;
        if (offset16 < 0) {
            offset16 = -sqrt16(-offset16);
        } else {
            offset16 = sqrt16(offset16);
        }
        uint8_t offset = offset16 + 112;
        return hue + (scale * offset >> 5);
    #else
        return hue + (scale * g_led_config.point[i].x >> 5);
    #endif
}

static bool RGB_CUSTOM(effect_params_t* params) {
    RGB_MATRIX_USE_LIMITS(led_min, led_max);

    HSV hsv = rgb_matrix_config.hsv;
    for (uint8_t i = led_min; i < led_max; i++) {
        RGB_MATRIX_TEST_LED_FLAGS();

        hsv.h   = gradient_hue(i);
        RGB rgb = hsv_to_rgb(hsv);

        rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
    }
    return led_max < DRIVER_LED_TOTAL;
}

static bool RGB_CUSTOM_CYCLE(effect_params_t* params) {
    RGB_MATRIX_USE_LIMITS(led_min, led_max);

    HSV hsv = rgb_matrix_config.hsv;
    for (uint8_t i = led_min; i < led_max; i++) {
        RGB_MATRIX_TEST_LED_FLAGS();

        hsv.h   = gradient_hue(i);
        hsv.h  += scale16by8(g_rgb_timer, rgb_matrix_config.speed / 24);
        RGB rgb = hsv_to_rgb(hsv);

        rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
    }
    return led_max < DRIVER_LED_TOTAL;
}

static RGB rgb_sqrt_blend(RGB a, RGB b, uint8_t t) {
    RGB rgb;
    // Compress the effect so the highlight is more pronounced
    t     = t * t / 255;
    // Blend RGB channels
    rgb.r = sqrt16(scale16by8(a.r * a.r, (255 - t)) + scale16by8(b.r * b.r, t));
    rgb.g = sqrt16(scale16by8(a.g * a.g, (255 - t)) + scale16by8(b.g * b.g, t));
    rgb.b = sqrt16(scale16by8(a.b * a.b, (255 - t)) + scale16by8(b.b * b.b, t));

    return rgb;
}

static RGB CUSTOM_SOLID_REACTIVE_CROSS_math(RGB bg, int16_t dx, int16_t dy, uint8_t dist, uint16_t tick) {
    uint16_t effect = tick - dist;
    // effect = 0 fully highlighted, 255 fully faded
    if (effect >= 255 || dist > 54) return bg; // effect size, default=72
    if ((dx > 8 || dx < -8) && (dy > 8 || dy < -8)) return bg; // effect spread, default=8

    // if (rand()/RAND_MAX < 0.0001) dprintf("effect: %u\n", effect);

    // Blend highlight over the gradient colour
    static int fg_hue;
    static RGB fg = {0x00, 0xFF, 0x00};
    if (rgb_matrix_alt_config.hsv.s != fg_hue) {
        fg_hue     = rgb_matrix_alt_config.hsv.s;
        HSV fg_hsv = {fg_hue, 0xFF, 0xFF};
        fg         = hsv_to_rgb(fg_hsv);
    }
    bg = rgb_sqrt_blend(fg, bg, effect);
    return bg;
}

typedef RGB (*custom_reactive_splash_f)(RGB rgb, int16_t dx, int16_t dy, uint8_t dist, uint16_t tick);

static bool custom_effect_runner_reactive_splash(uint8_t start, effect_params_t* params, custom_reactive_splash_f effect_func) {
    RGB_MATRIX_USE_LIMITS(led_min, led_max);

    uint8_t count = g_last_hit_tracker.count;
    for (uint8_t i = led_min; i < led_max; i++) {
        RGB_MATRIX_TEST_LED_FLAGS();
        HSV hsv = rgb_matrix_config.hsv;

        // Calculate the base gradient colour.
        hsv.h   = gradient_hue(i);
        RGB rgb = hsv_to_rgb(hsv);

        for (uint8_t j = start; j < count; j++) {
            if (g_last_hit_tracker.tick[j] > 2000) continue; // Optimise for keys which are long-expired.
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

static bool RGB_CUSTOM_MULTICROSS(effect_params_t* params) {
    return custom_effect_runner_reactive_splash(qsub8(g_last_hit_tracker.count, 12),  // Use only the previous 12 key samples for performance
                                                params, &CUSTOM_SOLID_REACTIVE_CROSS_math);
}

typedef RGB (*custom_reactive_f)(RGB rgb, uint16_t offset);

static bool custom_effect_runner_reactive(effect_params_t* params, custom_reactive_f effect_func) {
    RGB_MATRIX_USE_LIMITS(led_min, led_max);

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
        HSV hsv = rgb_matrix_config.hsv;

        // Calculate the base gradient colour
        hsv.h   = gradient_hue(i);
        RGB rgb = hsv_to_rgb(hsv);

        uint16_t offset = scale16by8(tick, RGB_MATRIX_REACTIVE_SIMPLE_SPEED);
        rgb             = effect_func(rgb, offset);
        rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
    }
    return led_max < DRIVER_LED_TOTAL;
}

static RGB CUSTOM_SOLID_REACTIVE_SIMPLE_math(RGB bg, uint16_t effect) {
    // effect = 0 fully highlighted, 255 fully faded
    if (effect >= 255) return bg;

    // Hold the effect after initial keypress for a short time
    uint8_t shifted = qsub8(effect, RGB_MATRIX_REACTIVE_SIMPLE_THRESHOLD);
    effect          = shifted * 255 / (255 - RGB_MATRIX_REACTIVE_SIMPLE_THRESHOLD);
    // if (rand()/RAND_MAX < 0.0001) dprintf("shifted: %u, effect: %u\n", shifted, effect);

    // Blend highlight over the gradient colour
    static int fg_hue;
    static RGB fg = {0x00, 0xFF, 0x00};
    if (rgb_matrix_alt_config.hsv.s != fg_hue) {
        fg_hue     = rgb_matrix_alt_config.hsv.s;
        HSV fg_hsv = {fg_hue, 0xFF, 0xFF};
        fg         = hsv_to_rgb(fg_hsv);
    }
    bg = rgb_sqrt_blend(fg, bg, effect);
    return bg;
}

static bool RGB_CUSTOM_SOLID_REACTIVE_SIMPLE(effect_params_t* params) { return custom_effect_runner_reactive(params, &CUSTOM_SOLID_REACTIVE_SIMPLE_math); }

#endif  // RGB_MATRIX_CUSTOM_EFFECT_IMPLS
