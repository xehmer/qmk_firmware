/* Copyright 2023 Andreas Fuchs <asf@boinkor.net>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "config.h"
#include QMK_KEYBOARD_H

extern bool is_drag_scroll;
uint16_t    overall_scroll_distance = 0;

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT(KC_BTN1, KC_BTN3, KC_BTN2, DRAG_SCROLL, MO(2)),
    [1] = LAYOUT(KC_BTN4, G(KC_W), KC_BTN5, _______, MO(3)),
    [2] = LAYOUT(KC_BRIGHTNESS_DOWN, KC_MEDIA_PLAY_PAUSE, KC_BRIGHTNESS_UP, MO(4), _______),
    [3] = LAYOUT(_______, _______, _______, _______, _______),
    [4] = LAYOUT(_______, _______, _______, _______, _______),
};

#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [0] = {ENCODER_CCW_CW(MS_WHLD, MS_WHLU)},
    [1] = {ENCODER_CCW_CW(LCS(KC_TAB), LCTL(KC_TAB))},
    [2] = {ENCODER_CCW_CW(KC_VOLD, KC_VOLU)},
    [3] = {ENCODER_CCW_CW(_______, _______)},
    [4] = {ENCODER_CCW_CW(_______, _______)},
};
#endif

bool wasAdhocScroll(uint16_t keycode, keyrecord_t *record) {
    /* If we scrolled further than that distance while holding the button pressed, it's an adhoc scroll & we clear the
     * drag-scroll state. */
    return keycode == DRAG_SCROLL && !record->event.pressed &&
           overall_scroll_distance > BOINKOR_PLOOPY_RELEASE_ADHOC_DISTANCE;
}
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (keycode == DRAG_SCROLL) {
        if (record->event.pressed)
            layer_on(1);
        else
            layer_off(1);
    }

    if (is_drag_scroll && (keycode != DRAG_SCROLL || wasAdhocScroll(keycode, record))) {
        is_drag_scroll = false;
        if (!layer_state_is(1)) {
            return false;
        }
    }

    return true;
}

report_mouse_t pointing_device_task_user(report_mouse_t mouse_report) {
    if (is_drag_scroll) {
        overall_scroll_distance += abs(mouse_report.v) + abs(mouse_report.h);
    } else {
        overall_scroll_distance = 0;
    }
    return mouse_report;
}

// copied from encoder.c:35+
void sendEncoderEvent(uint8_t index, bool clockwise) {
    // The delays below cater for Windows and its wonderful requirements.
    action_exec(clockwise ? MAKE_ENCODER_CW_EVENT(index, true) : MAKE_ENCODER_CCW_EVENT(index, true));
#if ENCODER_MAP_KEY_DELAY > 0
    wait_ms(ENCODER_MAP_KEY_DELAY);
#endif // ENCODER_MAP_KEY_DELAY > 0

    action_exec(clockwise ? MAKE_ENCODER_CW_EVENT(index, false) : MAKE_ENCODER_CCW_EVENT(index, false));
#if ENCODER_MAP_KEY_DELAY > 0
    wait_ms(ENCODER_MAP_KEY_DELAY);
#endif
}

uint     swallowed_up          = 0;
uint     swallowed_down        = 0;
uint16_t lastScrollHandledByUs = 0; // can't use Ploopy default 'lastScroll' due to queue handling asynchronicity

bool encoder_update_user(uint8_t index, bool clockwise) {
    // reset scroll accumulation after a duration with no scroll event
    if (timer_elapsed(lastScrollHandledByUs) > SCROLL_ACCUMULATOR_RESET_MS) {
        swallowed_up   = 0;
        swallowed_down = 0;
    }

    if (clockwise) {
        swallowed_up += 1;
        swallowed_down = 0;
        if (swallowed_up >= SCROLL_THRESHOLD) {
            sendEncoderEvent(index, clockwise);
        }
    } else {
        swallowed_down += 1;
        swallowed_up = 0;
        if (swallowed_down >= SCROLL_THRESHOLD) {
            sendEncoderEvent(index, clockwise);
        }
    }

    lastScrollHandledByUs = timer_read();
    return false;
}
