/* A custom suite of effects.

   Author: Rob Haswell <rob@haswell.co.uk> */

RGB_MATRIX_EFFECT(RGB_LEFT_RIGHT)
RGB_MATRIX_EFFECT(HSV_LEFT_RIGHT)
RGB_MATRIX_EFFECT(RGB_LEFT_RIGHT_CROSS)
RGB_MATRIX_EFFECT(RGB_LEFT_RIGHT_NEXUS)
RGB_MATRIX_EFFECT(RGB_LEFT_RIGHT_SINGLE)

#ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS
#include "rgb_effects.inc"

static bool RGB_LEFT_RIGHT(effect_params_t* params) {
    /*
    A left-right gradient with a separate color range for the underglow.

    Colors can be specified by constants (https://beta.docs.qmk.fm/features/feature_rgblight#colors) or manually:

    uint8_t KB_RIGHT[] = {RGB_MAGENTA};
    - or -
    uint8_t KB_RIGHT[] = {255, 0, 555};
    */
    uint8_t KB_LEFT[] = {RGB_GREEN};
    uint8_t KB_RIGHT[] = {RGB_MAGENTA};
    uint8_t UG_LEFT[] = {RGB_BLUE};
    uint8_t UG_RIGHT[] = {RGB_CYAN};
    
    return rgb_gradient_left_right(params, KB_LEFT, KB_RIGHT, UG_LEFT, UG_RIGHT);
}

static bool HSV_LEFT_RIGHT(effect_params_t* params) {
   /*
    A left-right gradient with a separate color range for the underglow.

    Colors can be specified by constants (https://beta.docs.qmk.fm/features/feature_rgblight#colors) or manually:

    uint8_t KB_RIGHT[] = {HSV_MAGENTA};
    - or -
    uint8_t KB_RIGHT[] = {213, 255, 255};
    */
    uint8_t KB_LEFT[] = {HSV_GREEN};
    uint8_t KB_RIGHT[] = {HSV_MAGENTA};
    uint8_t UG_LEFT[] = {HSV_BLUE};
    uint8_t UG_RIGHT[] = {HSV_CYAN};

    return hsv_gradient_left_right(params, KB_LEFT, KB_RIGHT, UG_LEFT, UG_RIGHT);
}

static bool RGB_LEFT_RIGHT_CROSS(effect_params_t* params) {
    /*
    A left-right gradient with a separate color range for the underglow, with a "cross"-type effect.

    Colors can be specified by constants (https://beta.docs.qmk.fm/features/feature_rgblight#colors) or manually:

    uint8_t KB_RIGHT[] = {RGB_MAGENTA};
    - or -
    uint8_t KB_RIGHT[] = {255, 0, 555};
    */
    uint8_t KB_LEFT[] = {RGB_GREEN};
    uint8_t KB_RIGHT[] = {RGB_MAGENTA};
    uint8_t UG_LEFT[] = {RGB_BLUE};
    uint8_t UG_RIGHT[] = {RGB_CYAN};

    uint8_t CROSS[] = {RGB_TEAL}; 
    uint8_t SPEED = 40; // 0-255
    
    return rgb_gradient_left_right_cross(params, KB_LEFT, KB_RIGHT, UG_LEFT, UG_RIGHT, CROSS, SPEED);
}

static bool RGB_LEFT_RIGHT_NEXUS(effect_params_t* params) {
    /*
    A left-right gradient with a separate color range for the underglow, with a "nexus"-type effect.

    Colors can be specified by constants (https://beta.docs.qmk.fm/features/feature_rgblight#colors) or manually:

    uint8_t KB_RIGHT[] = {RGB_MAGENTA};
    - or -
    uint8_t KB_RIGHT[] = {255, 0, 555};
    */
    uint8_t KB_LEFT[] = {RGB_GREEN};
    uint8_t KB_RIGHT[] = {RGB_MAGENTA};
    uint8_t UG_LEFT[] = {RGB_BLUE};
    uint8_t UG_RIGHT[] = {RGB_CYAN};

    uint8_t CROSS[] = {RGB_TEAL}; 
    uint8_t SPEED = 40; // 0-255
    
    return rgb_gradient_left_right_nexus(params, KB_LEFT, KB_RIGHT, UG_LEFT, UG_RIGHT, CROSS, SPEED);
}

static bool RGB_LEFT_RIGHT_SINGLE(effect_params_t* params) {
    /*
    A left-right gradient with a separate color range for the underglow, with a single key highlight effect.

    Colors can be specified by constants (https://beta.docs.qmk.fm/features/feature_rgblight#colors) or manually:

    uint8_t KB_RIGHT[] = {RGB_MAGENTA};
    - or -
    uint8_t KB_RIGHT[] = {255, 0, 555};
    */
    uint8_t KB_LEFT[] = {RGB_GREEN};
    uint8_t KB_RIGHT[] = {RGB_MAGENTA};
    uint8_t UG_LEFT[] = {RGB_BLUE};
    uint8_t UG_RIGHT[] = {RGB_CYAN};

    uint8_t KEY[] = {RGB_TEAL}; 
    uint8_t SPEED = 40; // 0-255
    
    return rgb_gradient_left_right_single(params, KB_LEFT, KB_RIGHT, UG_LEFT, UG_RIGHT, KEY, SPEED);
}
#endif
