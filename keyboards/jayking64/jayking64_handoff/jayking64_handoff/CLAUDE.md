# JAYKING64 — 프로젝트 인수인계 (Cowork → Claude Code)

이 파일은 Claude Cowork 세션에서 한 PCB/회로도 검토 결과를 정리한 것이다.
다음 단계는 **QMK 펌웨어 작업**이다. 하드웨어 사실관계는 아래 내용과 `matrix_map.csv`를 기준으로 한다.

- 작성일: 2026-10-01
- 원본 설계: Altium, `JAYKING64_Rev1r10` (프로젝트 종료, 아트웍 완료 상태)
- 같은 폴더의 파일
  - `CLAUDE.md`: 이 문서
  - `matrix_map.csv`: 스위치 66개 → 다이오드, 행, 열, 키 이름, 풋프린트, PCB 좌표
  - `layout_60_ansi_7u.kle.json`: 기본 배열 (keyboard-layout-editor.com의 Raw data로 붙여넣으면 열림)
- 원본 파일(사용자가 보관 중): `JAYKING64_Rev1r10.PcbDoc / .PcbLib / .SCHLIB / .xls(BOM)`,
  `01. MCU  LED.SchDoc`, `02. Matrix.SchDoc`, `01. MCU  LED.NET`, `02. Matrix.NET`(Protel 넷리스트)

---

## 1. 하드웨어 요약

| 항목 | 내용 |
|---|---|
| MCU | **STM32F072CBT6** (LQFP48, Flash 128KB, 크리스털 없음, USB는 HSI48 + CRS) |
| 스위치 | **ALPS (SKCM/SKCL 계열)**, 위치 66개(대체 배열 포함), 스위치는 PCB **Bottom** 면에 실장 |
| 매트릭스 | **5행 × 14열**, 키마다 다이오드(SOD-123) |
| 다이오드 방향 | **`COL2ROW`** (스위치 → 다이오드 A → K → 행) |
| USB | USB-C `TYPE-C-31-M-12`(CON2) **또는** JST SH 4핀 UDB 도터보드(CON3). 같은 선에 병렬 연결돼 있으니 **둘 중 하나만 실장** |
| LED | **WS2812B-2020 × 12** (언더글로, PCB Top 면), 74AHCT1G125로 5V 레벨 변환 |
| 부트 | SW1(BOOT 버튼): 누르면 BOOT0=High와 NRST 펄스가 동시에 걸려 **ROM DFU**로 진입 |
| 디버그 | CON1 5핀 2.54mm: 1 VCC3V3, 2 SWCLK, 3 GND, 4 SWDIO, 5 NRST |
| PCB | 2층, 약 284.8 × 94.3 mm, 양면 GND 해치 폴리곤, 열 사이 슬롯(flex cut) 있음 |

### 전원 경로
VBUS → F1(1206L050, 유지 0.5A) → Q1(SI2301 PMOS, 역전압 보호, 게이트는 R3 10k로 GND) → **VCC(5V)** → U2 XC6206P332 → **VCC3V3**

---

## 2. MCU 핀 맵 (QMK용)

| 기능 | 핀 |
|---|---|
| ROW 0–4 | `B3, B4, B5, B6, B9` |
| COL 0–13 | `B7, A15, C15, C14, C13, F0, B13, B12, B11, B10, B2, B1, A8, B0` |
| WS2812 DI | `A6` (TIM3_CH1) → R8 22Ω → 74AHCT1G125 → R7 470Ω → LED1 DIN |
| USB | `A11` D−, `A12` D+ |
| SWD | `A13` SWDIO, `A14` SWCLK |
| BOOT0 | BOOT0 핀(44), R5 10k 풀다운 |
| 미사용 | A0–A5, A7, A9, A10, B8, B14, B15, F1 (아날로그로 두면 됨) |

참고: C13–C15와 F0은 전류를 많이 못 내는 핀이다. `COL2ROW`에서 열은 입력이라 문제없다.

### QMK 설정 초안 (확정 아님, 작업 시작점)
```
processor:      STM32F072
bootloader:     stm32-dfu
diode_direction: COL2ROW
matrix_pins.rows: [B3, B4, B5, B6, B9]
matrix_pins.cols: [B7, A15, C15, C14, C13, F0, B13, B12, B11, B10, B2, B1, A8, B0]
ws2812: pin A6, driver pwm (TIM3 CH1, DMA 설정 필요), LED 12개
rgblight: 언더글로 12개, RGBLIGHT_LIMIT_VAL로 밝기 제한 필수 (아래 4번 참고)
```

---

## 3. 기본 배열 (사용자 지정 기준 배열)

**60% ANSI, 7U 스페이스, Apple 스타일 아랫줄, 60키** (사용자가 사진으로 지정)

```
행 0: ~ 1 2 3 4 5 6 7 8 9 0 - =  Delete(2U)
행 1: Tab(1.5) Q W E R T Y U I O P [ ]  \(1.5)
행 2: Caps(1.75) A S D F G H J K L ; '  Return(2.25)
행 3: Shift(2.25) Z X C V B N M , . /  Shift(2.75)
행 4: Control(1.5) Option(1) Command(1.5) Space(7) Command(1.5) Option(1) Control(1.5)
```

