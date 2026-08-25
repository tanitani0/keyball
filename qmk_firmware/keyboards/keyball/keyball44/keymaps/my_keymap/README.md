# Keyball44 / my_keymap

本家: [Yowkees/keyball](https://github.com/Yowkees/keyball) ／
コントローラは Pro Micro から [Liatris](https://splitkb.com/products/liatris) (RP2040) に換装済み。

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
| `qmk_firmware` | `main` | QMK 0.22.14 (`ca454169`) + 上記シンボリックリンクのみ（コア改造なし） |

## ビルド

```bash
cd ~/firmware/qmk_firmware
../.venv/bin/qmk compile -kb keyball/keyball44 -km my_keymap
#   -> keyball_keyball44_my_keymap_liatris.uf2
```

`CONVERT_TO = liatris` は `rules.mk` に書いてあるのでコマンドラインでの指定は不要。
Pro Micro (ATmega32U4) 向けにビルドしたい場合はその行を外すが、**復活させた機能を
いくつか削らないと 28,672 バイトに入らない**。

必要なツールチェーン:

```bash
sudo apt install gcc-arm-none-eabi               # Liatris (RP2040)
sudo apt install gcc-avr avr-libc binutils-avr   # Pro Micro に戻す場合のみ
```

`make keyball/keyball44:my_keymap` のように **make を直接叩く場合は、`qmk` 自体が PATH に
必要**（ビルド中に make が qmk を呼ぶ）。venv を有効化しておくこと:

```bash
source ~/firmware/.venv/bin/activate
```

上記の `qmk compile` をフルパスで呼ぶ形なら不要。

### 容量

RP2040 の使える flash は **2 MB**（Liatris の実装は 16 MB だが、QMK の RP2040 リンカ
スクリプトが 2 MB 前提。EEPROM 相当のウェアレベリング領域もその末尾に置かれる）。
現状 **約 65 KB / 3%** しか使っていないので、当面は容量を気にする必要はない。

Pro Micro 時代は 28,672 バイト中 残り 22 バイトまで詰まっており、OLED・EXTRAKEY・
NKRO・BOOTMAGIC・MAGIC などを泣く泣く切っていた。移行後にすべて戻してある
（`rules.mk` 参照）。意図的に切ったままなのは MOUSEKEY（トラックボールがあるので不要）、
CONSOLE/COMMAND（デバッグ用）、Auto Mouse Layer（操作感が変わるため不要との判断）。

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
キーコード `0x7E40` を Remap の ANY で割り当てる。押すたびに以下を巡回する。

| 状態 | 内容 | 点灯数 |
|---|---|---|
| 0 | 全点灯 | 60 |
| 1 | 上部アンダーグロー消灯 | 54 |
| 2 | アンダーグロー全消灯 | 40 |
| 3 | 「3x5」キーブロックのみ点灯 | 30 |
| 4 | 全消灯 | 0 |

状態はマスタからスレーブへ1秒ごとに送り直している。**変化時だけ送るのでは足りない。**
`transaction_rpc_send` が成功を返しても、スレーブが受け取ったとは限らないため
（[transactions.c] はスレーブ側でハンドラが未登録のときコールバックを飛ばすが、
トランザクション自体は成功する）。起動時にマスタの送信がスレーブの
`keyboard_post_init_user` より先に走ると握りつぶされ、スプリットのウォッチドッグや
ケーブル抜けでスレーブだけ再起動したときも同じことが起きる。どちらの場合も、
以前は `LED_MODE` を押し直すまで左側だけ元の状態のままだった。

マスクは `keymap.c` の `rgblight_call_driver`（QMK の weak シンボルを上書き）で
行っているので全モードに効き、
消灯した LED は電流バジェットの計算からも外れる。状態は `eeconfig_read_user()`
で永続化し、`housekeeping_task_user` からスレーブ側へ同期している。

> この上書きが効くのは `rules.mk` で `LTO_ENABLE = no` にしているから。LTO を有効に
> すると `rgblight.c` が自前の weak 定義をインライン展開してしまい、**エラーも警告も
> 出ないまま上書きが無視される**（LED ゾーンと電流制御が丸ごと効かなくなる）。
> Pro Micro 時代は容量の都合で LTO を切れず、やむなく QMK コアを直接改造していた。

ゾーンの定義はすべて `config.h` にある。状態を増減するときは:

- `RGB_LED_STATE_COUNT` を更新する（`keymap.c` の巡回と範囲チェックが参照する）
- `keymap.c` の `RGB_HIDDEN` マクロに case を足す。**番号は末尾ではなく
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
焼き込み、そこから発色させている。組み込みエフェクトのスロットを乗っ取る形で実装。

| 乗っ取り元 | モード番号 | 内容 |
|---|---|---|
| SNAKE | 15〜20 | 座標上を走る山 |
| RAINBOW_SWIRL | 9〜14 | 流れる虹 |
| STATIC_GRADIENT | 25〜34 | 静止した虹（左端→右端） |
| KNIGHT | 21〜23 | **静止した虹（左右対称）** |

**乗っ取ったエフェクトも `config.h` で有効にしたままにすること。** `RGBLIGHT_EFFECT_*`
を消すとモード番号が繰り上がり、上記の対応が全部ずれる。

左右対称版は、色相を座標そのものではなく**中心からの距離**で決めている。実測座標が
ちょうど対称になっている（左 0〜98、右 126〜224、中心 112 から左右とも 112、
内側の端も中心から±14）ので、これで正確に鏡像になる。

色相の起点は現在設定している色相で、**左右の外側の端がその色**になり、中心に向かって
色相が回って `色相+range` で合流する。3つのサブモードは回る量の違いで、
`fx_mirror_range[3] = {128, 96, 64}`（半周 / 中間 / 控えめ）。

全周（255）は6列に対して隣同士で約40も色相が飛ぶため、細かすぎて落ち着かなかった。
向きを逆（中心が設定色相で外向きに回る）にしたい場合は、`fx_render` の
`(FX_CENTRE - d)` を `d` に戻す。

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

スクロールレイヤだけ tapping term を **400ms**（`SCROLL_LAYER_TAPPING_TERM`）に
延ばしてある。既定の 200ms では tap 側を取りこぼしやすかったため。ボールを
動かせば即 hold に確定するので、**レイヤに入るまでが遅くなることはない**。
延びるのは「ボールに触れずにキーだけ押している」場合の判定時間だけ。
他のレイヤと mod-tap は 200ms のまま。

ボール変位のタイムスタンプを失効させる時間もこの 400ms に合わせてある
（この判定が開いていられる最長時間なので）。

タイムスタンプは「変位が押下より後か」を符号付き16bit差で見ているが、値が
32.7秒以上古くなるとラップして「未来」に見えてしまうため、`TAPPING_TERM` を
過ぎた時点で無効化している（これを入れないと、起動後ボールに触れずに一定時間
経ってから押した最初の tap/hold が即 hold になる）。

なお Keyball の `SCRL_MO`（Remap 上の `kb 7`）は tap/hold ではなく
**momentary**（押している間だけスクロール）なので、そもそも待ち時間はない。
遅延が出るのは layer-tap の hold 側でレイヤを踏んでいる場合。

### レイヤ1, 2 の hold を早く確定させる (permissive hold)
レイヤ1・2 も hold で踏んでいるが、素の QMK では `TAPPING_TERM` を待ち切らないと
hold にならず、切り替わりが遅い。`PERMISSIVE_HOLD_PER_KEY` を使って、
**他のキーを入れ子に押して離した時点で hold 確定**にしている。

対象レイヤは `config.h` の `PERMISSIVE_HOLD_LAYERS`（レイヤ番号のビットマスク）。

`HOLD_ON_OTHER_KEY_PRESS`（他キーの「押し」で確定）ではなく permissive hold
（「離し」で確定）にしているのは、レイヤ1の tap 側がスペースだから:

```
入れ子（レイヤを使いたいとき）
  LT↓  X↓  X↑  LT↑   -> どちらの方式でも レイヤ1のX

ローリング（高速に打っているとき）
  LT↓  X↓  LT↑  X↑   -> permissive hold      : スペース + X   ← 意図どおり
                        hold on other key press: レイヤ1のX     ← 誤発動
```

per-key（グローバルではなく）なのは mod-tap を巻き込まないため。
`LCTL_T(KC_LNG2)` のような英数/かなキーからローリングで次の文字へ移ったときに
Ctrl+文字 になってしまう。

実測（AVR、ボール機能まで入れた 28,568 が基準）:

| 方式 | サイズ | 残り |
|---|---|---|
| `PERMISSIVE_HOLD`（グローバル） | 28,580 | 92 |
| **`PERMISSIVE_HOLD_PER_KEY` + `get_permissive_hold`**（採用） | **28,650** | **22** |
| `HOLD_ON_OTHER_KEY_PRESS_PER_KEY` + フック | 28,786 | **容量超過** |
| `get_tapping_term` を流用した自前実装（押しで確定） | 28,616 | 56 |

### Liatris 本体の電源 LED を消す
Liatris は基板上に自前のオレンジ色の電源 LED を持っている（**GP24**）。キーボードに
組むと眩しく、位置も邪魔なので `keymap.c` の `keyboard_pre_init_user` で消している。

**論理が反転していて、ピンを High にすると消灯**する（splitkb のドキュメントに明記）。
左右両方で走る（keyball の `keyboard_pre_init_kb` がここを呼ぶ）。

電源投入直後、ファームウェアが走り始めるまでの一瞬は点灯する。これは基板の配線に
よるものでファームウェアからは抑えられない。

ピン名を `GP24` ではなく生の `24U` で書いているのは、promicro コンバータの
`_pin_defs.h` が RP2040 ベンダ側のものを覆い隠しており、Pro Micro 名（`D3` など）
しか見えないため。ベンダ側で `GP24` は `24U` と定義されているので値は同じ。

戻したい場合はこの関数を消す。Caps Lock インジケータとして使うなら、代わりに
`config.h` に以下を書く:

```c
#define LED_CAPS_LOCK_PIN GP24
#define LED_PIN_ON_STATE 0
```

### SK6812 のタイミング
`config.h` の `WS2812_TIMING` / `WS2812_T0H` / `WS2812_T1H`。QMK の既定値は
WS2812B 用で、そこから導かれる `T1L` が 350ns になる。SK6812MINI-E の規格は
450〜750ns なので下限割れで、1 のビットの後の低レベル期間が足りず次のビットを
誤読する。0 のはずのチャンネルに光が乗るため**色が淡くなる**。

Pro Micro では表面化しなかった。AVR の bitbang は CPU サイクル単位＋ループの
オーバーヘッドで、実際の低レベル期間が公称値より長く出ていたため。RP2040 の PIO は
指定どおりの波形を正確に出すので規格違反が顕在化した。

指定値はすべて 50ns の倍数（PIO の分解能）にしてあるので、丸め誤差なく再現される。

### 電流バジェット
`config.h` の `RGB_CURRENT_BUDGET` と `RGB_WEIGHT_R/G/B`。

USB バスパワーで、マスタ側が TRRS 経由でスレーブ側にも給電している。同時に多くの
チャンネルを点灯させると供給能力を超えて VDD が落ち、**MCU がブラウンアウトして
再起動を繰り返す**（Liatris での実測: 全点灯・純白で val=85 を超えると発生）。

SK6812MINI-E は各チャンネルを定電流で駆動するので、VDD から引く電流は色によらず
チャンネルのデューティ比の総和に比例する。よって重みは3色とも等しく、実質
`Σ(r+g+b)` の上限になっている。

> Pro Micro 時代は色ごとに違う重み (R 96 / G 300 / B 256) を使っていた。当時の
> 故障モードは**ちらつき**で、これは電圧降下と各 LED の順方向電圧の余裕がぶつかる
> 現象なので、青と緑が先に音を上げ赤は耐えるという色依存があった。Liatris では
> どの明るさでもちらつかず、代わりに固いブラウンアウトが起きるので、色の項は
> 何もモデル化していない。

> **単位に注意。** QMK の val は 0〜255 だが、**Remap の明るさスライダは 0〜100**
> で表示される。Remap の「85」は生の値では 217。一度この読み違いでバジェットを
> 2.55倍低く設定してしまい、白が Remap 表示 29% 相当までしか出ない状態になった。
> 測定値は必ず**生の値**（OLED 表示）で扱うこと。

**再測定の手順:**

1. `config.h` の `RGB_CURRENT_BUDGET` をコメントアウトしてビルド・書き込み
2. 彩度0（純白）・全LED点灯（最悪ケース）にして、明るさを上げていき再起動が
   始まる直前の**生の値**を記録
3. `BUDGET = 30 * (その値 * 3) * 0.95` を設定（30 は片側の LED 数。リミッタは
   `RGBLED_SPLIT` により左右それぞれ 30 個ずつに対して掛かる）

実測値: 純白・全点灯で生 val=217（Remap 表示 85%）まで安定。

### VIA レイヤ数
`DYNAMIC_KEYMAP_LAYER_COUNT 6`（QMK 既定の4から拡張）。
**変更すると EEPROM のレイアウトがずれるので、書き込み後に EEPROM リセットが必要。**

## 保留: 解像度2倍 (128x64) OLED

1.09インチ 128x64 / **SSD1312** のモジュールを入手したが、ピンヘッダの規格が
合わず未着手。合うモジュールが手に入ったら再開する。調べた結果を残しておく。

- QMK は **SSD1312 に対応していない**（0.22.14 にも現行 master にもなく、
  ドキュメントも SSD1306 / SH1106 / SH1107 のみ）
- ただし SSD1312 は SSD1306 と同じ Solomon Systech 系列で、QMK が送る初期化
  シーケンス（クロック→マルチプレクス比→チャージポンプ→メモリモード→COMピン
  →コントラスト→表示ON）はほぼ共通。**まず `#define OLED_DISPLAY_128X64` だけで
  試す価値がある**（`OLED_IC` は既定の SSD1306 のまま）
- SH1106 用の設定は使わないこと。ページアドレッシングのみの別系統で合わない
- ビルドは通ることを確認済み。`oled_buffer` が 512 → 1024 バイトになる
- Keyball 側の描画コードは座標を直書きせず 21文字（128px ÷ フォント幅6px）での
  自動折り返しに任せているだけなので、**128x64 でもそのまま動く**。現在の表示は
  ちょうど4行なので、下半分の4行が空く
- 映らない場合の切り分け: 全く光らない→チャージポンプ or I2Cアドレス(0x3C/0x3D)、
  真っ白→メモリアドレッシングモード、横にずれる→`OLED_COLUMN_OFFSET`、
  上下反転→COMスキャン方向、上半分だけ→`OLED_COM_PINS` (ALT ↔ SEQ)
- 最終手段として `OLED_DISPLAY_CUSTOM` で初期化シーケンスを自前で組める
