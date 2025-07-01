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
uint16_t overall_scroll_distance = 0;


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT( KC_BTN1, KC_BTN3, KC_BTN2, MO(1), DRAG_SCROLL ),
    [1] = LAYOUT( KC_BTN4, _______, KC_BTN5, _______, _______ ),
    [2] = LAYOUT( _______, _______, _______, _______, _______ ),
    [3] = LAYOUT( _______, _______, _______, _______, _______ ),
    [4] = LAYOUT( _______, _______, _______, _______, _______ ),
    [5] = LAYOUT( _______, _______, _______, _______, _______ ),
    [6] = LAYOUT( _______, _______, _______, _______, _______ ),
    [7] = LAYOUT( _______, _______, _______, _______, _______ )
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  /* Specially handle l/r clicks on drag-scrolling (stop scrolling) */
  if (is_drag_scroll && (keycode == KC_BTN1 || keycode == KC_BTN2)) {
    is_drag_scroll = false;
    return false;
  }

  if (is_drag_scroll && keycode == DRAG_SCROLL && !record->event.pressed &&
      overall_scroll_distance > BOINKOR_PLOOPY_RELEASE_ADHOC_DISTANCE) {
    /* If we scrolled further than that distance while holding the button pressed, it's an adhoc scroll & we clear the drag-scroll state. */
    is_drag_scroll = false;
    return false;
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

// int scroll_accumulator = 0;
// bool encoder_update_user(uint8_t index, bool clockwise) {
//     // reset scroll accumulation after a duration with no scroll event
//     if (timer_elapsed(lastScroll) > SCROLL_ACCUMULATOR_RESET_MS) {
//         scroll_accumulator = 0;
//     }

//     scroll_accumulator += clockwise ? 1 : -1;
//     if (abs(scroll_accumulator) >= SCROLL_THRESHOLD) {
//         bool scroll_up = scroll_accumulator > 0;
//         for (int i = 0; i < SCROLL_MULTIPLIER; i++) {
//             tap_code(scroll_up ? KC_WH_U : KC_WH_D);
//         }
//         scroll_accumulator = 0;
//     }
//     return false;
// }

uint swallowed_up = 0;
uint swallowed_down = 0;
uint16_t lastScrollHandledByUs = 0; // can't use Ploopy default 'lastScroll' due to queue handling asynchronicity

bool encoder_update_user(uint8_t index, bool clockwise) {
    // reset scroll accumulation after a duration with no scroll event
    if (timer_elapsed(lastScrollHandledByUs) > SCROLL_ACCUMULATOR_RESET_MS) {
        swallowed_up = 0;
        swallowed_down = 0;
    }

    if (clockwise) {
        swallowed_up += 1;
        swallowed_down = 0;
        if (swallowed_up >= SCROLL_THRESHOLD) {
            tap_code(KC_WH_U);
        }
    } else {
        swallowed_down += 1;
        swallowed_up = 0;
        if (swallowed_down >= SCROLL_THRESHOLD) {
            tap_code(KC_WH_D);
        }
    }

    lastScrollHandledByUs = timer_read();
    return false;
}
