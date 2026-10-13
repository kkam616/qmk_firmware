# JAYKING64

Apple Extended Keyboard(AEK)의 키캡과 ALPS 스위치를 옮겨 심어 만드는 60% 키보드입니다.
기본 배열은 **AEK64**(6.5U 스페이스, 1.25U Option)이며, 키캡은 Apple 스타일이지만 **Windows 기준**으로 키맵을 잡았습니다.

* Keyboard Maintainer: jay
* Hardware Supported: JAYKING64 Rev1r10 PCB
* MCU: STM32F072CBT6 (크리스털 없음, USB는 HSI48 + CRS)
* 스위치: ALPS SKCM/SKCL, 5행 × 14열 매트릭스 (COL2ROW)
* LED: WS2812B-2020 언더글로 12개
* 하드웨어 자료: [`hardware/`](hardware/) (넷리스트, 회로도 PDF, BOM)

## 기본 레이어

맨 아랫줄은 키캡 글자와 실제 기능이 다릅니다.

```
키캡: │ Control │ Option │ Command │   Space   │ Command │ Option │ Control │
기능: │  Ctrl   │  Win   │   Alt   │   Space   │  한/영  │   Fn   │  한자   │
```

* 왼쪽 위 키: 그냥 누르면 **Esc**, Shift나 Win과 같이 누르면 **`** (`~`)
* Delete 키: **Backspace** (앞 글자 지우기)

## Fn 레이어

오른쪽 **Option** 키(Fn)를 누른 채로 사용합니다. Fn이 오른손이라 이동 키는 왼손 쪽에 모았습니다.

```
┌────┬────┬────┬────┬────┬────┬────┬────┬────┬────┬────┬────┬────┬────────┐
│ `  │ F1 │ F2 │ F3 │ F4 │ F5 │ F6 │ F7 │ F8 │ F9 │F10 │F11 │F12 │  Del   │
├────┴─┬──┴─┬──┴─┬──┴─┬──┴─┬──┴─┬──┴─┬──┴─┬──┴─┬──┴─┬──┴─┬──┴─┬──┴─┬──────┤
│      │Home│ ↑  │End │PgUp│    │    │    │    │    │PrtSc│   │    │Boot3s│
├──────┴┬───┴┬───┴┬───┴┬───┴┬───┴┬───┴┬───┴┬───┴┬───┴┬───┴┬───┴┬───┴──────┤
│FnLock │ ←  │ ↓  │ →  │PgDn│    │    │    │    │    │    │    │          │
├───────┴─┬──┴─┬──┴─┬──┴─┬──┴─┬──┴─┬──┴─┬──┴─┬──┴─┬──┴─┬──┴─┬──┴──────────┤
│         │LED │Mode│Hue │    │Bri+│Bri-│Mute│Vol-│Vol+│Play│             │
└─────────┴────┴────┴────┴────┴────┴────┴────┴────┴────┴────┴─────────────┘
```

| 분류 | Fn + 키 | 기능 |
|---|---|---|
| 기호 | Esc 자리 | `` ` `` (백틱) |
| F키 | `1` ~ `=` | F1 ~ F12 |
| 지우기 | Delete 키 | Del (앞으로 지우기) |
| 방향키 | `W` / `A` / `S` / `D` | ↑ / ← / ↓ / → |
| 이동 | `Q` / `E` | Home / End |
| | `R` / `F` | Page Up / Page Down |
| 캡처 | `P` | Print Screen |
| **Fn 고정** | `Caps Lock` | Fn 레이어 고정 켜기 / 끄기 (아래 설명) |
| 미디어 | `M` | 음소거 |
| | `,` / `.` | 볼륨 − / 볼륨 + |
| | `/` | 재생 / 일시정지 |
| LED | `Z` | 켜기 / 끄기 |
| | `X` | 효과 모드 바꾸기 |
| | `C` | 색상(Hue) 바꾸기 |
| | `B` / `N` | 밝기 + / 밝기 − |
| 펌웨어 | `\` (**3초 누르기**) | 부트로더(DFU) 진입. 누르는 동안 언더글로 **빨간색**, 3초 전에 떼면 취소 |

> **팁:** LED 키(`X`, `C`, `B`, `N`)는 **Shift를 같이 누르면 반대 방향**으로 동작합니다.

## Fn 고정 (Fn Lock)

**Fn + Caps Lock**을 누르면 Fn을 누르고 있지 않아도 Fn 레이어가 유지됩니다 (WASD 방향키 등을 계속 쓸 때).
고정 중에 **Caps Lock** 또는 **Fn**을 누르면 풀립니다. 고정될 때와 풀릴 때 언더글로가 **흰색으로 5번 깜빡**입니다.
Caps Lock이 켜져 있어 이미 흰색일 때는 반대로 **꺼짐으로 깜빡**입니다.

> 고정 중에는 Fn 레이어에 기능이 있는 키(W A S D Q E R F P M , . / 등)는 글자 대신 Fn 기능으로 동작합니다.

## Caps Lock 표시

Caps Lock을 켜면 언더글로 12개가 **흰색**으로 바뀝니다.
조명이 꺼져 있거나 어떤 효과 모드든 상관없이 표시되고, Caps Lock을 끄면 원래 조명으로 돌아갑니다.

## LED 밝기와 전류

언더글로 12개를 흰색으로 켰을 때 키보드 전체 전류입니다 (USB 5V, 2026-10-13 측정).

| 밝기 | 전류 | 용도 |
|---|---|---|
| 180 | 200 mA | |
| 221 | 320 mA | **Caps Lock 흰색 표시** |
| 231 | 360 mA | **일반 조명 최대 밝기** (`max_brightness`) |
| 255 | 450 mA | 사용 불가: USB 폴리퓨즈 F1(유지 전류 0.5 A)이 트립되어 LED가 꺼짐 |

키맵은 [VIA](https://usevia.app)에서 바로 바꿀 수 있습니다. 정의 파일: [`jayking64_via.json`](jayking64_via.json)

## 빌드와 굽기

[QMK MSYS](https://msys.qmk.fm) 환경에서 실행합니다.

    qmk compile -kb jayking64 -km via

굽기 (먼저 아래 방법 중 하나로 부트로더에 진입):

    qmk flash -kb jayking64 -km via

See the [build environment setup](https://docs.qmk.fm/#/getting_started_build_tools) and the [make instructions](https://docs.qmk.fm/#/getting_started_make_guide) for more information. Brand new to QMK? Start with our [Complete Newbs Guide](https://docs.qmk.fm/#/newbs).

## 부트로더 진입

* **Bootmagic reset**: 왼쪽 위 키(Esc / `)를 누른 채로 USB를 꽂기 (저장된 VIA 설정이 초기화됨)
* **Physical reset button**: PCB의 SW1(BOOT) 버튼을 1초 정도 누르기
* **Keycode in layout**: Fn + `\`를 **3초** 누르기 (누르는 동안 빨간색)
