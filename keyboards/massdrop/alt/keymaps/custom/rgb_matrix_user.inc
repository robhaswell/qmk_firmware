// !!! DO NOT ADD #pragma once !!! //

// Step 1.
// Declare custom effects using the RGB_MATRIX_EFFECT macro
// (note the lack of semicolon after the macro!)
RGB_MATRIX_EFFECT(RGB_CUSTOM)
RGB_MATRIX_EFFECT(RGB_CUSTOM_MULTICROSS)

// Step 2.
// Define effects inside the `RGB_MATRIX_CUSTOM_EFFECT_IMPLS` ifdef block
#ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS


static bool massdrop_is_underglow(point_t* point) {
    // Massdrop ALT defines the underglow edge LEDs as being the LEDs on the edge
    // of the matrix.
    return (point->x <= 1 || point->y <= 1 || point->x >= 222 || point->y >= 62);
}

static uint8_t custom_calc_gradient_hue(point_t* point) {
        // The x range will be 0..224, map this to 0..7
        // Relies on hue being 8-bit and wrapping
        uint8_t hue   = rgb_matrix_config.hsv.h;
        uint8_t speed = rgb_matrix_config.speed;

        if (massdrop_is_underglow(point)) {
            // Shift the underglow colour.
            hue -= 132;   // Red
            speed -= 28;  // Use fewer colours
        }
        uint8_t scale = scale8(64, speed);

        return hue + (scale * point->x >> 5);
}

static bool RGB_CUSTOM(effect_params_t* params) {
    RGB_MATRIX_USE_LIMITS(led_min, led_max);

    HSV hsv = rgb_matrix_config.hsv;
    for (uint8_t i = led_min; i < led_max; i++) {
        RGB_MATRIX_TEST_LED_FLAGS();

        hsv.h   = custom_calc_gradient_hue(&g_led_config.point[i]);
        RGB rgb = hsv_to_rgb(hsv);

        rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
    }
    return led_max < DRIVER_LED_TOTAL;
}

typedef RGB (*custom_reactive_splash_f)(RGB rgb, int16_t dx, int16_t dy, uint8_t dist, uint16_t tick);

static bool custom_effect_runner_reactive_splash(uint8_t start, effect_params_t* params, custom_reactive_splash_f effect_func) {
    RGB_MATRIX_USE_LIMITS(led_min, led_max);

    uint8_t count = g_last_hit_tracker.count;
    for (uint8_t i = led_min; i < led_max; i++) {
        RGB_MATRIX_TEST_LED_FLAGS();
        HSV hsv = rgb_matrix_config.hsv;

        // Calculate the base gradient colour.
        hsv.h   = custom_calc_gradient_hue(&g_led_config.point[i]);
        RGB rgb = hsv_to_rgb(hsv);

        for (uint8_t j = start; j < count; j++) {
            int16_t  dx   = g_led_config.point[i].x - g_last_hit_tracker.x[j];
            int16_t  dy   = g_led_config.point[i].y - g_last_hit_tracker.y[j];
            uint8_t  dist = sqrt16(dx * dx + dy * dy);
            uint16_t tick = scale16by8(g_last_hit_tracker.tick[j], rgb_matrix_config.speed);
            rgb           = effect_func(rgb, dx, dy, dist, tick);
        }

        rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
    }
    return led_max < DRIVER_LED_TOTAL;
}

static uint8_t blend_rgb_channel(uint8_t fg_v, uint8_t bg_v, float_t pos) {
    // return fg_v + (bg_v - fg_v) * pos;
    float_t fg_f = fg_v / 255.0;
    float_t bg_f = bg_v / 255.0;

    return (uint8_t)(sqrt((1-pos) * pow(fg_f, 2) + pos * pow(bg_f, 2)) * 255);
}

static RGB CUSTOM_SOLID_REACTIVE_CROSS_math(RGB bg, int16_t dx, int16_t dy, uint8_t dist, uint16_t tick) {
    uint16_t effect = tick - dist;
    if (effect > 255) effect = 255;
    if (dist > 72) effect = 255;
    if ((dx > 8 || dx < -8) && (dy > 8 || dy < -8)) effect = 255;
    
    // effect = 0 fully highlighted, 255 fully faded
    if (effect >= 255) return bg;
    
    // if (rand()/RAND_MAX < 0.001) dprintf("effect: %u\n", effect);

    // Blend orange over the gradient colour
    float_t blend_pos = pow(effect / 255.0, 2);
    RGB fg = {100, 255, 0};

    bg.r = blend_rgb_channel(fg.r, bg.r, blend_pos);
    bg.g = blend_rgb_channel(fg.g, bg.g, blend_pos);
    bg.b = blend_rgb_channel(fg.b, bg.b, blend_pos);
    return bg;
}

static bool RGB_CUSTOM_MULTICROSS(effect_params_t* params) { return custom_effect_runner_reactive_splash(0, params, &CUSTOM_SOLID_REACTIVE_CROSS_math); }

#endif  // RGB_MATRIX_CUSTOM_EFFECT_IMPLS