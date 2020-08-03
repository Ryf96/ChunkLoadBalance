#include "HeartBeat.h"
#include "DerivedHeartBeat.h"
#include <time.h>
#include <fstream>
#include <string>
#include <iostream>
#include <stdio.h>

HeartBeat::~HeartBeat() {
	printf("Delete HeartBeat\n");
}
DNHeartBeat::~DNHeartBeat() {
	printf("Delete DNHeartBeat\n");
}
ChunkHeartBeat::~ChunkHeartBeat() {
	printf("Delete ChunkHeartBeat\n");
}

char* DNHeartBeat::CreateHeartBeatPacket() {
    std::ifstream f(filename);
    int i = 0;
    std::string res;
    while (i <= line_has_read) {
        i++;
        if (!getline(f, res)) {
            printf("Read file %s completed!\n", filename);
            break;
        }
    }
    line_has_read++;
	packetsize = res.size();	
    time_t timestamp = time(NULL);
    ts = (int) timestamp;
    return const_cast<char*>(res.c_str());
}

char* ChunkHeartBeat::CreateHeartBeatPacket() {
    std::ifstream f(filename);
    int i = 0;
    std::string res;
    while (i <= line_has_read) {
        i++;
        if (!getline(f, res)) {
            printf("Read file %s completed!\n", filename);
            break;
        }
    }
    line_has_read++;
	packetsize = res.size();
    time_t timestamp = time(NULL);
    ts = (int) timestamp;
    return const_cast<char*>(res.c_str());
}
