/* Copyright (C) 2005-2012 Misha Nasledov <misha@nasledov.com>
 *  All Rights Reserved.
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
