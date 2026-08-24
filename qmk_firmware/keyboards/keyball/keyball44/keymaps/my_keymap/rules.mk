# Liatris (RP2040) 前提。Pro Micro には容量が入らないので、変換を既定にしておく。
# Pro Micro 向けにビルドしたい場合はこの行を外し、下の復活させた機能を削ること。
CONVERT_TO = liatris

# keymap.c の rgblight_call_driver は QMK の weak シンボルを上書きしている。
# LTO を有効にすると rgblight.c が自前の weak 定義をインライン展開してしまい、
# エラーも警告も出ないまま上書きが無視される（LEDゾーンと電流制御が丸ごと死ぬ）。
LTO_ENABLE = no

RGBLIGHT_ENABLE = yes
OLED_ENABLE = yes
VIA_ENABLE = yes
COMBO_ENABLE = yes

# --- 以下は Pro Micro 時代に容量のために切っていたもの ---
EXTRAKEY_ENABLE = yes       # 音量・メディアキー、システムキー
NKRO_ENABLE = yes           # Nキーロールオーバー
BOOTMAGIC_ENABLE = yes      # 起動時のキー長押しで EEPROM リセット（復旧手段）
MAGIC_ENABLE = yes          # マジックキーコード（VIA から使える入れ替え系）
SPACE_CADET_ENABLE = yes
GRAVE_ESC_ENABLE = yes

# 以下は意図的に無効のまま:
#   MOUSEKEY_ENABLE  - トラックボールがあるので不要。KC_MS_BTN* は無しでも動く
#   CONSOLE/COMMAND  - デバッグ用。必要になったら有効化する
#   AUDIO/BACKLIGHT/SLEEP_LED - 対応ハードが載っていない
#   POINTING_DEVICE_AUTO_MOUSE_ENABLE - 操作感が変わるため不要との判断
