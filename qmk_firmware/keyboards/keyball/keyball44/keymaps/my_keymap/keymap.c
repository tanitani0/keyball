/*
Copyright 2022 @Yowkees
Copyright 2022 MURAOKA Taro (aka KoRoN, @kaoriya)

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include QMK_KEYBOARD_H

#include "quantum.h"

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  // keymap for default (VIA)
  [0] = LAYOUT_universal(
    KC_ESC   , KC_Q     , KC_W     , KC_E     , KC_R     , KC_T     ,                                        KC_Y     , KC_U     , KC_I     , KC_O     , KC_P     , KC_DEL   ,
    KC_TAB   , KC_A     , KC_S     , KC_D     , KC_F     , KC_G     ,                                        KC_H     , KC_J     , KC_K     , KC_L     , KC_SCLN  , S(KC_7)  ,
    KC_LSFT  , KC_Z     , KC_X     , KC_C     , KC_V     , KC_B     ,                                        KC_N     , KC_M     , KC_COMM  , KC_DOT   , KC_SLSH  , KC_INT1  ,
              KC_LALT,KC_LGUI,LCTL_T(KC_LNG2)     ,LT(1,KC_SPC),LT(3,KC_LNG1),                  KC_BSPC,LT(2,KC_ENT), RCTL_T(KC_LNG2),     KC_RALT  , KC_PSCR
  ),

  [1] = LAYOUT_universal(
    SSNP_FRE ,  KC_F1   , KC_F2    , KC_F3   , KC_F4    , KC_F5    ,                                         KC_F6    , KC_F7    , KC_F8    , KC_F9    , KC_F10   , KC_F11   ,
    SSNP_VRT ,  _______ , _______  , KC_UP   , KC_ENT   , KC_DEL   ,                                         KC_PGUP  , KC_BTN1  , KC_UP    , KC_BTN2  , KC_BTN3  , KC_F12   ,
    SSNP_HOR ,  _______ , KC_LEFT  , KC_DOWN , KC_RGHT  , KC_BSPC  ,                                         KC_PGDN  , KC_LEFT  , KC_DOWN  , KC_RGHT  , _______  , _______  ,
                  _______  , _______ , _______  ,         _______  , _______  ,                   _______  , _______  , _______       , _______  , _______
  ),

  [2] = LAYOUT_universal(
    _______  ,S(KC_QUOT), KC_7     , KC_8    , KC_9     , S(KC_8)  ,                                         S(KC_9)  , S(KC_1)  , S(KC_6)  , KC_LBRC  , S(KC_4)  , _______  ,
    _______  ,S(KC_SCLN), KC_4     , KC_5    , KC_6     , KC_RBRC  ,                                         KC_NUHS  , KC_MINS  , S(KC_EQL), S(KC_3)  , KC_QUOT  , S(KC_2)  ,
    _______  ,S(KC_MINS), KC_1     , KC_2    , KC_3     ,S(KC_RBRC),                                        S(KC_NUHS),S(KC_INT1), KC_EQL   ,S(KC_LBRC),S(KC_SLSH),S(KC_INT3),
                  KC_0     , KC_DOT  , _______  ,         _______  , _______  ,                   KC_DEL   , _______  , _______       , _______  , _______
  ),

  [3] = LAYOUT_universal(
    RGB_TOG  , AML_TO   , AML_I50  , AML_D50  , _______  , _______  ,                                        RGB_M_P  , RGB_M_B  , RGB_M_R  , RGB_M_SW , RGB_M_SN , RGB_M_K  ,
    RGB_MOD  , RGB_HUI  , RGB_SAI  , RGB_VAI  , _______  , SCRL_DVI ,                                        RGB_M_X  , RGB_M_G  , RGB_M_T  , RGB_M_TW , _______  , _______  ,
    RGB_RMOD , RGB_HUD  , RGB_SAD  , RGB_VAD  , _______  , SCRL_DVD ,                                        CPI_D1K  , CPI_D100 , CPI_I100 , CPI_I1K  , _______  , KBC_SAVE ,
                  QK_BOOT  , KBC_RST  , _______  ,        _______  , _______  ,                   _______  , _______  , _______       , KBC_RST  , QK_BOOT
  ),

  [4] = LAYOUT_universal(
    _______  , _______  , _______  , _______ , _______  , _______  ,                                         _______  , _______  , _______  , _______  , _______  , _______  ,
    _______  , _______  , _______  , _______ , _______  , _______  ,                                         _______  , _______  , _______  , _______  , _______  , _______  ,
    _______  , _______  , _______  , _______ , _______  , _______  ,                                         _______  , _______  , _______  , _______  , _______  , _______  ,
                  _______  , _______ , _______  ,         _______  , _______  ,                   _______  , _______  , _______       , _______  , _______
  ),

  [5] = LAYOUT_universal(
    _______  , _______  , _______  , _______ , _______  , _______  ,                                         _______  , _______  , _______  , _______  , _______  , _______  ,
    _______  , _______  , _______  , _______ , _______  , _______  ,                                         _______  , _______  , _______  , _______  , _______  , _______  ,
    _______  , _______  , _______  , _______ , _______  , _______  ,                                         _______  , _______  , _______  , _______  , _______  , _______  ,
                  _______  , _______ , _______  ,         _______  , _______  ,                   _______  , _______  , _______       , _______  , _______
  ),
};
// clang-format on

// Combos (fire wherever J/K/L are active, i.e. the base layer):
//   J+K = left click, K+L = right click, J+L = middle click.
enum combos { CMB_LCLICK, CMB_RCLICK, CMB_MCLICK };
const uint16_t PROGMEM combo_jk[] = {KC_J, KC_K, COMBO_END};
const uint16_t PROGMEM combo_kl[] = {KC_K, KC_L, COMBO_END};
const uint16_t PROGMEM combo_jl[] = {KC_J, KC_L, COMBO_END};
combo_t key_combos[] = {
    [CMB_LCLICK] = COMBO(combo_jk, KC_BTN1),
    [CMB_RCLICK] = COMBO(combo_kl, KC_BTN2),
    [CMB_MCLICK] = COMBO(combo_jl, KC_BTN3),
};

layer_state_t layer_state_set_user(layer_state_t state) {
    // Auto enable scroll mode when the highest layer is the scroll layer
    keyball_set_scroll_mode(get_highest_layer(state) == SCROLL_LAYER);
    return state;
}

// --- Instant scroll on ball motion -------------------------------------------
// See config.h for why shortening the tapping term settles a pending tap-hold.
// The keymap itself lives in EEPROM (edited in Remap), so this must not depend
// on which keycode sits on the key: it matches any layer-tap whose hold target
// is SCROLL_LAYER, whatever tap keycode is paired with it.
static uint16_t ball_move_time = 0;
static bool     ball_moved     = false;

report_mouse_t pointing_device_task_user(report_mouse_t r) {
    // Runs on the master, after the driver has applied both halves' motion.
    // r.x/r.y are the post-divisor deltas, so this ignores sensor jitter that
    // was too small to move the cursor. While scrolling they are already 0.
    if (r.x != 0 || r.y != 0) {
        ball_move_time = timer_read();
        ball_moved     = true;
    } else if (ball_moved && timer_elapsed(ball_move_time) > TAPPING_TERM) {
        // Expire the stamp well before the 16-bit timer can wrap past it.
        // Without this, a timestamp older than ~32.7s reads as being in the
        // future and would settle the next tap-hold the instant it is pressed.
        // Nothing can still be pending this long after the motion anyway.
        ball_moved = false;
    }
    return r;
}

uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    if (IS_QK_LAYER_TAP(keycode) && QK_LAYER_TAP_GET_LAYER(keycode) == SCROLL_LAYER) {
        // Signed 16-bit difference, so this asks "did the ball move after this
        // key went down?" and stays correct across timer wraparound. Returning 0
        // expires the term retroactively -> hold on the next tick.
        if (ball_moved && (int16_t)(ball_move_time - record->event.time) > 0) {
            return 0;
        }
    }
    return TAPPING_TERM;
}

// Layers reached by holding settle as soon as a key is nested inside the hold,
// instead of waiting out the whole tapping term. See config.h.
bool get_permissive_hold(uint16_t keycode, keyrecord_t *record) {
    return IS_QK_LAYER_TAP(keycode) && (PERMISSIVE_HOLD_LAYERS & (1 << QK_LAYER_TAP_GET_LAYER(keycode)));
}

#ifdef CONVERT_TO_LIATRIS
// The Liatris carries its own amber power LED on GP24, bright enough and placed
// awkwardly enough to be a nuisance under the board. Its logic is inverted:
// driving the pin HIGH is what turns it off.
//
// Runs on both halves (keyball's keyboard_pre_init_kb calls into here). The LED
// still flashes on at power-up, before any firmware runs -- that is wired into
// the board and cannot be suppressed from here.
//
// To get it back, drop this function. To repurpose it as a Caps Lock indicator
// instead, delete this and put in config.h:
//     #define LED_CAPS_LOCK_PIN GP24
//     #define LED_PIN_ON_STATE 0
// Not GP24: the promicro converter's _pin_defs.h shadows the RP2040 vendor one
// and only defines the Pro Micro names, so the GP* aliases are not visible here.
// The vendor header spells GP24 as 24U, so this is the same value.
#    define LIATRIS_POWER_LED_PIN 24U

void keyboard_pre_init_user(void) {
    setPinOutput(LIATRIS_POWER_LED_PIN);
    writePinHigh(LIATRIS_POWER_LED_PIN);
}
#endif

#ifdef OLED_ENABLE

#    include "lib/oledkit/oledkit.h"

void oledkit_render_info_user(void) {
    keyball_oled_render_keyinfo();
    keyball_oled_render_ballinfo();
    keyball_oled_render_layerinfo();
}
#endif

#ifdef RGBLIGHT_ENABLE
#    include "sync_timer.h"
#    include "transactions.h"

// --- Coordinate-based effects ------------------------------------------------
// Each LED is lit from its measured physical coordinate (ledの1d座標.csv), so
// gaps / reversals / warps in the chain order are all reflected faithfully.
// Three slots are hijacked:
//   SNAKE          -> travelling triangular bump   (moving, sync_timer phase)
//   RAINBOW_SWIRL  -> scrolling coordinate rainbow (moving, sync_timer phase)
//   STATIC_GRADIENT-> still coordinate rainbow     (no time term)
// Moving ones derive their phase from sync_timer so both halves stay in step.
//
// Per-LED coordinate, scaled coord*16 (0..224 == 0..14). Index 59 is unused.
static const uint8_t PROGMEM led_coord[RGBLED_NUM] = {
    /* left front  0..19 */  80,80,80,64,64,64,48,48,48,48,32,32,32,32,16,16,16,0,0,0,
    /* left back  20..29 */  72,40,8,0,0,32,48,67,85,98,
    /* right back 30..39 */  126,136,160,176,200,224,224,216,184,168,
    /* right front40..58 */  224,224,224,208,208,208,208,192,192,192,176,176,176,160,160,160,144,144,144,
    /* unused        59  */  255,
};

