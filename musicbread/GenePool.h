/* Copyright (C) 2005-2012 Misha Nasledov <misha@nasledov.com>
 *  All Rights Reserved.
 */
#ifndef GENE_POOL_H
#define GENE_POOL_H

class GenePool {
public:
    GenePool (int keyNote, int eventsPerGroup, int windowSize, 
              float selection, int mutations, int pitchMin, int pitchMax,
              int dynamicMin, int dynamicMax);
    ~GenePool (void);

    typedef list<BreadNoteCluster*> EventPool;
	typedef list<MIDITimedBigMessage*> MIDIEventList;

    void randomize (int count);
    void nextGeneration (void);
    void print (void);
	MIDIEventList *getEvents();
private:
    int evaluate (const BreadNoteCluster *events);
    void breed (const EventPool &breedingPool);
	void clearPool (void);

    int keyNote_; /* what key to place selective pressure on */
    int eventsPerGroup_; /* how many events in each BreadNoteCluster */
    int windowSize_; /* the time window for each generation */
    float selection_; /* what ratio of top-scoring BreadNoteClusters to breed */
    int mutations_; /* chance of mutation */
    int pitchMin_;
    int pitchMax_;
    int dynamicMin_;
    int dynamicMax_;
    int generation_; /* what generation we are currently on */

    EventPool pool_;
};

#endif /* GENE_POOL_H */
