#ifndef PIPE4HEARTBEAT_H
#define PIPE4HEARTBEAT_H

#define MAX_PACKET_SIZE 1024
class Pipe4HeartBeat {
public:    
    // signal handler
    int SigHandler(int signo);
    int StaticPipeBetweenSerAndCli(int DNnum, int chunknum);
    
private: 
    int pipefd[2] = {1};
    char buf[MAX_PACKET_SIZE] = {0};  
};

#endif   
