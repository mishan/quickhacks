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
#include "BreadNoteCluster.h"

BreadNoteCluster::BreadNoteCluster (int startTime, int windowSize, int pitchMin,
                              int pitchMax, int dynamicMin, int dynamicMax)
    : startTime_(startTime), windowSize_(windowSize), pitchMin_(pitchMin),
      pitchMax_(pitchMax), dynamicMin_(dynamicMin), dynamicMax_(dynamicMax)
{
}

BreadNoteCluster::~BreadNoteCluster (void)
{
	for (BreadNoteCluster::iterator i = begin(); i != end(); ++i) {
		delete (*i);
	}
}

void BreadNoteCluster::createRandomEvents (int count)
{
    for (int i = 0; i < count; i++)
    {
        BreadNote *e = new BreadNote();

        e->randomizeTime(startTime_, windowSize_);
        e->randomizePitch(pitchMin_, pitchMax_);
        e->randomizeDynamic(dynamicMin_, dynamicMax_);

        push_back(e);
    }
}

BreadNoteCluster *BreadNoteCluster::mate (BreadNoteCluster *partner)
{
    BreadNoteCluster *child = new BreadNoteCluster(startTime_+windowSize_,
                                             windowSize_, pitchMin_,
                                             pitchMax_, dynamicMin_,
                                             dynamicMax_);

    /* iterate over ourselves and our partner simultaneously */
    for (iterator i = begin(), j = partner->begin(); 
         i != end() && j != partner->end(); ++i, ++j)
    {
        /* add to the child the result of *i (a cope event from us) and
         *j (a cope event from partner) being mated */
        child->push_back((*i)->mate(*j, startTime_, windowSize_));
        child->push_back((*j)->mate(*i, startTime_, windowSize_));
    }
    
    return child;
}
