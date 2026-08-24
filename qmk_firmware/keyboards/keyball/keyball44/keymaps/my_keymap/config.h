/*
This is the c configuration file for the keymap

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

#pragma once

#ifdef RGBLIGHT_ENABLE
// Built-in effects are available via Remap. The custom coordinate "flow" (see
// keymap.c) renders only while the SOLID/STATIC_LIGHT mode is selected, so:
//   Remap "Solid Color" -> coordinate flow ;  any other mode -> that built-in
// effect. (Built-in effects run over chain order, so they are not spatially
// continuous like the flow; the flow is the coordinate-faithful one.)
#    define RGBLIGHT_EFFECT_BREATHING
#    define RGBLIGHT_EFFECT_RAINBOW_MOOD
#    define RGBLIGHT_EFFECT_RAINBOW_SWIRL
#    define RGBLIGHT_EFFECT_SNAKE
#    define RGBLIGHT_EFFECT_KNIGHT
#    define RGBLIGHT_EFFECT_CHRISTMAS
#    define RGBLIGHT_EFFECT_STATIC_GRADIENT
#    define RGBLIGHT_EFFECT_RGB_TEST
#    define RGBLIGHT_EFFECT_ALTERNATING
#    define RGBLIGHT_EFFECT_TWINKLE
#endif

#define TAP_CODE_DELAY 5

// --- SK6812MINI-E timing -----------------------------------------------------
// QMK's defaults are WS2812B's (1250ns period, T1H 900 -> T1L 350). SK6812MINI-E
// wants 600ns +/-150 for both halves of a 1 bit, so the stock T1L of 350ns is
// below its 450ns minimum: the low period after a 1 bit is too short and the
// following bit can be sampled wrong, lighting channels that were sent as 0 and
// washing the colors out.
//
// This did not show up on the Pro Micro because the AVR driver bit-banged the
// line in whole CPU cycles with loop overhead, so the real low periods came out
// longer than the nominal figure. The RP2040 PIO emits exactly what is asked
// for, which is what exposed the violation. Every value below is a multiple of
// 50ns, the PIO's granularity, so they are reproduced exactly.
#define WS2812_TIMING 1200 // bit period
#define WS2812_T0H    300  // -> T0L 900
#define WS2812_T1H    600  // -> T1L 600

// The frame-wide current limiter below is the safety mechanism, so let single
// colors reach full brightness instead of being flattened by the keyboard's
// blanket per-channel cap of 150.
#undef RGBLIGHT_LIMIT_VAL
#define RGBLIGHT_LIMIT_VAL 255

// --- Instant scroll on ball motion -------------------------------------------
// The scroll layer is reached through the hold half of a tap-hold key, so it
// normally takes TAPPING_TERM (200ms) of holding before scrolling starts --
// long enough to feel broken when you just want to flick the ball.
//
// QMK re-evaluates WITHIN_TAPPING_TERM on every scan (action_exec runs a tick
// event from keyboard_task), and that macro calls get_tapping_term() each time.
// So shortening the term *while the decision is still pending* retroactively
// expires it: the next tick takes the "after TAPPING_TERM" path in
// action_tapping.c with tap.count == 0, which is exactly the hold branch.
// keymap.c uses this to settle the key the moment the ball moves.
//
// Scoped to tap-hold keys targeting SCROLL_LAYER. Applying it to every layer-tap
// would mean brushing the ball while resting a thumb on space silently shifts
// layers.
#define TAPPING_TERM_PER_KEY
#define SCROLL_LAYER 3

// Layers 1 and 2 are also reached by holding, and waiting out the full tapping
// term to get there feels sluggish. Permissive hold settles them as soon as
// another key is pressed *and released* inside the term, so nesting a key под
// the layer key switches immediately while a fast roll off the thumb still
// types the tap keycode. HOLD_ON_OTHER_KEY_PRESS would decide at the press
// instead, which is snappier but turns "space, then the next letter" into a
// layered keypress whenever typing outruns the release.
//
// Per-key rather than global so mod-taps keep their timing: rolling off the
// 英数/かな thumb keys must stay a tap, not Ctrl+letter.
#define PERMISSIVE_HOLD_PER_KEY
#define PERMISSIVE_HOLD_LAYERS ((1 << 1) | (1 << 2))

// Auto Mouse Layer (AML) removed by request; the freed flash is used for the
// full set of RGB effects above.

// Increase VIA/Remap layer count from the QMK default of 4 to 6.
// NOTE: changing this shifts the EEPROM layout, so an EEPROM reset
// (Remap's "Clear EEPROM" / bootmagic) is required after flashing.
#define DYNAMIC_KEYMAP_LAYER_COUNT 6

// --- Current-aware RGB brightness limiter ------------------------------------
// The Keyball is USB bus powered and the master half feeds the slave over TRRS.
// Lighting many channels at once draws more than the supply can hold up, VDD
// sags, and the MCU browns out -- measured on Liatris as a reboot loop at pure
// white above val=85 with every LED on. RGBLIGHT_LIMIT_VAL only caps each
// channel, so it would dim single colors just as hard while doing nothing about
// the total. Instead the whole frame is scaled down, and only when its estimated
// current exceeds a budget; low-load scenes pass through untouched.
//
// The scaling lives in rgblight_call_driver in keymap.c, which overrides the
// weak definition in quantum/rgblight/rgblight.c. It is gated on
// RGB_CURRENT_BUDGET, so undefining that disables the limiter outright (useful
// for re-measuring).
//
// Model: SK6812MINI-E drives each channel from its own constant-current sink, so
// the current pulled from VDD is proportional to the sum of the channel duty
// cycles regardless of colour. Hence stress = Σ(r*Wr + g*Wg + b*Wb)/256 with all
// three weights equal, and the budget is a plain cap on Σ(r+g+b).
//
// This replaced an earlier colour-weighted model (R 96 / G 300 / B 256) that was
// calibrated on the Pro Micro against *flicker*, which is a different mechanism:
// supply droop meeting each LED's forward-voltage headroom, so blue and green
// gave out first while red rode it out. On Liatris no flicker appears at any
// brightness -- the failure is a hard brownout instead -- so the colour term no
// longer models anything real.
//
// Calibration: the limiter is applied per half, over that half's 30 LEDs (see
// RGBLED_SPLIT). White at val=85 measured 30 * (85*3) = 7650 as the last stable
// point, and the budget keeps 10% below it. To re-measure: comment out
// RGB_CURRENT_BUDGET, flash, raise brightness on pure white until the board
// resets, then set the budget to 30 * (last_good_val * 3) * 0.9.
#define RGB_CURRENT_BUDGET 6885 // white capped at val~76 (10% under the measured 85)
#define RGB_WEIGHT_R       256  // equal weights: current does not depend on colour
#define RGB_WEIGHT_G       256
#define RGB_WEIGHT_B       256

// (LED_MAP removed: the custom coordinate flow in keymap.c uses the raw led[]
// indexed by physical chain position, so no logical remap is wanted here.)

// --- LED zone cycle ----------------------------------------------------------
// From ledの1d座標.csv:
//   key LEDs (front)      : chain 0..19, 40..58
//   underglow (back)      : chain 20..39
//   upper underglow subset: chain 20..22 (left) and 37..39 (right)
// The LED_MODE keycode (0x7E40, assign in Remap via ANY) cycles:
//   0 all on -> 1 upper underglow off -> 2 all underglow off
//     -> 3 "3x5" key block only -> 4 all off -> 0
// Masking is done in rgblight_call_driver, so it applies to every mode and the
// hidden LEDs also drop out of the current budget.
#define RGB_LED_STATE_COUNT 5
#define RGB_BACK_FIRST 20
#define RGB_BACK_LAST  39
#define RGB_IS_UPPER_UG(gi) (((gi) >= 20 && (gi) <= 22) || ((gi) >= 37 && (gi) <= 39))

// The "3x5" block: the front key LEDs flagged in the 3x5領域か column of
// led-1d-coordinates.csv (30 LEDs: 3 rows x 5 columns on each half).
// A 60-bit set packed into 8 bytes, expanded into a PROGMEM table in
// rgblight_call_driver. Packing it this way rather than as two uint32_t
// constants avoids a runtime 32-bit shift per LED, which costs ~100 bytes of
// flash on AVR -- worth it while the Pro Micro is this close to full.
// To regenerate after editing the CSV, run tools/gen-zone-mask.py here.
#define RGB_3X5_MASK_BYTES 0xFF, 0xDD, 0x01, 0x00, 0x00, 0xB8, 0xFF, 0x07

// Key processing only happens on the master half, so the toggle state has to be
// pushed to the slave or only one side would go dark.
#define SPLIT_TRANSACTION_IDS_USER USER_SYNC_BACK

// Recover the slave half if it locks up: reset a half that stops responding
// rather than leaving it stuck with stale colors.
//
// The original reason is gone -- on the Pro Micro the ws2812 bit-bang driver
// disabled interrupts for ~0.9ms per frame and corrupted the soft-serial link,
// while RP2040 drives the LEDs from PIO/DMA without blocking the CPU. Kept
// anyway: it costs nothing now that flash is not scarce, it only acts when a
// half has already stopped talking, and a split link can wedge for other
// reasons (cable, connector) where an automatic reset is still the right cure.
#define SPLIT_WATCHDOG_ENABLE
