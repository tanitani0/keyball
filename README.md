> **Note:** This is a personal fork of [Yowkees/keyball](https://github.com/Yowkees/keyball) —
> go there for the original project. This copy adds a personal Keyball44 keymap and
> a few small changes around it, listed below. Its notes are in Japanese.
>
> 本家 [Yowkees/keyball](https://github.com/Yowkees/keyball) の個人的なフォークです。
> 本家からの差分は Keyball44 まわりだけで、内容は以下のとおりです。
>
> - Keyball44 用のキーマップ [`my_keymap`](qmk_firmware/keyboards/keyball/keyball44/keymaps/my_keymap/)（新規）
> - LED のチェーン位置を実測するための [`probe`](qmk_firmware/keyboards/keyball/keyball44/keymaps/probe/) キーマップ（新規）
> - `keyball44/config.h`: RP2040 変換時に WS2812 を PIO1 へ退避（分割通信と PIO0 を取り合うため）
> - `lib/keyball/keyball.c`: `BL` を const 変数からマクロへ（静的初期化子で使えるように）
>
> Liatris (RP2040) に載せ替えて使っており、その過程で分かったことを
> [キーマップの README](qmk_firmware/keyboards/keyball/keyball44/keymaps/my_keymap/README.md)
> にまとめています。SK6812 のタイミングが QMK 既定値では規格外になる件、電流制限の
> 較正、LED ゾーンの左右同期の取りこぼしなど。同じ構成で困っている方の参考になれば。

# Keyball Series

![Keyball61](./keyball61/doc/rev1/images/kb61_001.jpg)

Keyball series is keyboard family which have 100% track ball.

Keyboards in the family are:

* Available
    * Keyball39: split + 39 keys + a track ball
    * Keyball44: split + 44 keys + a track ball
    * Keyball61: split + 61 keys + a track ball
* Unavailable
    * Keyball46 (first one!)
    * One47

## Where to Buy

|Keyboard   |Shirogane Lab / 白銀ラボ                                   |Yushakobo / 遊舎工房                       |
|-----------|-------------------------------------------|-----------------------------------------------------------|
|Keyball39  |<https://shiroganelab.com/products/keyball39> |<https://shop.yushakobo.jp/products/5357>  |
|Keyball44  |<https://shiroganelab.com/products/keyball44> |<https://shop.yushakobo.jp/products/8337>  |
|Keyball61  |<https://shiroganelab.com/products/keyball61> |<https://shop.yushakobo.jp/products/5358>  |

## Build Guide

*   Keyball39:
    [English/英語](/keyball39/doc/rev1/buildguide_en.md),
    [日本語/Japanese](./keyball39/doc/rev1/buildguide_jp.md)
*   Keyball44:
    [English/英語](./keyball44/doc/rev1/buildguide_en.md),
    [日本語/Japanese](./keyball44/doc/rev1/buildguide_jp.md)
*   Keyball61:
    [English/英語](./keyball61/doc/rev1/buildguide_en.md),
    [日本語/Japanese](./keyball61/doc/rev1/buildguide_jp.md)

## Firmware

See [document for firmware source code](./qmk_firmware/keyboards/keyball/readme.md).

### Pre-compiled Firmwares

(TO BE DOCUMENTED)
