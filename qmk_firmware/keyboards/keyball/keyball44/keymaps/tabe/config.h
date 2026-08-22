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

// Auto Mouse Layer (AML) removed by request; the freed flash is used for the
// full set of RGB effects above.

// Increase VIA/Remap layer count from the QMK default of 4 to 6.
// NOTE: changing this shifts the EEPROM layout, so an EEPROM reset
// (Remap's "Clear EEPROM" / bootmagic) is required after flashing.
#define DYNAMIC_KEYMAP_LAYER_COUNT 6

// --- Current-aware RGB brightness limiter -----------------------------------
// The Keyball is USB bus powered. Lighting many channels at once (e.g. white on
// every LED) can draw more current than the port/ProMicro polyfuse supplies,
// making VDD sag and the LEDs flicker. RGBLIGHT_LIMIT_VAL only caps per-channel
// brightness, so it also dims single colors. Instead we scale the *whole frame*
// down only when its estimated current exceeds a budget; low-load scenes
// (single color / few LEDs) pass through untouched.
//
// The actual scaling lives in quantum/rgblight/rgblight.c:rgblight_call_driver
// (overriding it from the keymap does not work: rgblight.c inlines its own weak
// definition, so the core function is the only reliable place). That code is
// gated on RGB_CURRENT_BUDGET, so it is active only for this keymap.
//
// Model: flicker is color dependent (SK6812MINI-E, 12mA/ch constant current).
// It is supply droop meeting each LED's forward-voltage headroom: red (low VF)
// tolerates droop, blue/green (high VF) flicker first. So we sum a *weighted*
// stress = Σ(r*Wr + g*Wg + b*Wb)/256 and cap it. Red is weighted light.
// Tuning: lower RGB_CURRENT_BUDGET if flicker remains; raise a color's weight
// to make that color start dimming sooner.
// Calibrated to this board's measured flicker behavior (Remap brightness, single
// color). Weights decouple each color's cap: cap(color) ~= BUDGET / (k * W_color)
// with the mixed weight for white. Current targets:
//   white ~42 (tight; white flickered slightly at 50), blue ~free (fine at 100),
//   green ~92 (removes its slight flicker), red never caps.
// To retune: white cap moves with BUDGET and the *sum* of weights; to loosen a
// single color raise its cap by lowering its weight; to tighten it raise its
// weight. Lower BUDGET tightens everything.
#define RGB_CURRENT_BUDGET 1425 // 5% safety margin (was 1500); weights unchanged
#define RGB_WEIGHT_R       96  // red very tolerant -> light weight, never caps
#define RGB_WEIGHT_G       300 // green: slightly tightened
#define RGB_WEIGHT_B       256 // blue: full weight (kept loose -> ~free to 100)

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
// led-1d-coordinates.csv (33 LEDs: 3x6 on the left, 3x5 on the right).
// A 60-bit set packed into 8 bytes, expanded into a PROGMEM table in
// rgblight_call_driver. Packing it this way rather than as two uint32_t
// constants avoids a runtime 32-bit shift per LED, which costs ~100 bytes of
// flash on AVR -- worth it while the Pro Micro is this close to full.
// To regenerate after editing the CSV, run tools/gen-zone-mask.py here.
#define RGB_3X5_MASK_BYTES 0xFF, 0xDD, 0x0F, 0x00, 0x00, 0xB8, 0xFF, 0x07

// Key processing only happens on the master half, so the toggle state has to be
// pushed to the slave or only one side would go dark.
#define SPLIT_TRANSACTION_IDS_USER USER_SYNC_BACK

// Recover the slave half if it locks up. The ws2812 bitbang driver disables
// interrupts for ~0.9ms per LED frame, which can corrupt the soft-serial split
// link; the watchdog resets a half that stops responding instead of leaving it
// stuck with stale colors.
#define SPLIT_WATCHDOG_ENABLE
