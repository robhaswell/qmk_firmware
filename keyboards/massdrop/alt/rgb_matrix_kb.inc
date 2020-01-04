/* A custom suite of effects.

   Author: Rob Haswell <rob@haswell.co.uk> */

RGB_MATRIX_EFFECT(RGB_LEFT_RIGHT)
RGB_MATRIX_EFFECT(HSV_LEFT_RIGHT)

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

#endif