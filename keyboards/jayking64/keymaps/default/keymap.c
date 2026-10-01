// Copyright 2026 jay
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

enum layers {
    _BASE,
    _FN,
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /* Base: AEK64 배열 (6.5U 스페이스, Windows 기준)
     * ┌───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───────┐
     * │Esc│ 1 │ 2 │ 3 │ 4 │ 5 │ 6 │ 7 │ 8 │ 9 │ 0 │ - │ = │Delete │
     * ├───┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─────┤
     * │ Tab │ Q │ W │ E │ R │ T │ Y │ U │ I │ O │ P │ [ │ ] │  \  │
     * ├─────┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴─────┤
     * │ Caps │ A │ S │ D │ F │ G │ H │ J │ K │ L │ ; │ ' │ Return │
     * ├──────┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴────────┤
     * │ Shift  │ Z │ X │ C │ V │ B │ N │ M │ , │ . │ / │  Shift   │
     * ├─────┬──┴─┬─┴───┼───┴───┴───┴───┴───┴─┬─┴───┼───┴┬─────────┤
     * │Ctrl │Win │ Alt │        Space        │ H/E │ Fn │ Hanja   │
     * └─────┴────┴─────┴─────────────────────┴─────┴────┴─────────┘
     * - 왼쪽 위 키(QK_GESC): 그냥 누르면 Esc, Shift/Win과 같이 누르면 ` (~)
     * - Delete(KC_BSPC): Apple 키보드처럼 앞 글자 지우기
     * - 왼쪽은 Windows 표준 순서(Ctrl, Win, Alt): Option 키캡 = Win, Command 키캡 = Alt
     * - 오른쪽 Command 자리 = 한/영 (KC_LNG1)
     * - 오른쪽 Option 자리 = Fn (누르고 있는 동안 _FN 레이어)
     * - 오른쪽 Control 자리 = 한자 (KC_LNG2)
     */
    [_BASE] = LAYOUT_aek64(
        QK_GESC, KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSPC,
        KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS,
        KC_CAPS, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,          KC_ENT,
        KC_LSFT,          KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,          KC_RSFT,
        KC_LCTL, KC_LGUI, KC_LALT,                            KC_SPC,                             KC_LNG1, MO(_FN), KC_LNG2
    ),

    /* Fn 레이어 (오른쪽 Option 자리의 Fn을 누른 채로)
     * - Esc 키: `, 숫자줄: F1~F12, Delete 자리 = 앞으로 지우기(Del)
     * - 왼손 내비게이션 (Fn은 오른손이라 손이 꼬이지 않게 왼쪽에 모음)
     *     Q Home   W ↑    E End    R PgUp
     *     A ←      S ↓    D →      F PgDn
     * - P: Print Screen
     * - M / , / . / /: 음소거 / 볼륨 Down / 볼륨 Up / 재생·일시정지
     * - Z~N: 언더글로 LED (켜기/끄기, 모드, 색상, 채도, 밝기+/-)
     * - \: 부트로더(DFU) 진입(QK_BOOT)  ← 소프트웨어 리셋을 거치므로 NRST(C9) 회로 테스트도 이걸로 함
     */
    [_FN] = LAYOUT_aek64(
        KC_GRV,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  KC_DEL,
        _______, KC_HOME, KC_UP,   KC_END,  KC_PGUP, _______, _______, _______, _______, _______, KC_PSCR, _______, _______, QK_BOOT,
        _______, KC_LEFT, KC_DOWN, KC_RGHT, KC_PGDN, _______, _______, _______, _______, _______, _______, _______,          _______,
        _______,          UG_TOGG, UG_NEXT, UG_HUEU, UG_SATU, UG_VALU, UG_VALD, KC_MUTE, KC_VOLD, KC_VOLU, KC_MPLY,          _______,
        _______, _______, _______,                            _______,                            _______, _______, _______
    ),
};
