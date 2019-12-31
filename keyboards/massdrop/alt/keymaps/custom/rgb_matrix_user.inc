// !!! DO NOT ADD #pragma once !!! //

// Step 1.
// Declare custom effects using the RGB_MATRIX_EFFECT macro
// (note the lack of semicolon after the macro!)
RGB_MATRIX_EFFECT(CUSTOM_RGB_MULTICROSS)

// Step 2.
// Define effects inside the `RGB_MATRIX_CUSTOM_EFFECT_IMPLS` ifdef block
#ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

// static bool massdrop_is_underglow(point_t* point) {
//     // Massdrop ALT defines the underglow edge LEDs as being the LEDs on the edge
//     // of the matrix.
//     return (point->x <= 1 || point->y <= 1 || point->x >= 222 || point->y >= 62);
// }

// static uint8_t custom_calc_gradient_hue(point_t* point) {
//         // The x range will be 0..224, map this to 0..7
//         // Relies on hue being 8-bit and wrapping
//         uint8_t hue   = rgb_matrix_config.hsv.h;
//         uint8_t speed = rgb_matrix_config.speed;

//         if (massdrop_is_underglow(point)) {
//             // Shift the underglow colour.
//             hue -= 132;   // Red
//             speed -= 28;  // Use fewer colours
//         }
//         uint8_t scale = scale8(64, speed);

//         return hue + (scale * point->x >> 5);
// }

// static bool RGB_CUSTOM(effect_params_t* params) {
//     RGB_MATRIX_USE_LIMITS(led_min, led_max);

//     HSV hsv = rgb_matrix_config.hsv;
//     for (uint8_t i = led_min; i < led_max; i++) {
//         RGB_MATRIX_TEST_LED_FLAGS();

//         hsv.h   = custom_calc_gradient_hue(&g_led_config.point[i]);
//         RGB rgb = hsv_to_rgb(hsv);

//         rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
//     }
//     return led_max < DRIVER_LED_TOTAL;
// }

typedef HSV (*reactive_splash_f)(HSV hsv, int16_t dx, int16_t dy, uint8_t dist, uint16_t tick);

bool custom_effect_runner_reactive_splash(uint8_t start, effect_params_t* params, reactive_splash_f effect_func) {
    RGB_MATRIX_USE_LIMITS(led_min, led_max);

    uint8_t count = g_last_hit_tracker.count;
    for (uint8_t i = led_min; i < led_max; i++) {
        RGB_MATRIX_TEST_LED_FLAGS();
        HSV hsv = rgb_matrix_config.hsv;
        hsv.v   = 0;
        for (uint8_t j = start; j < count; j++) {
            int16_t  dx   = g_led_config.point[i].x - g_last_hit_tracker.x[j];
            int16_t  dy   = g_led_config.point[i].y - g_last_hit_tracker.y[j];
            uint8_t  dist = sqrt16(dx * dx + dy * dy);
            uint16_t tick = scale16by8(g_last_hit_tracker.tick[j], rgb_matrix_config.speed);
            hsv           = effect_func(hsv, dx, dy, dist, tick);
        }
        hsv.v   = scale8(hsv.v, rgb_matrix_config.hsv.v);
        RGB rgb = hsv_to_rgb(hsv);
        rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
    }
    return led_max < DRIVER_LED_TOTAL;
}

static HSV CUSTOM_SOLID_REACTIVE_CROSS_math(HSV hsv, int16_t dx, int16_t dy, uint8_t dist, uint16_t tick) {
    uint16_t effect = tick + dist;
    dx              = dx < 0 ? dx * -1 : dx;
    dy              = dy < 0 ? dy * -1 : dy;
    dx              = dx * 16 > 255 ? 255 : dx * 16;
    dy              = dy * 16 > 255 ? 255 : dy * 16;
    effect += dx > dy ? dy : dx;
    if (effect > 255) effect = 255;
    hsv.v = qadd8(hsv.v, 255 - effect);
    return hsv;
}

bool CUSTOM_RGB_MULTICROSS(effect_params_t* params) { return custom_effect_runner_reactive_splash(0, params, &CUSTOM_SOLID_REACTIVE_CROSS_math); }

#endif  // RGB_MATRIX_CUSTOM_EFFECT_IMPLS