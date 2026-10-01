// Copyright 2026 jay
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

enum layers {
    _BASE,
    _FN,
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /* Base: Apple 스타일 60% ANSI 7U
     * ┌───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───────┐
     * │Esc│ 1 │ 2 │ 3 │ 4 │ 5 │ 6 │ 7 │ 8 │ 9 │ 0 │ - │ = │Delete │
     * ├───┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─────┤
     * │ Tab │ Q │ W │ E │ R │ T │ Y │ U │ I │ O │ P │ [ │ ] │  \  │
     * ├─────┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴─────┤
     * │ Caps │ A │ S │ D │ F │ G │ H │ J │ K │ L │ ; │ ' │ Return │
     * ├──────┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴────────┤
     * │ Shift  │ Z │ X │ C │ V │ B │ N │ M │ , │ . │ / │  Shift   │
     * ├─────┬──┴┬──┴──┬┴───┴───┴───┴───┴───┴──┬┴───┴┬──┴┬─────────┤
     * │Ctrl │Opt│ Cmd │         Space         │ Cmd │Opt│ Fn      │
     * └─────┴───┴─────┴───────────────────────┴─────┴───┴─────────┘
     * - 왼쪽 위 키(QK_GESC): 그냥 누르면 Esc, Shift/Cmd와 같이 누르면 ` (~)
     * - Delete(KC_BSPC): Apple 키보드처럼 앞 글자 지우기
     * - 오른쪽 Control 자리 = Fn (누르고 있는 동안 _FN 레이어)
     */
    [_BASE] = LAYOUT_60_ansi_7u(
        QK_GESC, KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSPC,
        KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS,
        KC_CAPS, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,          KC_ENT,
        KC_LSFT,          KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,          KC_RSFT,
        KC_LCTL, KC_LALT, KC_LGUI,                            KC_SPC,                             KC_RGUI, KC_RALT, MO(_FN)
    ),

    /* Fn 레이어 (오른쪽 Fn을 누른 채로)
     * - 숫자줄: F1~F12, Delete 자리 = 앞으로 지우기(Del)
     * - I/J/K/L: 방향키, U/O: Home/End, P/;: PgUp/PgDn
     * - Z~N: 언더글로 LED (켜기/끄기, 모드, 색상, 밝기+/-)
     * - R: 소프트 리셋(QK_RBT), \: 부트로더(DFU) 진입(QK_BOOT)  ← 실보드 리셋 회로 테스트용
     * - A/S/D: 볼륨 Down/Up/음소거
     */
    [_FN] = LAYOUT_60_ansi_7u(
        KC_GRV,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  KC_DEL,
        _______, _______, _______, _______, QK_RBT,  _______, _______, KC_HOME, KC_UP,   KC_END,  KC_PGUP, _______, _______, QK_BOOT,
        _______, KC_VOLD, KC_VOLU, KC_MUTE, _______, _______, _______, KC_LEFT, KC_DOWN, KC_RGHT, KC_PGDN, _______,          _______,
        _______,          UG_TOGG, UG_NEXT, UG_HUEU, UG_SATU, UG_VALU, UG_VALD, _______, _______, _______, _______,          _______,
        _______, _______, _______,                            _______,                            _______, _______, _______
    ),
};
