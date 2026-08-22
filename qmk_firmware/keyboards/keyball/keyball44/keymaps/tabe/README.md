# Keyball44 / tabe キーマップ

## 全体構成

ソースは2つのリポジトリに分かれている。

```
/home/tabas/firmware/
├── keyball/                    … Yowkees/keyball のクローン (branch: tabe)
│   └── qmk_firmware/keyboards/keyball/   ← Keyball 関連の実体はすべてここ
├── qmk_firmware/               … qmk/qmk_firmware のクローン (branch: tabe)
│   └── keyboards/keyball  ->  ../../keyball/qmk_firmware/keyboards/keyball
└── .venv/                      … qmk CLI (Python)
```

`qmk_firmware/keyboards/keyball` は **シンボリックリンク**であって実体ではない。
以前は両方に同じファイルのコピーが存在していたが、片方だけ編集する事故を防ぐため
実体を keyball リポジトリ側に一本化した。編集は常にこのリポジトリ側で行う。

### ブランチ

| リポジトリ | ブランチ | 内容 |
|---|---|---|
| `keyball` | `tabe` | このキーマップ、`probe` キーマップ、`keyball44/config.h` と `lib/keyball/keyball.c` への変更 |
| `qmk_firmware` | `tabe` | QMK 0.22.14 (`ca454169`) + `quantum/rgblight/rgblight.c` へのコアパッチ + 上記シンボリックリンク |

## QMK コアへのパッチ（重要）

このキーマップは **`quantum/rgblight/rgblight.c` への変更に依存している**。
電流バジェットによる減光と LED ゾーンマスクがそこに入っていないと、
`RGB_CURRENT_BUDGET` や `LED_MODE` キーが機能しない。

`rgblight_call_driver()` は weak 宣言されているのにキーマップ側から override できない。
`rgblight.c` が同一翻訳単位内で自前の定義をインライン展開してしまうためで、
`LTO_ENABLE = yes` がこれを助長している。そのためコアを直接触っている。

パッチは `qmk_firmware` の `tabe` ブランチにコミット済み。控えとして
[`patches/qmk-0.22.14-rgblight.patch`](patches/qmk-0.22.14-rgblight.patch)
も同梱してある（コミット単位ではなく 0.22.14 からの累積差分）。QMK を別バージョンに
更新したり再クローンした場合はこれを当て直す。**コアパッチを変更したら再生成すること**:

```bash
cd /home/tabas/firmware/qmk_firmware
git diff ca454169 -- quantum/rgblight/rgblight.c \
  > keyboards/keyball/keyball44/keymaps/tabe/patches/qmk-0.22.14-rgblight.patch
```

> **移行時のTODO:** Liatris (RP2040) では容量に余裕ができるので `LTO_ENABLE` を切れる。
> そうすると weak override が本来どおり効くようになり、このコアパッチは不要になって
> keymap.c 側へ移せる可能性が高い。移行時に検証すること。

## ビルド

```bash
cd /home/tabas/firmware/qmk_firmware

# Pro Micro (ATmega32U4) — 現行
../.venv/bin/qmk compile -kb keyball/keyball44 -km tabe
#   -> keyball_keyball44_tabe.hex

# Liatris (RP2040) — コンスルー入手後
../.venv/bin/qmk compile -kb keyball/keyball44 -km tabe -e CONVERT_TO=liatris
#   -> keyball_keyball44_tabe_liatris.uf2
```

必要なツールチェーン:

```bash
sudo apt install gcc-avr avr-libc binutils-avr   # Pro Micro
sudo apt install gcc-arm-none-eabi               # Liatris
```

### 容量

Pro Micro (caterina) の上限は 28,672 バイトで、現状 **28,568 バイト / 残り 104 バイト**。
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

WSL からは `cp .../keyball_keyball44_tabe_liatris.uf2 /mnt/<ドライブレター>/`。

書き込み後は **EEPROM のクリアが必須**（AVR の EEPROM から RP2040 の flash
ウェアレベリング領域に変わるため、キーマップ・レイヤ・RGB 設定は引き継がれない）。
Remap の「Clear EEPROM」を実行してから設定を入れ直す。

なお **Remap のファームウェア書き込み機能は Liatris では使えない**（Caterina/DFU 専用で
UF2 非対応）。キーマップ編集（WebHID/VIA）はこれまでどおり使える。

## このキーマップの構成要素

### LED ゾーン切り替え (`LED_MODE`)
キーコード `0x7E40` を Remap の ANY で割り当てる。押すたびに以下を巡回する。

| 状態 | 内容 | 点灯数 |
|---|---|---|
| 0 | 全点灯 | 60 |
| 1 | 上部アンダーグロー消灯 | 54 |
| 2 | アンダーグロー全消灯 | 40 |
| 3 | 「3x5」キーブロックのみ点灯 | 30 |
| 4 | 全消灯 | 0 |

