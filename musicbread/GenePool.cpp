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
#include "BreadNoteCluster.h"
#include "GenePool.h"
#include "music-util.h"

/*GenePool::GenePool (void)
    : selection_(DEFAULT_SELECTION), mutations_(DEFAULT_MUTATIONS)
{
}*/

GenePool::GenePool (int keyNote, int eventsPerGroup, int windowSize, 
                    float selection, int mutations, int pitchMin, int pitchMax,
                    int dynamicMin, int dynamicMax)
    : keyNote_(keyNote), eventsPerGroup_(eventsPerGroup), 
      windowSize_(windowSize), selection_(selection), mutations_(mutations),
      pitchMin_(pitchMin), pitchMax_(pitchMax), dynamicMin_(dynamicMin),
      dynamicMax_(dynamicMax)
{
    generation_ = 0;
}

GenePool::~GenePool (void)
{
	clearPool();
}

void GenePool::clearPool (void)
{
	for (EventPool::iterator i = pool_.begin(); i != pool_.end(); ++i) {
		delete (*i);
	}
}

void GenePool::randomize (int count)
{
	clearPool();

    for (int i = 0; i < count; i++)
    {
        BreadNoteCluster *elist = new BreadNoteCluster(generation_*windowSize_,
                                                 windowSize_, pitchMin_,
                                                 pitchMax_, dynamicMin_,
                                                 dynamicMax_);

        elist->createRandomEvents(eventsPerGroup_);
        pool_.push_back(elist);
    }
}

GenePool::MIDIEventList *GenePool::getEvents(void)
{
	list<MIDITimedBigMessage*> *events = new list<MIDITimedBigMessage*>;

    for (EventPool::iterator i = pool_.begin(); i != pool_.end(); ++i)
    {
		for (BreadNoteCluster::iterator j = (*i)->begin(); j != (*i)->end(); ++j)
		{
            BreadNote *e = (*j);
			
            u_int8_t pitch = e->pitch();
            u_int32_t ontime = e->ontime();
            u_int32_t duration = e->duration();
            u_int8_t channel = e->channel();
            u_int8_t dynamic = e->dynamic();

			MIDITimedBigMessage *m = new MIDITimedBigMessage;
			m->SetTime(ontime);
			m->SetNoteOn(channel, pitch, dynamic);
			events->push_back(m);
			m = new MIDITimedBigMessage;
			m->SetTime(ontime+duration);
			m->SetNoteOff(channel, pitch, dynamic);
			events->push_back(m);

//            printf("(%d %d %d %d %d) ", ontime, pitch, duration, channel,
//                   dynamic);
        }
	}

	return events;
}

void GenePool::print (void)
{
    for (EventPool::iterator i = pool_.begin(); i != pool_.end(); ++i)
    {
//        int score = evaluate(*i);

        for (BreadNoteCluster::iterator j = (*i)->begin(); j != (*i)->end(); ++j)
        {
            BreadNote *e = (*j);

            u_int8_t pitch = e->pitch();
            u_int32_t ontime = e->ontime();
            u_int32_t duration = e->duration();
            u_int8_t channel = e->channel();
            u_int8_t dynamic = e->dynamic();

            printf("(%d %d %d %d %d) ", ontime, pitch, duration, channel,
                   dynamic);
        }
//      printf("-- score: %d\n", score);
//      printf("-----------------\n");
    }
}

void GenePool::nextGeneration (void)
{
    int *scores = new int[pool_.size()];
    int maxScore = 0;
    EventPool breedingPool;
    int eventCnt = 0;

    /* go through gene pool, assign scores to each item */
    for (EventPool::iterator i = pool_.begin(); i != pool_.end(); ++i)
    {
        scores[eventCnt] = evaluate(*i);
        
        if (scores[eventCnt] > maxScore)
            maxScore = scores[eventCnt];

        eventCnt++;
    }

    if (maxScore == 0)
    {
        generation_++;
        randomize(pool_.size());
        delete scores;

        return;
    }

    /* go through the pool again, and check if the corresponding item in the
       pool has a score ratio that is acceptable to our selection rate; if so,
       then add it to our breedingPool */
    eventCnt = 0;
    for (EventPool::iterator i = pool_.begin(); i != pool_.end(); ++i)
    {
        if ((float)(scores[eventCnt]/maxScore) >= selection_)
        {
            breedingPool.push_back(*i);
        }

        eventCnt++;
    }

    breed(breedingPool);

    delete scores;
}

int GenePool::evaluate (const BreadNoteCluster *events)
{
    int tonality = 0;

    for (BreadNoteCluster::const_iterator i = events->begin(); i != events->end();
         ++i)
    {
        if (MusicUtil::isNaturalMinor((*i)->pitch(), keyNote_))
            tonality++;
    }

    return tonality;
}

void GenePool::breed (const EventPool &breedingPool)
{
    generation_++;

    /* woops! our selection wasn't good enough ... let's take this from the
       top! */
    if (breedingPool.size() <= 1) {
        randomize(pool_.size());
        return;
    }

    pool_.clear();

    for (EventPool::const_iterator i = breedingPool.begin();
         i != breedingPool.end(); ++i)
    {
        for (EventPool::const_iterator j = breedingPool.begin();
             j != breedingPool.end(); ++j)
        {
            /* don't mate with ourselves!! */
            if (i == j)
                continue;

            pool_.push_back((*i)->mate(*j));
        }
    }
}
