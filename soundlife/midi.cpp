/* Copyright (C) 2005 Misha Nasledov */
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include "midi.h"

#define MIDI_NOTE_MIN 30
#define MIDI_NOTE_MAX 90

AlsaMidi::AlsaMidi (const char *name, const char *dest)
{
	name_ = name;
	device_ = ALSA_DEFAULT_MIDI_DEVICE;
	seq_opened_ = open_seq(dest);
}

AlsaMidi::~AlsaMidi (void)
{
	snd_seq_close(seq_handle_);
}

void AlsaMidi::sendNote (int note, bool state)
{
	snd_seq_event_t ev;

//	note = (note < MIDI_NOTE_MIN ? MIDI_NOTE_MIN : note);
//	note = (note > MIDI_NOTE_MAX ? MIDI_NOTE_MAX : note);

	snd_seq_ev_clear(&ev);

	if (state)
		snd_seq_ev_set_noteon(&ev, 0, note, 127);
	else
		snd_seq_ev_set_noteoff(&ev, 0, note, 127);

	snd_seq_ev_set_subs(&ev);
	snd_seq_ev_set_direct(&ev);

	ev.dest = dest_;

	int err = snd_seq_event_output_direct(seq_handle_, &ev);
	if (err < 0)
		fprintf(stderr, "%s\n",  snd_strerror(err));
}

void AlsaMidi::flush (void)
{
	snd_seq_drain_output(seq_handle_);
}

bool AlsaMidi::open_seq (const char *destPort)
{
	int port_id;

	if (snd_seq_open(&seq_handle_, device_.c_str( ),
					 SND_SEQ_OPEN_DUPLEX, 0) < 0)
	{
		fprintf(stderr, "Cannot open ALSA sequencer\n");
		return false;
	}

	snd_seq_set_client_name(seq_handle_, name_.c_str());

    if ((port_id = snd_seq_create_simple_port(seq_handle_, name_.c_str(),
						SND_SEQ_PORT_CAP_READ|SND_SEQ_PORT_CAP_SUBS_READ,
						SND_SEQ_PORT_TYPE_MIDI_GENERIC)) < 0)
	{
        fprintf(stderr, "Error creating sequencer port.\n");
		return false;
    }
	else
	{
		printf("Opened sequencer port for ALSA on %d:%d\n",
			snd_seq_client_id(seq_handle_), port_id);
	}
	
//	snd_seq_client_info_t info;
//	snd_seq_get_client_info (seq_handle_, &info);

//	snd_seq_client_pool_set_output_room(info, 5000);
	snd_seq_set_output_buffer_size(seq_handle_, 100000);

	if (snd_seq_parse_address(seq_handle_, &dest_, destPort) < 0) {
		fprintf(stderr, "invalid sender address %s\n", destPort);
		exit(1);
		return false;
	}

/*	snd_seq_port_subscribe_alloca(&subs);
	snd_seq_port_subscribe_set_sender(subs, &dest);
//	snd_seq_port_subscribe_set_dest(subs, &dest);
	snd_seq_port_subscribe_set_queue(subs, queue);
	snd_seq_port_subscribe_set_exclusive(subs, exclusive);
	snd_seq_port_subscribe_set_time_update(subs, convert_time);
	snd_seq_port_subscribe_set_time_real(subs, convert_real);

	snd_seq_subscribe_port(seq_handle_, subs); */

	return true;
}
