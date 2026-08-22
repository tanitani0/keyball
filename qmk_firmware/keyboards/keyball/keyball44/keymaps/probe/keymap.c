/* LED probe firmware for Keyball44.
 *
 * Purpose: light exactly ONE LED at a time, by its raw WS2812 chain index, so
 * you can walk the whole chain and record each index's physical position. Use
 * the recorded order to build RGBLIGHT_LED_MAP and make effects spatially
 * continuous.
 *
 * Split note: RGBLIGHT_SPLIT syncs only the HSV/mode config between halves, not
 * the raw LED buffer. This probe writes the LED buffer directly, so LEDs light
 * ONLY on the USB-connected (master) half. Probe one half, then move the USB
 * cable to the other half and continue.
 *
 * Controls (any key; text input is swallowed while probing):
 *   - press any RIGHT-hand key  -> next index
 *   - press any LEFT-hand key   -> previous index
 * The current index is shown on the OLED of the USB-connected half.
 */

#include QMK_KEYBOARD_H

#ifdef OLED_ENABLE
#    include "lib/oledkit/oledkit.h"
#endif

static uint8_t probe_index = 0;

static void probe_paint(void) {
    rgblight_setrgb_range(0, 0, 0, 0, RGBLED_NUM);    // all LEDs off
    rgblight_setrgb_at(120, 120, 120, probe_index);   // current index -> white
}

void keyboard_post_init_user(void) {
    rgblight_enable_noeeprom();
    rgblight_mode_noeeprom(RGBLIGHT_MODE_STATIC_LIGHT); // no animation to fight us
    rgblight_sethsv_noeeprom(0, 0, 0);                  // start all black
    probe_paint();
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        if (record->event.key.row < 4) {
            // left hand -> previous
            probe_index = (probe_index == 0) ? (uint8_t)(RGBLED_NUM - 1) : (uint8_t)(probe_index - 1);
        } else {
            // right hand -> next
            probe_index = (uint8_t)((probe_index + 1) % RGBLED_NUM);
        }
        probe_paint();
    }
    return false; // swallow every key while probing
}

#ifdef OLED_ENABLE
void oledkit_render_info_user(void) {
    oled_write_P(PSTR("LED PROBE\n"), false);
    oled_write_P(PSTR("index="), false);
    char d[4];
    d[0] = '0' + (probe_index / 10) % 10;
    d[1] = '0' + (probe_index % 10);
    d[2] = '\n';
    d[3] = 0;
    oled_write(d, false);
    oled_write_P(PSTR("R:next L:prev"), false);
}
#endif

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [0] = LAYOUT_universal(
    KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
    KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
    KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
            KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,   KC_NO, KC_NO, KC_NO, KC_NO, KC_NO
  ),
};
// clang-format on
