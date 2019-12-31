// !!! DO NOT ADD #pragma once !!! //

// Step 1.
// Declare custom effects using the RGB_MATRIX_EFFECT macro
// (note the lack of semicolon after the macro!)
RGB_MATRIX_EFFECT(RGB_CUSTOM)

// Step 2.
// Define effects inside the `RGB_MATRIX_CUSTOM_EFFECT_IMPLS` ifdef block
#ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

// e.g: A simple effect, self-contained within a single method
static bool RGB_CUSTOM(effect_params_t* params) {
    RGB_MATRIX_USE_LIMITS(led_min, led_max);

    HSV     hsv   = rgb_matrix_config.hsv;
    for (uint8_t i = led_min; i < led_max; i++) {
        RGB_MATRIX_TEST_LED_FLAGS();
        // The x range will be 0..224, map this to 0..7
        // Relies on hue being 8-bit and wrapping
        uint8_t hue = rgb_matrix_config.hsv.h;
        uint8_t speed = rgb_matrix_config.speed;

        if (
            g_led_config.point[i].x <= 1 || g_led_config.point[i].y <= 1 ||
            g_led_config.point[i].x >= 222 || g_led_config.point[i].y >= 62) {
          hue -= 132; // Red
          speed -= 28; // Use fewer colours
        }
        uint8_t scale = scale8(64, speed);

        hsv.h   = hue + (scale * g_led_config.point[i].x >> 5);
        RGB rgb = hsv_to_rgb(hsv);
        rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
    }
    return led_max < DRIVER_LED_TOTAL;
}

#endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS