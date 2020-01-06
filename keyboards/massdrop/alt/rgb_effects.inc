/* A custom suite of effects.

   Author: Rob Haswell <rob@haswell.co.uk> */

// A smooth gradient from one RGB value to another

static uint8_t blend(uint8_t start, uint8_t end, uint8_t numer, uint8_t denom) { return start + (end - start) * numer / denom; }

static HSV custom_rgb_to_hsv(RGB rgb) {
    HSV     hsv;
    uint8_t rgbMin, rgbMax;

    rgbMin = rgb.r < rgb.g ? (rgb.r < rgb.b ? rgb.r : rgb.b) : (rgb.g < rgb.b ? rgb.g : rgb.b);
    rgbMax = rgb.r > rgb.g ? (rgb.r > rgb.b ? rgb.r : rgb.b) : (rgb.g > rgb.b ? rgb.g : rgb.b);

    hsv.v = rgbMax;
    if (hsv.v == 0) {
        hsv.h = 0;
        hsv.s = 0;
        return hsv;
    }

    hsv.s = 255 * (rgbMax - rgbMin) / hsv.v;
    if (hsv.s == 0) {
        hsv.h = 0;
        return hsv;
    }

    if (rgbMax == rgb.r)
        hsv.h = 0 + 43 * (rgb.g - rgb.b) / (rgbMax - rgbMin);
    else if (rgbMax == rgb.g)
        hsv.h = 85 + 43 * (rgb.b - rgb.r) / (rgbMax - rgbMin);
    else
        hsv.h = 171 + 43 * (rgb.r - rgb.g) / (rgbMax - rgbMin);

    return hsv;
}

static RGB calc_rgb_gradient_left_right(uint8_t i, RGB kb_start_rgb, RGB kb_end_rgb, RGB ug_start_rgb, RGB ug_end_rgb) {
    uint8_t dx = g_led_config.point[i].x;

    // Calculate the channel values
    if (HAS_FLAGS(g_led_config.flags[i], LED_FLAG_UNDERGLOW)) {
        return (RGB){
            blend(ug_start_rgb.g, ug_end_rgb.g, dx, 224),
            blend(ug_start_rgb.r, ug_end_rgb.r, dx, 224),
            blend(ug_start_rgb.b, ug_end_rgb.b, dx, 224),
        };
    } else {
        return (RGB){
            blend(kb_start_rgb.g, kb_end_rgb.g, dx, 224),
            blend(kb_start_rgb.r, kb_end_rgb.r, dx, 224),
            blend(kb_start_rgb.b, kb_end_rgb.b, dx, 224),
        };
    }
}

static HSV calc_hsv_gradient_left_right(uint8_t i, HSV kb_start_hsv, HSV kb_end_hsv, HSV ug_start_hsv, HSV ug_end_hsv) {
    uint8_t dx = g_led_config.point[i].x;

    // Calculate the channel values
    HSV hsv;
    if (HAS_FLAGS(g_led_config.flags[i], LED_FLAG_UNDERGLOW)) {
        return (HSV){
            blend(ug_start_hsv.h, ug_end_hsv.h, dx, 224),
            blend(ug_start_hsv.s, ug_end_hsv.s, dx, 224),
            blend(ug_start_hsv.v, ug_end_hsv.v, dx, 224),
        };
    } else {
        return (HSV){
            blend(kb_start_hsv.h, kb_end_hsv.h, dx, 224),
            blend(kb_start_hsv.s, kb_end_hsv.s, dx, 224),
            blend(kb_start_hsv.v, kb_end_hsv.v, dx, 224),
        };
    }
    return hsv;
}

static RGB  calc_rgb_gradient_left_right_via_hsv(uint8_t i, RGB kb_start_rgb, RGB kb_end_rgb, RGB ug_start_rgb, RGB ug_end_rgb) {
    HSV kb_start_hsv = custom_rgb_to_hsv(kb_start_rgb);
    HSV kb_end_hsv = custom_rgb_to_hsv(kb_end_rgb);
    HSV ug_start_hsv = custom_rgb_to_hsv(ug_start_rgb);
    HSV ug_end_hsv = custom_rgb_to_hsv(ug_end_rgb);

    return hsv_to_rgb(calc_hsv_gradient_left_right(i, kb_start_hsv, kb_end_hsv, ug_start_hsv, ug_end_hsv));
}

static bool rgb_gradient_left_right(effect_params_t* params, uint8_t kb_start[], uint8_t kb_end[], uint8_t ug_start[], uint8_t ug_end[]) {
    RGB_MATRIX_USE_LIMITS(led_min, led_max)

    const RGB kb_start_rgb = (RGB){kb_start[1], kb_start[0], kb_start[2]};
    const RGB kb_end_rgb   = (RGB){kb_end[1], kb_end[0], kb_end[2]};
    const RGB ug_start_rgb = (RGB){ug_start[1], ug_start[0], ug_start[2]};
    const RGB ug_end_rgb   = (RGB){ug_end[1], ug_end[0], ug_end[2]};

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
    const HSV kb_end_hsv   = (HSV){kb_end[0], kb_end[1], kb_end[2]};
    const HSV ug_start_hsv = (HSV){ug_start[0], ug_start[1], ug_start[2]};
    const HSV ug_end_hsv   = (HSV){ug_end[0], ug_end[1], ug_end[2]};

    for (uint8_t i = led_min; i < led_max; i++) {
        RGB_MATRIX_TEST_LED_FLAGS();

        HSV hsv = calc_hsv_gradient_left_right(i, kb_start_hsv, kb_end_hsv, ug_start_hsv, ug_end_hsv);

        // Scale the output brightness
        hsv.v   = scale8(hsv.v, rgb_matrix_config.hsv.v);
        RGB rgb = hsv_to_rgb(hsv);
        rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
    }
    return led_max < DRIVER_LED_TOTAL;
}

typedef RGB (*custom_reactive_splash_f)(RGB rgb, int16_t dx, int16_t dy, uint8_t dist, uint16_t tick, RGB cross_rgb);

typedef RGB (*bg_f)(uint8_t i, RGB kb_start_rgb, RGB kb_end_rgb, RGB ug_start_rgb, RGB ug_end_rgb);

static bool custom_effect_runner_reactive_splash(uint8_t start, effect_params_t* params, custom_reactive_splash_f effect_func, bg_f bg_func, RGB kb_start_rgb, RGB kb_end_rgb, RGB ug_start_rgb, RGB ug_end_rgb, RGB cross_rgb, uint8_t speed) {
    RGB_MATRIX_USE_LIMITS(led_min, led_max);

    uint8_t count = g_last_hit_tracker.count;
    for (uint8_t i = led_min; i < led_max; i++) {
        RGB_MATRIX_TEST_LED_FLAGS();

        RGB rgb = calc_rgb_gradient_left_right(i, kb_start_rgb, kb_end_rgb, ug_start_rgb, ug_end_rgb);

        for (uint8_t j = start; j < count; j++) {
            int16_t  dx   = g_led_config.point[i].x - g_last_hit_tracker.x[j];
            int16_t  dy   = g_led_config.point[i].y - g_last_hit_tracker.y[j];
            uint8_t  dist = sqrt16(dx * dx + dy * dy);
            uint16_t tick = scale16by8(g_last_hit_tracker.tick[j], speed);
            rgb           = effect_func(rgb, dx, dy, dist, tick, cross_rgb);
        }
        // Scale the output brightness
        rgb.r = scale8(rgb.r, rgb_matrix_config.hsv.v);
        rgb.g = scale8(rgb.g, rgb_matrix_config.hsv.v);
        rgb.b = scale8(rgb.b, rgb_matrix_config.hsv.v);

        rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
    }
    return led_max < DRIVER_LED_TOTAL;
}

static RGB calc_custom_reactive_cross(RGB rgb, int16_t dx, int16_t dy, uint8_t dist, uint16_t tick, RGB cross_rgb) {
    uint16_t effect = tick + dist;
    dx              = dx < 0 ? dx * -1 : dx;
    dy              = dy < 0 ? dy * -1 : dy;
    dx              = dx * 16 > 255 ? 255 : dx * 16;
    dy              = dy * 16 > 255 ? 255 : dy * 16;
    effect += dx > dy ? dy : dx;
    if (effect >= 255) return rgb;

    rgb.r = blend8(cross_rgb.r, rgb.r, effect);
    rgb.g = blend8(cross_rgb.g, rgb.g, effect);
    rgb.b = blend8(cross_rgb.b, rgb.b, effect);
    return rgb;
}

static bool rgb_gradient_left_right_cross(effect_params_t* params, uint8_t kb_start[], uint8_t kb_end[], uint8_t ug_start[], uint8_t ug_end[], uint8_t cross[], uint8_t speed) {
    RGB_MATRIX_USE_LIMITS(led_min, led_max)

    const RGB kb_start_rgb = (RGB){kb_start[1], kb_start[0], kb_start[2]};
    const RGB kb_end_rgb   = (RGB){kb_end[1], kb_end[0], kb_end[2]};
    const RGB ug_start_rgb = (RGB){ug_start[1], ug_start[0], ug_start[2]};
    const RGB ug_end_rgb   = (RGB){ug_end[1], ug_end[0], ug_end[2]};
    const RGB cross_rgb    = (RGB){cross[1], cross[0], cross[2]};

    return custom_effect_runner_reactive_splash(0, params, &calc_custom_reactive_cross, &calc_rgb_gradient_left_right, kb_start_rgb, kb_end_rgb, ug_start_rgb, ug_end_rgb, cross_rgb, speed);
}

static RGB calc_custom_reactive_nexus(RGB rgb, int16_t dx, int16_t dy, uint8_t dist, uint16_t tick, RGB cross_rgb) {
    uint16_t effect = tick - dist;
    if (effect > 255) effect = 255;
    if (dist > 72) effect = 255;
    if ((dx > 8 || dx < -8) && (dy > 8 || dy < -8)) effect = 255;
    if (effect >= 255) return rgb;

    rgb.r = blend8(cross_rgb.r, rgb.r, effect);
    rgb.g = blend8(cross_rgb.g, rgb.g, effect);
    rgb.b = blend8(cross_rgb.b, rgb.b, effect);
    return rgb;
}

static bool rgb_gradient_left_right_nexus(effect_params_t* params, uint8_t kb_start[], uint8_t kb_end[], uint8_t ug_start[], uint8_t ug_end[], uint8_t cross[], uint8_t speed) {
    RGB_MATRIX_USE_LIMITS(led_min, led_max)

    const RGB kb_start_rgb = (RGB){kb_start[1], kb_start[0], kb_start[2]};
    const RGB kb_end_rgb   = (RGB){kb_end[1], kb_end[0], kb_end[2]};
    const RGB ug_start_rgb = (RGB){ug_start[1], ug_start[0], ug_start[2]};
    const RGB ug_end_rgb   = (RGB){ug_end[1], ug_end[0], ug_end[2]};
    const RGB cross_rgb    = (RGB){cross[1], cross[0], cross[2]};

    return custom_effect_runner_reactive_splash(0, params, &calc_custom_reactive_nexus, &calc_rgb_gradient_left_right, kb_start_rgb, kb_end_rgb, ug_start_rgb, ug_end_rgb, cross_rgb, speed);
}

typedef RGB (*custom_reactive_f)(RGB rgb, uint16_t offset, RGB key_rgb);

static bool custom_effect_runner_reactive(effect_params_t* params, custom_reactive_f effect_func, bg_f bg_func, RGB kb_start_rgb, RGB kb_end_rgb, RGB ug_start_rgb, RGB ug_end_rgb, RGB key_rgb, uint8_t speed) {
    RGB_MATRIX_USE_LIMITS(led_min, led_max);

    uint16_t max_tick = 65535 / speed;
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

        uint16_t offset = scale16by8(tick, speed);
        RGB      rgb    = calc_rgb_gradient_left_right(i, kb_start_rgb, kb_end_rgb, ug_start_rgb, ug_end_rgb);
        rgb             = effect_func(rgb, offset, key_rgb);

        // Scale the output brightness
        rgb.r = scale8(rgb.r, rgb_matrix_config.hsv.v);
        rgb.g = scale8(rgb.g, rgb_matrix_config.hsv.v);
        rgb.b = scale8(rgb.b, rgb_matrix_config.hsv.v);

        rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
    }
    return led_max < DRIVER_LED_TOTAL;
}

static RGB calc_custom_reactive_single(RGB rgb, uint16_t offset, RGB key_rgb) {
    if (offset >= 255) return rgb;
    uint8_t effect = offset > 255 ? 255 : offset;

    rgb.r = blend8(key_rgb.r, rgb.r, effect);
    rgb.g = blend8(key_rgb.g, rgb.g, effect);
    rgb.b = blend8(key_rgb.b, rgb.b, effect);
    return rgb;
}

static bool rgb_gradient_left_right_single(effect_params_t* params, uint8_t kb_start[], uint8_t kb_end[], uint8_t ug_start[], uint8_t ug_end[], uint8_t key[], uint8_t speed) {
    RGB_MATRIX_USE_LIMITS(led_min, led_max)

    const RGB kb_start_rgb = (RGB){kb_start[1], kb_start[0], kb_start[2]};
    const RGB kb_end_rgb   = (RGB){kb_end[1], kb_end[0], kb_end[2]};
    const RGB ug_start_rgb = (RGB){ug_start[1], ug_start[0], ug_start[2]};
    const RGB ug_end_rgb   = (RGB){ug_end[1], ug_end[0], ug_end[2]};
    const RGB key_rgb      = (RGB){key[1], key[0], key[2]};

    return custom_effect_runner_reactive(params, &calc_custom_reactive_single, &calc_rgb_gradient_left_right, kb_start_rgb, kb_end_rgb, ug_start_rgb, ug_end_rgb, key_rgb, speed);
}

static bool rgb_gradient_left_right_single_via_hsv(effect_params_t* params, uint8_t kb_start[], uint8_t kb_end[], uint8_t ug_start[], uint8_t ug_end[], uint8_t key[], uint8_t speed) {
    RGB_MATRIX_USE_LIMITS(led_min, led_max)

    const RGB kb_start_rgb = (RGB){kb_start[1], kb_start[0], kb_start[2]};
    const RGB kb_end_rgb   = (RGB){kb_end[1], kb_end[0], kb_end[2]};
    const RGB ug_start_rgb = (RGB){ug_start[1], ug_start[0], ug_start[2]};
    const RGB ug_end_rgb   = (RGB){ug_end[1], ug_end[0], ug_end[2]};
    const RGB key_rgb      = (RGB){key[1], key[0], key[2]};

    return custom_effect_runner_reactive(params, &calc_custom_reactive_single, &calc_rgb_gradient_left_right_via_hsv, kb_start_rgb, kb_end_rgb, ug_start_rgb, ug_end_rgb, key_rgb, speed);
}