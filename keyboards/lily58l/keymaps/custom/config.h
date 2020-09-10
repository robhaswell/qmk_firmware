/*
This is the c configuration file for the keymap

Copyright 2012 Jun Wako <wakojun@gmail.com>
Copyright 2015 Jack Humbert

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

//#define USE_MATRIX_I2C

/* Select hand configuration */

#define MASTER_LEFT
// #define MASTER_RIGHT
// #define EE_HANDS

#define USE_SERIAL_PD2

#define TAPPING_FORCE_HOLD
#define TAPPING_TERM 100

#define RGBLIGHT_HUE_STEP 2
#define RGBLIGHT_SAT_STEP 25
#define RGBLIGHT_VAL_STEP 25

// Define which RGB lighting effects we want access to so we can reduce the footprint
// #define RGBLIGHT_ANIMATIONS
#define RGBLIGHT_EFFECT_STATIC_GRADIENT
#define RGBLIGHT_EFFECT_RAINBOW_SWIRL

// RGBLIGHT options
#define RGBLIGHT_SLEEP
#undef RGBLIGHT_LIMIT_VAL
#define RGBLIGHT_LIMIT_VAL UINT8_MAX

// Encoder
#ifdef ENCODER_ENABLE
    // Enable if encoder is on the left side
    #define ENCODER_DIRECTION_FLIP
#endif

// Enable some space-saving measures for QMK
#define NO_ACTION_MACRO
#define NO_ACTION_FUNCTION
#define NO_ACTION_TAPPING
#define NO_ACTION_ONESHOT

// See the performance of the keyboard
// #define DEBUG_MATRIX_SCAN_RATE