#define FX_AXIS        225 // coordinate range is 0..224
#define FX_CENTRE      112 // midpoint of the axis, and the mirror line
#define FX_WIDTH       40  // triangular bump half-width (~2.5 coord units)
#define FX_SPEED_SHIFT 4   // larger = slower
// Render interval for moving effects. This was 50ms (20fps) on the Pro Micro:
// the AVR ws2812 driver bit-banged the line with interrupts disabled for the
// whole ~0.9ms frame, which corrupted the timing-critical soft serial link
// between the halves. On RP2040 the PIO clocks the data out over DMA, so the
// CPU is never blocked and the rate is free.
#define FX_FRAME_MS    16   // ~60fps

// LED zone cycle state, read by the masking code in rgblight_call_driver so it
// applies to every lighting mode. 0 all on, 1 upper-underglow off, 2 all
// underglow off, 3 "3x5" key block only, 4 all off. The zone definitions and
// RGB_LED_STATE_COUNT live in config.h.
static uint8_t rgb_led_state = 0;

// --- Frame post-processing: zone mask + current-aware limiter ----------------
// Overrides the weak definition in quantum/rgblight/rgblight.c so it runs for
// every lighting mode, built-in ones included, and so hidden LEDs drop out of
// the current estimate. This lived in the QMK core until the RP2040 swap: with
// LTO on, rgblight.c inlines its own weak definition and a keymap override is
// silently ignored, and the Pro Micro had no flash to spare for turning LTO
// off. See config.h for the current model and the zone definitions.
void rgblight_call_driver(LED_TYPE *start_led, uint8_t num_leds) {
    uint8_t led_state = rgb_led_state;
    bool    masking   = (led_state != 0);

    static const uint8_t PROGMEM rgb_3x5_mask[] = {RGB_3X5_MASK_BYTES};
#    define RGB_IS_3X5(gi) ((pgm_read_byte(&rgb_3x5_mask[(gi) >> 3]) >> ((gi) & 7)) & 1)
    // Chain index of buffer slot idx. With RGBLIGHT_SPLIT each half renders only
    // its own clipped span, so the zone tests need the global index.
#    define RGB_GI(idx) ((uint8_t)(rgblight_ranges.clipping_start_pos + (idx)))
#    define RGB_HIDDEN(idx) (led_state == 4 ? true : \
                             led_state == 3 ? !RGB_IS_3X5(RGB_GI(idx)) : \
                             led_state == 2 ? (RGB_GI(idx) >= RGB_BACK_FIRST && RGB_GI(idx) <= RGB_BACK_LAST) : \
                             led_state == 1 ? RGB_IS_UPPER_UG(RGB_GI(idx)) : false)

