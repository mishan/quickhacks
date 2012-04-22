#ifndef BREAD_NOTE_CLUSTER_H
#define BREAD_NOTE_CLUSTER_H

#include "BreadNote.h"

class BreadNoteCluster : public list<BreadNote*> {
public:
    BreadNoteCluster (int startTime, int windowSize, int pitchMin, int pitchMax,
					  int dynamicMin, int dynamicMax);
    ~BreadNoteCluster(void);
	
    void createRandomEvents (int count);
    BreadNoteCluster *mate (BreadNoteCluster *partner);
private:
    int startTime_, windowSize_;
    int pitchMin_, pitchMax_;
    int dynamicMin_, dynamicMax_;
};

#endif /* BREAD_NOTE_CLUSTER_H */
