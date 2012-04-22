/* Copyright (C) 2005 Misha Nasledov */
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
