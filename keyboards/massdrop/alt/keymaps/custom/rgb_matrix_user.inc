// !!! DO NOT ADD #pragma once !!! //

// Step 1.
// Declare custom effects using the RGB_MATRIX_EFFECT macro
// (note the lack of semicolon after the macro!)
RGB_MATRIX_EFFECT(RGB_CUSTOM)

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

#endif  // RGB_MATRIX_CUSTOM_EFFECT_IMPLS