#    ifdef RGB_CURRENT_BUDGET
    uint32_t stress = 0;
    for (uint8_t i = 0; i < num_leds; i++) {
        if (masking && RGB_HIDDEN(i)) continue; // hidden LEDs draw no current
        stress += (uint32_t)start_led[i].r * RGB_WEIGHT_R + (uint32_t)start_led[i].g * RGB_WEIGHT_G + (uint32_t)start_led[i].b * RGB_WEIGHT_B;
    }
    stress >>= 8; // back to byte-equivalent units
    uint16_t scale = 256; // 1.0 in 0..256 fixed point
    if (stress > RGB_CURRENT_BUDGET) {
        scale = (uint32_t)RGB_CURRENT_BUDGET * 256 / stress;
    }
#    else
    const uint16_t scale = 256;
#    endif

    if (scale < 256 || masking) {
        LED_TYPE buf[RGBLED_NUM];
        for (uint8_t i = 0; i < num_leds; i++) {
            if (masking && RGB_HIDDEN(i)) {
                buf[i].r = 0;
                buf[i].g = 0;
                buf[i].b = 0;
                continue;
            }
            buf[i]   = start_led[i];
            buf[i].r = ((uint16_t)start_led[i].r * scale) >> 8;
            buf[i].g = ((uint16_t)start_led[i].g * scale) >> 8;
            buf[i].b = ((uint16_t)start_led[i].b * scale) >> 8;
        }
        ws2812_setleds(buf, num_leds);
        return;
    }
    ws2812_setleds(start_led, num_leds);
