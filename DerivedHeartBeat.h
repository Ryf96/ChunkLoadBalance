#ifndef DERIVED_HEARTBEAT_H
#define DERIVED_HEARTBEAT_H

#include "HeartBeat.h"

class DNHeartBeat : public HeartBeat {
public:
    DNHeartBeat(char* file) : HeartBeat(file){		
}
    virtual ~DNHeartBeat();
    char* CreateHeartBeatPacket();
};

class ChunkHeartBeat : public HeartBeat {
public:
    ChunkHeartBeat(char* file) : HeartBeat(file){} 
    virtual ~ChunkHeartBeat();
    char* CreateHeartBeatPacket();
};


#endif
