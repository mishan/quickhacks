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
#ifndef BREAD_NOTE_H
#define BREAD_NOTE_H

#define DEFAULT_DURATION 100
#define DEFAULT_DYNAMIC 64

class BreadNote {
public:
    BreadNote (u_int32_t ontime=0, u_int8_t pitch=0,
               u_int32_t duration=DEFAULT_DURATION, u_int8_t channel=1, 
               u_int8_t dynamic=DEFAULT_DYNAMIC)
        : ontime_(ontime), pitch_(pitch), duration_(duration),
          channel_(channel), dynamic_(dynamic)
        { }
    BreadNote (const BreadNote &copyEvent)
        : ontime_(copyEvent.ontime()), pitch_(copyEvent.pitch()),
          duration_(copyEvent.duration()), channel_(copyEvent.channel()),
          dynamic_(copyEvent.dynamic())
        { }
    ~BreadNote (void) { };

    void randomizePitch (u_int8_t min, u_int8_t max);
    void randomizeTime (u_int32_t startTime, u_int32_t windowSize);
    void randomizeDynamic (u_int8_t min, u_int8_t max);

    /* accessors */
    u_int32_t ontime   (void) const { return ontime_;   }
    u_int8_t  pitch    (void) const { return pitch_;    }
    u_int32_t duration (void) const { return duration_; }
    u_int8_t  channel  (void) const { return channel_;  }
    u_int8_t  dynamic  (void) const { return dynamic_;  }

    BreadNote *mate(const BreadNote *partner, u_int32_t startTime, 
                    u_int32_t windowSize);
private:
    /* data */
    u_int32_t ontime_;
    u_int8_t  pitch_;
    u_int32_t duration_;
    u_int8_t  channel_;
    u_int8_t  dynamic_;
};

#endif /* BREAD_NOTE_H */
