# Keyball44 / my_keymap

## 全体構成

ソースは2つのリポジトリに分かれている。

```
~/firmware/
├── keyball/                    … Yowkees/keyball のクローン (branch: main)
│   └── qmk_firmware/keyboards/keyball/   ← Keyball 関連の実体はすべてここ
├── qmk_firmware/               … qmk/qmk_firmware のクローン (branch: main)
│   └── keyboards/keyball  ->  ../../keyball/qmk_firmware/keyboards/keyball
└── .venv/                      … qmk CLI (Python)
```

`qmk_firmware/keyboards/keyball` は **シンボリックリンク**であって実体ではない。
以前は両方に同じファイルのコピーが存在していたが、片方だけ編集する事故を防ぐため
実体を keyball リポジトリ側に一本化した。編集は常にこのリポジトリ側で行う。

### ブランチ

| リポジトリ | ブランチ | 内容 |
|---|---|---|
| `keyball` | `main` | このキーマップ、`probe` キーマップ、`keyball44/config.h` と `lib/keyball/keyball.c` への変更 |
| `qmk_firmware` | `main` | QMK 0.22.14 (`ca454169`) + `quantum/rgblight/rgblight.c` へのコアパッチ + 上記シンボリックリンク |

## QMK コアへのパッチ（重要）

このキーマップは **`quantum/rgblight/rgblight.c` への変更に依存している**。
電流バジェットによる減光と LED ゾーンマスクがそこに入っていないと、
`RGB_CURRENT_BUDGET` や `LED_MODE` キーが機能しない。

`rgblight_call_driver()` は weak 宣言されているのにキーマップ側から override できない。
`rgblight.c` が同一翻訳単位内で自前の定義をインライン展開してしまうためで、
`LTO_ENABLE = yes` がこれを助長している。そのためコアを直接触っている。

パッチは `qmk_firmware` の `main` ブランチにコミット済み。控えとして
[`patches/qmk-0.22.14-rgblight-current-budget.patch`](patches/qmk-0.22.14-rgblight-current-budget.patch)
も同梱してあるので、QMK を別バージョンに更新したり再クローンした場合はこれを当て直す。

> **移行時のTODO:** Liatris (RP2040) では容量に余裕ができるので `LTO_ENABLE` を切れる。
> そうすると weak override が本来どおり効くようになり、このコアパッチは不要になって
> keymap.c 側へ移せる可能性が高い。移行時に検証すること。

## ビルド

```bash
cd ~/firmware/qmk_firmware

# Pro Micro (ATmega32U4) — 現行
../.venv/bin/qmk compile -kb keyball/keyball44 -km my_keymap
#   -> keyball_keyball44_my_keymap.hex

# Liatris (RP2040) — コンスルー入手後
../.venv/bin/qmk compile -kb keyball/keyball44 -km my_keymap -e CONVERT_TO=liatris
#   -> keyball_keyball44_my_keymap_liatris.uf2
```

必要なツールチェーン:

```bash
sudo apt install gcc-avr avr-libc binutils-avr   # Pro Micro
sudo apt install gcc-arm-none-eabi               # Liatris
```

### 容量

Pro Micro (caterina) の上限は 28,672 バイトで、現状 **28,176 バイト / 残り 496 バイト**。
非常にタイトなので、機能追加のたびにビルド時の警告を確認すること。
足りなくなったら `config.h` の `RGBLIGHT_EFFECT_*` を削ると 1 効果あたり 200 バイト前後戻る
（CHRISTMAS / RGB_TEST / ALTERNATING の3つで実測 624 バイト）。

Liatris では 2 MB（QMK の RP2040 リンカスクリプトが 16 MB のうち 2 MB を使う）に対して
48 KB しか使わないので、容量のために削った機能（OLED、Auto Mouse Layer、LTO 等）は
すべて戻せる。

## 書き込み

### Pro Micro
リセット2回押しでブートローダに入り、`qmk flash` または Remap のファームウェア書き込み。

### Liatris (RP2040)
方式が変わり、**UF2 のドラッグ&ドロップ**になる。

1. Liatris 裏面の BOOT ボタンを押しながら USB-C を挿す（QMK 書き込み済みならリセットボタンのダブルタップでも可）
2. `RPI-RP2` という USB ドライブが現れる
3. `.uf2` をそのドライブへコピー → 自動で再起動
4. **左右それぞれに対して実施**（同一の .uf2 でよい。左右判定はマトリクスグリッド）

WSL からは `cp .../keyball_keyball44_my_keymap_liatris.uf2 /mnt/<ドライブレター>/`。

書き込み後は **EEPROM のクリアが必須**（AVR の EEPROM から RP2040 の flash
ウェアレベリング領域に変わるため、キーマップ・レイヤ・RGB 設定は引き継がれない）。
Remap の「Clear EEPROM」を実行してから設定を入れ直す。

なお **Remap のファームウェア書き込み機能は Liatris では使えない**（Caterina/DFU 専用で
UF2 非対応）。キーマップ編集（WebHID/VIA）はこれまでどおり使える。

## このキーマップの構成要素

### LED ゾーン切り替え (`LED_MODE`)
キーコード `0x7E40` を Remap の ANY で割り当てる。押すたびに
全点灯 → 上部アンダーグロー消灯 → アンダーグロー全消灯 → 全消灯 を巡回する。
マスクは `rgblight_call_driver`（コアパッチ側）で行っているので全モードに効く。

状態は `eeconfig_read_user()` で永続化している。**パターンを5つ以上に増やす場合は
`keymap.c` の復元側のマスク `u & 0x03` も広げること**（忘れると電源断で巻き戻る）。

### 座標ベースの RGB エフェクト
LED のチェーン順は物理的な並びと一致していないため、実測した1D座標
（[`led-1d-coordinates.csv`](led-1d-coordinates.csv)）を `keymap.c` の `led_coord[]` に
焼き込み、そこから発色させている。SNAKE / RAINBOW_SWIRL / STATIC_GRADIENT の
スロットを乗っ取る形で実装。

座標の実測には同梱の **`probe` キーマップ**を使う（チェーンインデックスを1つずつ
点灯させて物理位置を記録するための専用ファーム）。

### 電流バジェット
`config.h` の `RGB_CURRENT_BUDGET` と `RGB_WEIGHT_R/G/B`。実測したちらつき挙動に
合わせて較正済みなので、値を動かす場合は config.h のコメントの手順に従うこと。

### VIA レイヤ数
`DYNAMIC_KEYMAP_LAYER_COUNT 6`（QMK 既定の4から拡張）。
**変更すると EEPROM のレイアウトがずれるので、書き込み後に EEPROM リセットが必要。**
