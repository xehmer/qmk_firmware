/* Copyright 2020 Christopher Courtney, aka Drashna Jael're  (@drashna) <drashna@live.com>
 * Copyright 2019 Sunjun Kim
 * Copyright 2020 Ploopy Corporation
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

#pragma once

#define DYNAMIC_KEYMAP_LAYER_COUNT 4

/* defaults to 600 DPI */
#define PLOOPY_DPI_OPTIONS { 600, 900, 1200, 1600, 2400 }
#define PLOOPY_DPI_DEFAULT 0

#define PLOOPY_DRAGSCROLL_INVERT
#define PLOOPY_DRAGSCROLL_DIVISOR_H 50.0
#define PLOOPY_DRAGSCROLL_DIVISOR_V 30.0

/* Number of points scrolled after which a click-scroll-release counts as
   "active while held". Scroll further than that in any direction & releasing
   the button deactivates drag scrolling.
   This number is pretty small to let us discern between "accidental"
   nudges of the ball when I meant to activate the mode, vs. small
   amount of scroll. */
#define BOINKOR_PLOOPY_RELEASE_ADHOC_DISTANCE 2

#define SCROLL_THRESHOLD 2
#define SCROLL_MULTIPLIER 2
#define SCROLL_ACCUMULATOR_RESET_MS 1000