#    undef RGB_HIDDEN
#    undef RGB_GI
#    undef RGB_IS_3X5
}

// Custom keycode to cycle the LED zones. Assign 0x7E40 in Remap via ANY.
enum custom_keycodes {
    LED_MODE = KEYBALL_SAFE_RANGE,
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (keycode == LED_MODE) {
        if (record->event.pressed) {
            rgb_led_state = (rgb_led_state + 1) % RGB_LED_STATE_COUNT;
            eeconfig_update_user(rgb_led_state); // persist across power cycles
            rgblight_set();                      // repaint immediately
        }
        return false;
    }
    return true;
}

static void back_sync_handler(uint8_t in_len, const void *in, uint8_t out_len, void *out) {
    if (in_len >= 1) rgb_led_state = *(const uint8_t *)in;
}

void keyboard_post_init_user(void) {
    // RGB mode/color are restored from EEPROM by rgblight_init (Remap saves them),
    // so we no longer force a boot mode. Restore the LED-zone state from the user
    // EEPROM block; guard the uninitialized (0xFFFFFFFF) value to "all on".
    uint32_t u    = eeconfig_read_user();
    rgb_led_state = (u == 0xFFFFFFFF) ? 0 : (uint8_t)(u & 0x07);
    // 3bit 幅なので、状態数を減らしたときに範囲外の値が残っていることがある
    if (rgb_led_state >= RGB_LED_STATE_COUNT) rgb_led_state = 0;
    transaction_register_rpc(USER_SYNC_BACK, back_sync_handler);
}

// Sub-mode variations, mirroring the built-in effects' own tables so each
// numbered sub-mode behaves like stock QMK, but computed from physical
// coordinates and the shared sync_timer phase.
//   SNAKE / SWIRL   : delta/2 picks a speed, odd delta reverses direction
//                     (built-in intervals are {100,50,20} ms per step)
//   STATIC_GRADIENT : delta/2 picks how much of the hue circle to span
//                     (built-in ranges {255,170,127,85,64})
static const uint8_t PROGMEM fx_speed_shift[3]    = {6, 5, 4};             // slow -> fast
static const uint8_t PROGMEM fx_gradient_range[5] = {255, 170, 127, 85, 64};
// Hue span for the mirrored gradient: full circle, half, subtle.
static const uint8_t PROGMEM fx_mirror_range[3]   = {255, 127, 64};

