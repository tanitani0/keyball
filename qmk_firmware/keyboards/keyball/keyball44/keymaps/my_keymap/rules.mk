RGBLIGHT_ENABLE = yes

OLED_ENABLE = no

VIA_ENABLE = yes

COMBO_ENABLE = yes

# The frame post-processing in keymap.c overrides a weak symbol in
# quantum/rgblight/rgblight.c. LTO lets that file inline its own weak
# definition, which silently discards the override, so it has to stay off.
LTO_ENABLE = no
