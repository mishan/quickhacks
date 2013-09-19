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
#include <stdio.h>
#include <time.h>

#include "main.h"
#include "BreadNoteCluster.h"
#include "GenePool.h"

#define GENE_POOL_SIZE 20

#define KEY_NOTE 0 /* defines the scale */
#define EVENTS_PER_GROUP 5 /* how many CopeEvents in each CopeEventList */
#define WINDOWSIZE 2000 /* how much time per generation */
#define SELECTION .7 /* what percentage to drop */
#define MUTATION 0 /* chance of mutation; XXX: not implemented */
#define PITCH_MIN 40
#define PITCH_MAX 80
#define DYNAMIC_MIN 20
#define DYNAMIC_MAX 100

#define GENERATIONS 20

int main (int argc, char *argv[])
{
	if (argc < 2) {
		printf("Usage: %s [outfile]\n", argv[0]);
		return 1;
	}

	char *outfile_name = argv[1];

    /* seed our random number generator with the current time */
    srand(time(NULL));

    GenePool *pool = new GenePool(KEY_NOTE, EVENTS_PER_GROUP, WINDOWSIZE, 
                                  SELECTION, MUTATION, PITCH_MIN, PITCH_MAX,
                                  DYNAMIC_MIN, DYNAMIC_MAX);

	MIDIMultiTrack tracks(2);
	tracks.SetClksPerBeat(100);
	MIDITimedBigMessage timeSig;
	timeSig.SetTime(0);
	timeSig.SetTempo(1000000);
	tracks.GetTrack(0)->PutEvent(timeSig);

    pool->randomize(GENE_POOL_SIZE);

    for (int i = 0; i < GENERATIONS; i++)
    {
		GenePool::MIDIEventList *events = pool->getEvents();
		for(GenePool::MIDIEventList::iterator i = events->begin();
			i != events->end(); i++) {
			tracks.GetTrack(1)->PutEvent(*(*i));
			delete (*i);
		}
		delete events;
        pool->nextGeneration();
    }

	// need to call this since we inserted events out of order
	tracks.SortEventsOrder();

	MIDIFileWriteStreamFileName out_stream(outfile_name);


    if( out_stream.IsValid() )
    {
        // the object which takes the midi tracks and writes the midifile to the output stream
        MIDIFileWriteMultiTrack writer( &tracks, &out_stream );

        // write the output file
        if ( writer.Write(2) )
        {
            cout << "\nOK writing file " << outfile_name << endl;
        }
        else
        {
            cerr << "\nError writing file " << outfile_name << endl;
        }
    }
    else
    {
        cerr << "\nError opening file " << outfile_name << endl;
    }
}