// kind: 1 = travelling bump (SNAKE), 2 = scrolling rainbow (SWIRL),
//       3 = still rainbow (STATIC_GRADIENT), 4 = mirrored still rainbow
//       (KNIGHT). delta = sub-mode offset.
static void fx_render(uint8_t kind, uint8_t delta) {
    uint8_t sat  = rgblight_get_sat();
    uint8_t maxv = rgblight_get_val();
    if (kind == 4) {
        // Mirrored still rainbow: hue follows the distance from the centre of
        // the board rather than the raw coordinate, so the two halves are each
        // other's mirror image. The measured coordinates make this exact --
        // the left spans 0..98 and the right 126..224, both 112 from the centre
        // at 112, and the two inner edges sit 14 either side of it.
        //
        // Sub-mode picks how much of the hue circle is spanned. As with the
        // gradient on the STATIC_GRADIENT slot the hue currently set is the
        // starting point, but here the gradient starts at the mirror line: the
        // centre of the board carries that hue and the sweep runs outwards to
        // hue+range at both outer edges. To start from the edges instead and
        // converge on the centre, use (FX_CENTRE - d) in place of d below.
        uint8_t range  = pgm_read_byte(&fx_mirror_range[delta % 3]);
        uint8_t offset = rgblight_get_hue();
        for (uint8_t i = 0; i < RGBLED_NUM; i++) {
            uint8_t c = pgm_read_byte(&led_coord[i]);
            uint8_t d = (c > FX_CENTRE) ? (c - FX_CENTRE) : (FX_CENTRE - c); // 0..112
            uint8_t hue = (uint8_t)(((uint16_t)d * range) / FX_CENTRE) + offset;
            sethsv(hue, sat, maxv, &led[i]);
        }
    } else if (kind == 3) {
        // Still rainbow: the sub-mode narrows the span of the hue circle mapped
        // across the board, from a full rainbow down to a subtle gradient.
        // The built-in picks the range with delta/2, which makes each pair of
        // sub-modes identical; we keep that range table but give the odd one of
        // each pair a half-turn starting phase, so all ten look different.
        uint8_t range  = pgm_read_byte(&fx_gradient_range[(delta / 2) % 5]);
        uint8_t offset = rgblight_get_hue() + ((delta & 1) ? 128 : 0);
        for (uint8_t i = 0; i < RGBLED_NUM; i++) {
            uint8_t c   = pgm_read_byte(&led_coord[i]);
            uint8_t hue = (uint8_t)(((uint16_t)c * range) / 224) + offset;
            sethsv(hue, sat, maxv, &led[i]);
        }
    } else {
        uint8_t shift = pgm_read_byte(&fx_speed_shift[(delta / 2) % 3]);
        bool    rev   = (delta & 1);
        if (kind == 1) {
            // Triangular bump travelling along the coordinate axis. Its center
            // runs from just off one end to just off the other, so at the wrap
            // every LED is dark -> seamless restart.
            uint8_t        hue    = rgblight_get_hue();
            const uint16_t period = FX_AXIS + 2 * FX_WIDTH;
            uint16_t       t      = (uint16_t)((sync_timer_read32() >> shift) % period);
            if (rev) t = period - 1 - t;
            int16_t center = (int16_t)t - FX_WIDTH;
            for (uint8_t i = 0; i < RGBLED_NUM; i++) {
                int16_t d = (int16_t)pgm_read_byte(&led_coord[i]) - center;
                if (d < 0) d = -d;
                uint8_t v = (d < FX_WIDTH) ? (uint8_t)(((uint16_t)(FX_WIDTH - d) * maxv) / FX_WIDTH) : 0;
                sethsv(hue, sat, v, &led[i]);
            }
        } else {
            // Rainbow scrolling along the coordinate axis.
            uint8_t offset = (uint8_t)(sync_timer_read32() >> shift);
            if (rev) offset = (uint8_t)-offset;
            for (uint8_t i = 0; i < RGBLED_NUM; i++) {
                uint8_t c   = pgm_read_byte(&led_coord[i]);
                uint8_t hue = (uint8_t)(((uint16_t)c * 255) / 224) + offset;
                sethsv(hue, sat, maxv, &led[i]);
            }
        }
    }
    rgblight_set();
}

