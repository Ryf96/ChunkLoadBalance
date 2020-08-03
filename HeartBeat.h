#ifndef HEARTBEAT_H
#define HEARTBEAT_H

class HeartBeat {
public:
    HeartBeat(char* file) : filename(file){
	line_has_read = 0;
	packetsize = 0;
	ts = 0;
}
    virtual ~HeartBeat();

    //virtual char* CreateHeartBeatPacket();
    int PacketSize()    {   return packetsize;  }
    int LastTimeStamp() {   return ts;          }
protected:
    int line_has_read;
    int packetsize;
    int ts;
    char* filename;
    
};

#endif