### 매트릭스 배치 (이 배열에서 쓰는 60개)
```
        c0   c1   c2   c3   c4   c5   c6   c7   c8   c9  c10  c11  c12  c13
row0    S1   S2   S3   S4   S5   S6   S7   S8   S9  S10  S11  S12  S13   --
row1   S15  S16  S17  S18  S19  S20  S21  S22  S23  S24  S25  S26  S27  S28
row2   S29  S30  S31  S32  S33  S34  S35  S36  S37  S38  S39  S40   --  S42
row3   S43   --  S45  S46  S47  S48  S49  S50  S51  S52  S53  S54   --  S56
row4   S57  S58   --  S60   --  S62   --   --  S65   --  S67  S68   --  S70
```
- **S70 = Delete(2U)**: 물리적으로는 맨 윗줄 끝이지만 **매트릭스상 row4/col13**에 있다. 키맵 LAYOUT 매크로에서 위치를 맞춰야 한다.
- 이 배열에서 쓰지 않는 대체 위치: S14(분할 백스페이스), S41(ISO #), S44(ISO \\), S55·S69(분할 오른쪽 Shift), S64(1.25U 대체). 매트릭스 좌표는 `matrix_map.csv`에 있다.
- 스위치가 Bottom 면에 있어서 PCB Top 뷰에서는 좌우가 뒤집혀 보인다. 키캡 쪽에서 보면 S1이 맨 왼쪽 위다.

---

## 4. 검토 결과 요약

**결론: 기능을 망가뜨리는 큰 이슈는 없다.** 넷리스트와 PCB가 100% 일치하고, 단락·미연결·간격 위반(7.2mil)이 없다.

### 다음 리비전에서 우선 고칠 것
1. **[PCB] 패드 안의 비아**: WS2812B-2020 데이터 핀(LED1–4, LED11), U3(핀 1·3·5), C3-2. 리플로 때 솔더가 비아로 빨려 들어갈 위험이 있다. 비아를 패드에서 0.15mm 이상 떼거나 POFV(비아 메움)를 쓴다.
2. **[회로/PCB] NRST**: C9가 470nF로 ST 권장 100nF보다 크고, MCU에서 약 78mm 떨어져 있다(NRST 배선 약 129mm, BOOT0 약 93mm). 100nF를 MCU 핀 옆에 둔다.
   → **펌웨어 확인 항목: QMK 소프트웨어 리셋과 부트로더 진입(QK_BOOT)이 정상 동작하는지 실보드로 테스트할 것.**
3. **[펌웨어] LED 전류**: LED 12개를 최대 밝기 흰색으로 켜면 F1 유지 전류 0.5A와 USB 500mA 한도에 가깝다. `RGBLIGHT_LIMIT_VAL`을 반드시 건다(시작값 약 120–150/255 권장, 실측해서 조정).

### 기타 (개선 권장)
- [회로] 3.3V 대용량 커패시터 부족: 4.7µF 추가, VDDA에 10nF 추가 권장
- [PCB] USB D−가 오른쪽 노치에서 보드 끝과 0.64mm, 보드 끝 2mm 안을 지나는 구간 약 134mm. USB 배선 전체 약 235mm
- [PCB] S28(1.5U) 패드 애뉼러 링 0.07mm (패드 폭 2.1mm, 구멍 1.96mm)
- [PCB] 스테빌라이저 NPTH 구멍과 동박 사이 0.19–0.24mm (7곳)
- [PCB] S21, S16이 그리드에서 0.03mm 벗어남 (영향 없음)
- [PCB] 적층 설정의 절연층이 12.6mil로 돼 있음 → 실제 두께로 수정 필요
- [회로] 행·열을 전원 포트 심볼(COL_x/ROW_x)로 시트 간 연결해서 넷 이름이 이중으로 붙어 있음
- [조립] 다이오드 라이브러리 1번 핀 = 애노드(IPC 관례와 반대). 조립을 맡길 때 실크의 캐소드 표시 기준이라고 명시할 것
- [조립] CON2/CON3, S14/S70, S28/S42는 둘 중 하나만 실장하는 위치

### 확인된 정상 동작 요소
CC 5.1kΩ 개별 저항, SRV05 ESD(VBUS·D±), USB-C 실드는 페라이트(BLM21) + 0.1µF로 GND에 연결, LDO 핀 배치와 입출력 1µF, MCU 0.1µF ×5가 핀 3.5mm 이내, LED마다 0.1µF, AHCT 버퍼로 5V 레벨 변환, BOOT 버튼 자동 DFU 회로(C8 4.7µF / R6 100k → Q2가 NRST를 약 0.2초 Low로 잡음).

---

## 5. 다음 작업 (QMK)
1. `keyboards/jayking64/` 생성: `keyboard.json`(핀, 매트릭스, 다이오드, USB VID/PID, 부트로더, rgblight, layouts)
2. `LAYOUT_60_ansi_7u` 매크로 작성: 위 3번 매트릭스 배치와 `matrix_map.csv` 기준, S70 위치 주의
3. `keymaps/default/keymap.c`: Apple 스타일(Option = Alt, Command = GUI) + Fn 레이어
4. WS2812 PWM 드라이버 설정(A6 / TIM3_CH1 / DMA), `halconf.h` / `mcuconf.h`
5. 빌드 → `stm32-dfu`로 플래시 → 실보드 테스트(매트릭스 전체, 소프트 리셋·부트로더 진입, LED 전류)

> 하드웨어 수치를 바꾸거나 추정할 때는 이 문서와 CSV를 기준으로 하고, 모호한 점은 사용자에게 확인한다.
