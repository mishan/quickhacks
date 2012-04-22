/* Copyright (C) 2005-2012 Misha Nasledov <misha@nasledov.com>
 *  All Rights Reserved.
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
