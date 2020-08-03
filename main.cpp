#include "HeartBeat.h"
#include "DerivedHeartBeat.h"
#include "Pipe4HeartBeat.h"
#include <stdio.h>

int main() {
    Pipe4HeartBeat* pipe = new Pipe4HeartBeat();
    int DNnum = 2 ;
    int chunknum = 3;
    if (pipe->StaticPipeBetweenSerAndCli(DNnum, chunknum) == 0) {
        printf("main: ipc error!\n");
        return 0;
    }   
    return 1;

}
