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
#ifndef ALSA_MIDI_H
#define ALSA_MIDI_H

using namespace std;
#include <alsa/asoundlib.h>

#define ALSA_DEFAULT_MIDI_DEVICE "default"

class AlsaMidi {
public:
	AlsaMidi (const char *name, const char *dest);
	~AlsaMidi (void);

	void sendNote (int note, bool state);
	void flush (void);
protected:
	string name_;
	string device_;


	bool open_seq (const char *destPort);
	bool seq_opened_;

	snd_seq_t *seq_handle_;
	snd_seq_addr_t dest_;
};

#endif /* ALSA_MIDI_H */
