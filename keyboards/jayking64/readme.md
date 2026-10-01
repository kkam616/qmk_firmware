# JAYKING64

60% ANSI keyboard (7U space, Apple-style bottom row) with ALPS switches and 12 WS2812B underglow LEDs.

* Keyboard Maintainer: jay
* Hardware Supported: JAYKING64 Rev1 PCB (STM32F072CBT6)

Make example for this keyboard (after setting up your build environment):

    make jayking64:default

Flashing example for this keyboard:

    make jayking64:default:flash

See the [build environment setup](https://docs.qmk.fm/#/getting_started_build_tools) and the [make instructions](https://docs.qmk.fm/#/getting_started_make_guide) for more information. Brand new to QMK? Start with our [Complete Newbs Guide](https://docs.qmk.fm/#/newbs).

## Bootloader

Enter the bootloader in 3 ways:

* **Bootmagic reset**: Hold down the top-left key (Esc / `) and plug in the keyboard
* **Physical reset button**: Press SW1 (BOOT) on the PCB
* **Keycode in layout**: Fn + `\` (`QK_BOOT`)
