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
#include "main.h"
#include "BreadNote.h"

void BreadNote::randomizePitch (u_int8_t min, u_int8_t max)
{
    pitch_ = min+(rand()%(max-min));
}

void BreadNote::randomizeTime (u_int32_t startTime, u_int32_t windowSize)
{
    duration_ = rand()%windowSize;
    ontime_ = startTime + (windowSize-duration_);
}

void BreadNote::randomizeDynamic (u_int8_t min, u_int8_t max)
{
    dynamic_ = min+(rand()%(max-min));
}

BreadNote *BreadNote::mate (const BreadNote *partner, u_int32_t startTime,
                      u_int32_t windowSize)
{
    /* "flip a coin" for each parameter of the cope event to decide from
       which parent to take the data from */
    BreadNote *child = new BreadNote(rand()%2 ? ontime_+windowSize : 
                                     partner->ontime() + windowSize,
                                     
                                     rand()%2 ? pitch_ : partner->pitch(),
                                     rand()%2 ? duration_ :
                                     partner->duration(),
                                     channel_, /* XXX: only one channel is
                                                  supported, so this is
                                                  irrelevant for now */
                                     rand()%2 ? dynamic_ : partner->dynamic());

    return child;                
}
