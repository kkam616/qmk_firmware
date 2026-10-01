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

## 3. 기본 배열: AEK64 (사용자 지정 기준 배열)

**AEK64 = Apple Extended Keyboard(AEK)에서 키캡과 ALPS 스위치를 옮겨 심는 배열.**
60% ANSI, **6.5U 스페이스**, Apple 스타일 아랫줄, 60키. 키캡은 Apple 스타일이지만 **기본 사용 OS는 Windows**.
QMK 배열 이름: `LAYOUT_aek64`

> 2026-10-01 수정: 처음 인수인계 때 "Space 7U / Option 1U"로 잘못 적혀 있었다. 사용자 확인 결과 기본은 6.5U / 1.25U다.
> 7U와 6.5U는 같은 스위치(S57, S58, S60, S62, S65, S67, S68)를 쓰므로 매트릭스는 같다.

```
행 0: ~ 1 2 3 4 5 6 7 8 9 0 - =  Delete(2U)
행 1: Tab(1.5) Q W E R T Y U I O P [ ]  \(1.5)
행 2: Caps(1.75) A S D F G H J K L ; '  Return(2.25)
행 3: Shift(2.25) Z X C V B N M , . /  Shift(2.75)
행 4: Control(1.5) Option(1.25) Command(1.5) Space(6.5) Command(1.5) Option(1.25) Control(1.5)
```

