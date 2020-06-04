# Quantum Mechanical Keyboard Firmware

[![Current Version](https://img.shields.io/github/tag/qmk/qmk_firmware.svg)](https://github.com/qmk/qmk_firmware/tags)
[![Build Status](https://travis-ci.org/qmk/qmk_firmware.svg?branch=master)](https://travis-ci.org/qmk/qmk_firmware)
[![Discord](https://img.shields.io/discord/440868230475677696.svg)](https://discord.gg/Uq7gcHh)
[![Docs Status](https://img.shields.io/badge/docs-ready-orange.svg)](https://docs.qmk.fm)
[![GitHub contributors](https://img.shields.io/github/contributors/qmk/qmk_firmware.svg)](https://github.com/qmk/qmk_firmware/pulse/monthly)
[![GitHub forks](https://img.shields.io/github/forks/qmk/qmk_firmware.svg?style=social&label=Fork)](https://github.com/qmk/qmk_firmware/)

This is a keyboard firmware based on the [tmk\_keyboard firmware](https://github.com/tmk/tmk_keyboard) with some useful features for Atmel AVR and ARM controllers, and more specifically, the [OLKB product line](https://olkb.com), the [ErgoDox EZ](https://ergodox-ez.com) keyboard, and the [Clueboard product line](https://clueboard.co).

## Rob Haswell's fork

This fork of the QMK firmware has some unique lighting and functionality:

### Secondary colour configuration

This fork defines some custom implementations of RGB effects, all of which are variations of a left-right gradient:

* `RGB_CUSTOM`: A left-right gradient. Alternative colour controls underglow. Colour "speed" controls the gradient width.
* `RGB_CUSTOM_MULTICROSS`: A left-right gradient with a "cross" reactive effect, like RGB_CUSTOM. Alternative saturation controls reactive colour.
* `RGB_CUSTOM_SOLID_REACTIVE_SIMPLE`: A left-right gradient with a "simple" (single-key) reactive effect, like RGB_CUSTOM. Alternative saturation controls reactive colour.
* `RGB_CUSTOM_CYCLE`: A left-right gradient which cycles very slowly. Like RGB_CUSTOM.

The term "alternative colour" refers to a completely separate set of colour configuration made available by this fork. QMK provides keycodes to control the HUE, SAT, VAL and SPD (HSV & speed) of the RGB system. This fork provides an identical additional HSV & speed which are altered by holding SHIFT when using the normal HUE, SAT, VAL & SPD keycodes. This is the alternative colour configuration.

### Colour profiles

The user can define a list of profiles. These can be cycled through with FN+x. The profiles are simply pre-selected values for the HSV & speed + alternative HSV & speed (see above). They are defined in [keymap.c](massdrop/alt/keymaps/custom/keymap.c):

```c
rgb_profile_t rgb_profiles[] = {
    [0] = {{ .enable=1, .mode=39, .hsv={ 0, 255, 255 }, .speed=20 },
           { .enable=0, .mode=0, .hsv={ 136, 140, 255 }, .speed=24 }},
    [1] = {{ .enable=1, .mode=38, .hsv={ 136, 255, 255 }, .speed=48 },
           { .enable=0, .mode=0, .hsv={ 120, 16, 255 }, .speed=222 }},
    ...
};
```

To make it more convenient to define these the user can enable 'Debug mode' with the `DBG_TOG` keycode (FN+m). On keypress this will print a copiable representation of the RGB configuration to the debug console:

```c
    [x] = {{ .enable=1, .mode=39, .hsv={ 0, 255, 255 }, .speed=20 },
           { .enable=0, .mode=0, .hsv={ 136, 140, 255 }, .speed=24 }},
```

Above the profile coniguration the user can also set the initial profile.

### Initial colour configuration

The Massdrop ALT does not support writing to the EEPROM, so in order for the ALT to start with the correct colour configuration, some additional constants can be defined in [config.h](keyboards/massdrop/alt/config.h). These are overridden by profiles.

* `RGB_MATRIX_STARTUP_ALT_HUE`
* `RGB_MATRIX_STARTUP_ALT_SAT`
* `RGB_MATRIX_STARTUP_ALT_VAL`
* `RGB_MATRIX_STARTUP_ALT_SPD`

### Additional RGB configuration

Some under-the-hood tweaks can be made to the RGB config which cannot be chanegd at runtime. These are defined in [config.h](keyboards/massdrop/alt/config.h):

* `RGB_MATRIX_BLEND_RGB`: Blend overlaid colours together using the RGB colour space instead of HSV.
* `RGB_MATRIX_BLEND_SQRT`: Enable a more precise calculation of blended colours. This decreases scan rate.
* `RGB_MATRIX_REACTIVE_CROSS_SPEED`: The speed of the cross effect.
* `RGB_MATRIX_REACTIVE_SIMPLE_SPEED`: The speed of the single-key effect.
* `RGB_MATRIX_REACTIVE_SIMPLE_THRESHOLD`: Introduce a delay before single-key reactive effects begin to fade. This allows the effect to be seen for longer after the user releases the key.

### Idle brightness

This fork includes a custom idle timer, due to the shipped idle timer being nonfunctional at the time. It also allows idle dimming instead of fully powering down. These are defined in [config.h](keyboards/massdrop/alt/config.h):

* `IDLE_TIMEOUT`: Timeout time in seconds
* `IDLE_BRIGHTNESS`: Brightness value to set (0-255)

### Demo mode

The keymap defines a demo mode, where RGB effects can be demonstrated (and changed) but no keys will be sent to the OS. It is activated by holding down both FN keys (bottom left and bottom right, as defined by the keymap).

### Mac mode (or "work mode")

A keymap which emulates a Mac keyboard, namely swapping the LGUI and LALT keys, and swapping the location of `"` and `@`.

This mode also disables RGB as it is used as a "work mode".

### CAPS -> CTRL

As a user preference, the Caps Lock key has been mapped to CTRL. In its stead the CTRL key has become FN. As another convenience the RIGHT_CTRL key has become another FN key.

Caps Lock is accessed by holding FN and double-tapping the usual Caps Lock key.

### Mac-like # key

The author of this fork is primarily a Mac user. UK Macs provide `£` on SHIFT+3 and `#` on ALT+3. For familiarity this keymap provides the same mapping under Windows, for which the default keymap is designed. Pressing LGUI+3 will send `KC_BSLS` which is interpreted by Windows as `#` when using a UK layout. This functionality can be removed in [keymap.c](massdrop/alt/keymaps/custom/keymap.c).

### Faster reset

The default ALT firmware will enter bootloader mode after the reset key (FN+b) has been released after being held for 500ms. This fork slightly changes that so that the keyboard resets immediately after 500ms, instead of waiting for the key to be released.

## Documentation

* [See the official documentation on docs.qmk.fm](https://docs.qmk.fm)

The docs are powered by [Docsify](https://docsify.js.org/) and hosted on [GitHub](/docs/). They are also viewable offline; see [Previewing the Documentation](https://docs.qmk.fm/#/contributing?id=previewing-the-documentation) for more details.

You can request changes by making a fork and opening a [pull request](https://github.com/qmk/qmk_firmware/pulls), or by clicking the "Edit this page" link at the bottom of any page.

## Supported Keyboards

* [Planck](/keyboards/planck/)
* [Preonic](/keyboards/preonic/)
* [ErgoDox EZ](/keyboards/ergodox_ez/)
* [Clueboard](/keyboards/clueboard/)
* [Cluepad](/keyboards/clueboard/17/)
* [Atreus](/keyboards/atreus/)

The project also includes community support for [lots of other keyboards](/keyboards/).

## Maintainers

QMK is developed and maintained by Jack Humbert of OLKB with contributions from the community, and of course, [Hasu](https://github.com/tmk). The OLKB product firmwares are maintained by [Jack Humbert](https://github.com/jackhumbert), the Ergodox EZ by [ZSA Technology Labs](https://github.com/zsa), the Clueboard by [Zach White](https://github.com/skullydazed), and the Atreus by [Phil Hagelberg](https://github.com/technomancy).

## Official Website

[qmk.fm](https://qmk.fm) is the official website of QMK, where you can find links to this page, the documentation, and the keyboards supported by QMK.
