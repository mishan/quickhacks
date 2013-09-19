/*
 * Copyright (C) 2005-2013 Misha Nasledov <misha@nasledov.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */
#include <stdlib.h>
#include "music-util.h"

u_int8_t natMinorTest[] = {0, 2, 3, 5, 7, 8, 10, 12};
u_int8_t natMinorTestLen = sizeof(natMinorTest);

bool MusicUtil::isNaturalMinor (u_int8_t note, u_int8_t keynote)
{
    note %= 12; /* ignore octave */

    for (int i = 0; i < natMinorTestLen; i++)
    {
        if (note == natMinorTest[i])
            return true;
    }

    return false;
}