マスクは `rgblight_call_driver`（コアパッチ側）で行っているので全モードに効き、
消灯した LED は電流バジェットの計算からも外れる。状態は `eeconfig_read_user()`
で永続化し、`housekeeping_task_user` からスレーブ側へ同期している。

ゾーンの定義はすべて `config.h` にある。状態を増減するときは:

- `RGB_LED_STATE_COUNT` を更新する（`keymap.c` の巡回と範囲チェックが参照する）
- `rgblight.c` の `RGB_HIDDEN` マクロに case を足す。**番号は末尾ではなく
  意図した位置に挿入する**（「全消灯」は常に最後の番号でいてほしい）
- **6状態を超えるなら `keymap.c` の EEPROM 復元マスク `u & 0x07` を広げる**
  （忘れると電源断で巻き戻る）

`3x5` ゾーンの集合は [`led-1d-coordinates.csv`](led-1d-coordinates.csv) の
`3x5領域か` 列が実体で、`config.h` の `RGB_3X5_MASK_BYTES` はそこから生成した
8バイトのビットマスク。**CSV を編集したらマスクを再生成すること**:

```bash
python3 tools/gen-zone-mask.py            # 既定の 3x5領域か 列
python3 tools/gen-zone-mask.py 上部アンダーグローか   # 他の列も可
```

生成結果を `config.h` に貼り直す。スクリプトは以下を検証する:

- 列の値が半角の `0` / `1` のみであること（全角数字や空欄はエラーで止める。
  黙って「立っていない」と解釈されると気づけないため）
- 生成したマスクから復元した集合が CSV と一致すること

あわせて左右の内訳（何列×何行か）を出力する。非対称なら大抵は入力ミス。

### 座標ベースの RGB エフェクト
LED のチェーン順は物理的な並びと一致していないため、実測した1D座標
（[`led-1d-coordinates.csv`](led-1d-coordinates.csv)）を `keymap.c` の `led_coord[]` に
焼き込み、そこから発色させている。SNAKE / RAINBOW_SWIRL / STATIC_GRADIENT の
スロットを乗っ取る形で実装。

座標の実測には同梱の **`probe` キーマップ**を使う（チェーンインデックスを1つずつ
点灯させて物理位置を記録するための専用ファーム）。

### ボール変位でスクロールを即座に開始
スクロールレイヤ（`SCROLL_LAYER`、既定は 3）は tap/hold キーの hold 側で踏むが、
素の QMK では hold が確定するまで `TAPPING_TERM`（200ms）待つため、ボールを
動かし始めてから実際にスクロールに変わるまでが体感でかなり遅い。

QMK は毎スキャン `WITHIN_TAPPING_TERM` を評価し直しており（`keyboard_task` が
`action_exec(MAKE_TICK_EVENT)` を回す）、そのたびに `get_tapping_term()` を呼ぶ。
つまり**判定が保留中のあいだに tapping term を縮めると、遡って期限切れになる**。
`action_tapping.c` の "after TAPPING_TERM" 分岐に入り、`tap.count == 0` なので
hold として確定する。これを利用して、ボールが動いた瞬間に term を 0 にしている。

- 検出は `pointing_device_task_user`（マスタ側で、両手の motion 適用後に走る）。
  `r.x`/`r.y` は除算後の値なので、カーソルが動かない程度のセンサのゆらぎは拾わない。
- 対象は **hold 先が `SCROLL_LAYER` の layer-tap だけ**。全ての layer-tap に
  掛けると、親指をスペースに置いたままボールに触れただけでレイヤが変わる。
- **キーマップは Remap（EEPROM）側が実体**なので、特定のキーコードには依存せず、
  「hold 先が `SCROLL_LAYER` の layer-tap」という条件だけで判定している。
  Remap で tap 側に何を割り当てても効く。
- ただし **hold 先のレイヤ番号を変えたら `config.h` の `SCROLL_LAYER` も直すこと**。
  `layer_state_set_user` のスクロール判定も同じ定数を見ている。

タイムスタンプは「変位が押下より後か」を符号付き16bit差で見ているが、値が
32.7秒以上古くなるとラップして「未来」に見えてしまうため、`TAPPING_TERM` を
過ぎた時点で無効化している（これを入れないと、起動後ボールに触れずに一定時間
経ってから押した最初の tap/hold が即 hold になる）。

なお Keyball の `SCRL_MO`（Remap 上の `kb 7`）は tap/hold ではなく
**momentary**（押している間だけスクロール）なので、そもそも待ち時間はない。
遅延が出るのは layer-tap の hold 側でレイヤを踏んでいる場合。

### 電流バジェット
`config.h` の `RGB_CURRENT_BUDGET` と `RGB_WEIGHT_R/G/B`。実測したちらつき挙動に
合わせて較正済みなので、値を動かす場合は config.h のコメントの手順に従うこと。

### VIA レイヤ数
`DYNAMIC_KEYMAP_LAYER_COUNT 6`（QMK 既定の4から拡張）。
**変更すると EEPROM のレイアウトがずれるので、書き込み後に EEPROM リセットが必要。**