void housekeeping_task_user(void) {
    // Push the zone state to the slave: keys are only processed on the master,
    // so without this only the master half would follow LED_MODE.
    //
    // Reasserted on a slow heartbeat, not just when it changes. A send that
    // "succeeds" is not proof the slave took it -- transactions.c skips the
    // callback when the slave has not registered its handler yet and still
    // reports success, so a send that races the slave's keyboard_post_init_user
    // is silently dropped. The same hole swallows the state whenever the slave
    // restarts on its own (split watchdog, a knocked cable). Sending once left
    // that half stuck on its own boot state until LED_MODE was pressed again.
    if (is_keyboard_master()) {
        static uint8_t  sent     = 0xff;
        static uint16_t last_try = 0;
        uint16_t        interval = (sent != rgb_led_state) ? 50 : 1000;
        if (timer_elapsed(last_try) > interval) {
            last_try      = timer_read();
            uint8_t state = rgb_led_state;
            if (transaction_rpc_send(USER_SYNC_BACK, sizeof(state), &state)) {
                sent = state;
            }
        }
    }
    uint8_t mode = rgblight_get_mode();
    uint8_t hue = rgblight_get_hue(), sat = rgblight_get_sat(), val = rgblight_get_val();
    uint8_t kind = 0, delta = 0;
    if (mode >= RGBLIGHT_MODE_SNAKE && mode <= RGBLIGHT_MODE_SNAKE + 5) {
        kind  = 1; // coordinate bump flow
        delta = mode - RGBLIGHT_MODE_SNAKE;
    } else if (mode >= RGBLIGHT_MODE_RAINBOW_SWIRL && mode <= RGBLIGHT_MODE_RAINBOW_SWIRL + 5) {
        kind  = 2; // coordinate rainbow flow
        delta = mode - RGBLIGHT_MODE_RAINBOW_SWIRL;
    } else if (mode >= RGBLIGHT_MODE_STATIC_GRADIENT && mode <= RGBLIGHT_MODE_STATIC_GRADIENT + 9) {
        kind  = 3; // coordinate still rainbow
        delta = mode - RGBLIGHT_MODE_STATIC_GRADIENT;
    } else if (mode >= RGBLIGHT_MODE_KNIGHT && mode <= RGBLIGHT_MODE_KNIGHT + 2) {
        // KNIGHT's own scanning bar is redundant with the coordinate bump on
        // the SNAKE slot, so its three sub-modes carry the mirrored gradient.
        // RGBLIGHT_EFFECT_KNIGHT has to stay defined in config.h even though
        // nothing renders it now: dropping it renumbers every mode above it.
        kind  = 4; // mirrored coordinate still rainbow
        delta = mode - RGBLIGHT_MODE_KNIGHT;
    }

    // Repaint on any change (mode / color / brightness / zone), tracked on BOTH
    // halves so the slave reflects synced mode changes and the underglow mask.
    static uint8_t l_mode = 0xff, l_hue = 0xff, l_sat = 0xff, l_val = 0xff, l_state = 0xff;
    bool           changed = (mode != l_mode || hue != l_hue || sat != l_sat || val != l_val || rgb_led_state != l_state);
    if (changed) {
        l_mode = mode; l_hue = hue; l_sat = sat; l_val = val; l_state = rgb_led_state;
    }

    if (kind == 0) {
        // Built-in effect: rgblight renders it; repaint on change for the mask.
        if (changed) rgblight_set();
        return;
    }

    // Hijacked slot: stop the built-in animation so it does not interleave with
    // our frames (only needed when entering the mode).
    if (changed) rgblight_timer_disable();

    if (kind == 3 || kind == 4) {
        // Still image: render on change plus a slow heartbeat to reassert.
        static uint16_t lt = 0;
        if (changed || timer_elapsed(lt) > 500) {
            lt = timer_read();
            fx_render(kind, delta);
        }
        return;
    }

    // Moving flows: render every frame, and immediately on change.
    static uint16_t last = 0;
    if (changed || timer_elapsed(last) >= FX_FRAME_MS) {
        last = timer_read();
        fx_render(kind, delta);
    }
}
#endif