### 대체 배열 (PCB 멀티 레이아웃, 사용자 확인 2026-10-01)
| 영역 | 대체 배열 | 스위치 (행,열) |
|---|---|---|
| 백스페이스 | 1U + 1U 분할 | S14 (0,13) + S70 (4,13, 다른 홀 사용) |
| 엔터 | ISO 엔터 + `#` / Big-Ass Enter | 엔터는 S42 (2,13), `#`은 S41 (2,12) |
| Caps Lock | Stepped Caps | S29 그대로 (키캡만 다름) |
| 왼쪽 Shift | ISO 1.25U + 1U `\` | S43 (3,0) + S44 (3,1). 1U + 1.25U 조합은 미정 |
| 오른쪽 Shift | 1.75U + 1U / 1U + 1.75U | S56 (3,13) + S69 (4,12) / S55 (3,12) + S56 (3,13) |
| 아랫줄 | 7U / 6.5U (키 7개), 6.25U / 6U (키 8개) | 8개짜리는 S64 (4,7) 추가 |

넷리스트(`hardware/*.NET`)로 확인: 대체 스위치끼리는 **전기적으로 독립**(각자 다이오드, 다른 행/열 교차점)이다.
"둘 중 하나만 실장"은 **풋프린트가 물리적으로 겹치는 제약**이다.

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

## 5. QMK 작업 이력 (완료)
1. ✅ `keyboard.json`: 핀, 매트릭스, 다이오드, USB VID/PID(`0x4A6B/0x4A64`. 처음엔 `0xFEED`였으나 VIA가 0xFEED를 거부해서 2026-10-01 변경), 부트로더, rgblight(12개, 최대 밝기 120), layouts
2. ✅ `LAYOUT_aek64` (처음에는 `LAYOUT_60_ansi_7u`로 만들었다가 6.5U로 고치면서 이름 변경). S70 = row4/col13 주의
3. ✅ `keymaps/default`, `keymaps/via` (두 키맵 내용 동일, `via`는 `VIA_ENABLE = yes`)
4. ✅ WS2812 PWM (A6 / TIM3_CH1 / DMA1 ch3), `halconf.h`, `mcuconf.h`, `config.h`
5. ✅ `jayking64.c`: 크리스털이 없어서 HSI48을 USB SOF로 자동 보정하는 **CRS를 켬** (QMK 기본값은 CRS를 안 켬)
6. ✅ `jayking64_via.json`: VIA 정의 파일 (60키, 6.5U, LED 메뉴)
7. ✅ `hardware/`: 넷리스트 2개, 회로도 PDF, BOM. **넷리스트로 핀/매트릭스가 펌웨어와 100% 일치함을 확인함**

> 하드웨어 수치를 바꾸거나 추정할 때는 이 문서와 CSV/넷리스트를 기준으로 하고, 모호한 점은 사용자에게 확인한다.

---

## 6. 현재 상태와 다음 할 일 (2026-10-01 회사 PC 세션에서 정리)

### 사용자에 대해 (새 대화에서 Claude가 알아야 할 것)
- **하드웨어 개발자**(Altium, STM32 보드 설계). **펌웨어, git, VS Code는 처음**이다. 한국어로, 하드웨어 비유를 써서 쉽게 설명한다.
- 배우는 게 목적일 때는 **한 단계씩 설명하고 직접 해보게** 한다. "그냥 해줘"라고 하면 직접 처리한다.
- GitHub: `kkam616`. 커밋 작성자는 `Jay <83931616+kkam616@users.noreply.github.com>` (실제 이메일이 공개되지 않게 noreply 사용).
- **기본 사용 OS는 Windows**다 (키캡만 Apple 스타일, Mac은 안 씀).

### git
- 브랜치 `jayking64`. `origin` = 사용자 포크 `github.com/kkam616/qmk_firmware` (공개), `upstream` = `qmk/qmk_firmware`.
- QMK `.gitignore`가 `*.zip`, `*.png`, `*.pdf`, `keymaps/via/`를 제외한다. 이 키보드의 해당 파일들은 `git add -f`로 강제 추가했다. 새로 추가할 때도 `-f`가 필요하다.

### 현재 키맵 (Windows 기준, default와 via 동일)
```
기본 레이어 아랫줄:
 키캡: Control | Option | Command | Space | Command | Option | Control
 기능: Ctrl    | Win    | Alt     | Space | 한/영   | Fn     | 한자
                                            KC_LNG1   MO(_FN)  KC_LNG2
- 왼쪽 위 키 = QK_GESC (Esc, Shift/Win과 같이 누르면 `)
- Delete 키 = Backspace

Fn 레이어 (Fn이 오른손이라 이동 키는 왼손에 모음):
 Esc키=`  숫자줄=F1~F12  Backspace=Del
 Q Home  W ↑  E End  R PgUp
 A ←     S ↓  D →    F PgDn
 P = Print Screen,  \ = 부트로더(QK_BOOT)
 M 음소거  , 볼륨-  . 볼륨+  / 재생·일시정지
 Z~N = 언더글로 LED (켜기/끄기, 모드, 색상, 채도, 밝기+, 밝기-)
```
- 리셋 키(QK_RBT)는 사용자 요청으로 뺐다 (USB를 다시 꽂으면 됨).

### 빌드와 굽기 (QMK MSYS)
```
qmk compile -j 0 -kb jayking64 -km via
qmk flash -kb jayking64 -km via        # 빌드 + 굽기. 먼저 SW1을 1초 정도 꾹 눌러 DFU 진입
```
- 처음 굽기는 USB만으로 된다 (F072 ROM DFU, SW1 버튼). SWD 디버거는 필요 없다.
- Windows에서 DFU 장치를 못 찾으면 QMK Toolbox의 Tools → Install Drivers를 실행한다.

### 실보드 테스트 진행 상황 (2026-10-01 집 PC)
- ✅ ROM DFU 굽기 (SW1), USB 인식 → CRS/클럭 정상
- ✅ 납땜된 키 전부 정상 → **`COL2ROW` 다이오드 방향 확정**
- ✅ LED 켜짐, 모드 변경 확인 (12개 전부/색 순서/전류는 아직)
- ⚠️ 테스트 중 기존 키보드와 "충돌"로 PC 재부팅 1회 (증상 미확인, 재현 안 됨)
- ⏳ 남은 다이오드·스위치 납땜 후: 나머지 키, Fn+\ 부트로더 반복, 한/영·한자

### ⏭ 실보드 테스트 체크리스트
| 항목 | 방법 | 실패하면 |
|---|---|---|
| USB 인식 | 굽고 나서 키보드로 잡히는지 | `jayking64.c`의 CRS 코드, 클럭 설정 의심 |
| 키 60개 | VIA Key Tester 탭 | **키가 전부 안 되면** 다이오드 방향 의심: `keyboard.json`의 `diode_direction`을 `ROW2COL`로 바꿔본다 (넷리스트로는 다이오드 1번 핀이 애노드인지 확인 불가) |
| 한/영, 한자 | 메모장에서 오른쪽 Command / 오른쪽 Control | Windows 한국어 입력기와 키보드 드라이버 설정 확인 |
| **부트로더 진입** | 펌웨어를 구운 뒤 **Fn+\\** → 장치 관리자에 "STM32 BOOTLOADER". **5~10번 반복** | 안 되거나 가끔만 되면 **C9(470nF)가 소프트웨어 리셋을 방해**하는 것. 다음 리비전에서 100nF로 바꾸고 MCU 가까이 둔다 |
| LED | Fn+Z로 켜기. 12개 다 켜지는지, 색 순서(빨강 고르면 빨강인지) | 색이 바뀌어 나오면 색 순서(RGB/GRB) 설정 |
| LED 전류 | 흰색 최대 밝기에서 USB 전류 측정 | `rgblight.max_brightness`(지금 120)를 조정 |

### ⏭ 다음 작업 후보
1. **멀티 레이아웃 VIA 옵션**: `keyboard.json`에 `LAYOUT_all`(66키) 추가, 대체 스위치 6개(S14, S41, S44, S55, S64, S69)에 기본 키 넣기, VIA JSON에 레이아웃 옵션 메뉴(분할 백스페이스 / ANSI·ISO·BAE 엔터 / 왼쪽 Shift / 오른쪽 Shift / 아랫줄 4종). 3번 표 참고.
   - 미확정: ISO·BAE 엔터는 S42 자리를 "쓰는 것 같다"고 함 (아트웍으로 재확인 권장). 왼쪽 Shift 1U+1.25U 조합은 미정.
2. **Caps Lock 표시**: Caps Lock 상태 LED가 없어서, 켜지면 언더글로 색이 바뀌게 (rgblight layers). 실보드에서 LED 확인 후.
3. VIA로 며칠 써보고 마음에 드는 배치를 `keymap.c`에 반영.

### VS Code 세팅 (회사 PC는 완료, 집 PC는 다시 해야 함)
- 확장: **clangd** 사용, Microsoft C/C++의 IntelliSense는 끔 (`"C_Cpp.intelliSenseEngine": "disabled"`).
- **Windows에서 clangd 빨간 밑줄 해결**: QMK 루트의 `.clangd`가 `Compiler: clang`이라 Windows에서 x86용 코드로 해석해서 ChibiOS "Unknown compiler" 에러가 난다. `keyboards/jayking64/.clangd`(커밋됨)에 `--target=thumbv6m-none-eabi`를 넣어 해결했다. 추가로 빌드 정보 파일이 필요하다:
  `qmk compile --compiledb -j 0 -kb jayking64 -km via` (새 .c 파일 추가나 큰 설정 변경 때만 다시 실행)
- 사용자 설정: QMK MSYS 터미널 프로필 (`C:\QMK_MSYS\usr\bin\bash.exe --login`, `MSYSTEM=MINGW64`, `CHERE_INVOKING=1`), `files.watcherExclude`와 `search.exclude`에 `.build`, `lib`.
- `.vscode/tasks.json` (git 제외 파일): Ctrl+Shift+B = `qmk compile -j 0 -kb jayking64 -km via`, `problemMatcher: $gcc`.
