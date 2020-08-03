#include <unistd.h>
#include <string>
#include <stdio.h>
#include <time.h>
#include <signal.h>
#include <string.h>
#include "Ipc4HeartBeat.h"
#include "DerivedHeartBeat.h"
#include "HeartBeat.h"

#define HEARTBEAT_INTERVAL 1
#define MAX_PACKET_SIZE 1024
#define LINE_NUM 3
/*
int Ipc4HeartBeat::SigHandler(int signo)
{
    //read pipe
    if(read(pipefd[0], buf, MAX_PACKET_SIZE) == -1) {
	printf("Read from pipe error!");
	}
    //handle heatbeat packet
    //PackHandler(buf);
    printf("packet: %s\n\n", buf);
    return 1;
}
*/

int Ipc4HeartBeat::StaticIpcBetweenSerAndCli(int DNnum, int chunknum)   { 
    int num = DNnum + chunknum;

    //init pipe
    if (pipe(pipefd) == -1) {
        printf("Create pipe error!\n");
        return 0;
    }

	pid_t cpid;
    //pid_t ppid;
    //ppid = getpid();
    int i = 0;
    for (i = 0; i < num; i++) {
        cpid = fork();
        if (cpid == -1) {
            printf("Fork process error!\n");
            return 0;
        }
        if (!cpid) {
            break;
        }
    }
//children processes
    if (i < num) {
        close(pipefd[0]);
        if (i < DNnum) {
            std::string filename;
            filename = "DN" + std::to_string(i + 1) + ".txt";
            DNHeartBeat* DN_heartbeat = new DNHeartBeat(const_cast<char*>(filename.c_str()));
            //TODO:Need to add jump out conditions
            int j = 0;
            while (j < LINE_NUM) {
                char* packet = DN_heartbeat->CreateHeartBeatPacket();
		j++;
                if(write(pipefd[1], packet, DN_heartbeat->PacketSize()) == -1) {
			printf("Process %d write packet: %s failed!\n", getpid(), packet);
			exit(-1);
		}
/*
                //send signal to parent process
            if(kill(ppid, SIGUSR1) == -1) {
                    perror("Fail to send SIGUSR1!\n");
                    exit(-1);
                }
*/
                time_t ts = DN_heartbeat->LastTimeStamp();
                while ((int) (ts - DN_heartbeat->LastTimeStamp()) < HEARTBEAT_INTERVAL) {
                    ts = time(NULL);
                }
            }
        }
        else {
            std::string filename;
            filename = "Chunk" + std::to_string(i + 1 - DNnum) + ".txt";
            ChunkHeartBeat* chunk_heartbeat = new ChunkHeartBeat(const_cast<char*>(filename.c_str()));
            //TODO:Need to add jump out conditions
            int j = 0;
            while (j < LINE_NUM) {
                char* packet = chunk_heartbeat->CreateHeartBeatPacket();
		j++;
                write(pipefd[1], packet, chunk_heartbeat->PacketSize());
                //send signal to parent process
                /*
                if(kill(ppid, SIGUSR1) == -1) {
                    perror("Fail to send SIGUSR1!\n");
                    exit(-1);
                }
*/
                //sleep(HEARTBEAT_INTERVAL);
                time_t ts = chunk_heartbeat->LastTimeStamp();
                while ((int) (ts - chunk_heartbeat->LastTimeStamp()) < HEARTBEAT_INTERVAL) {
                    ts = time(NULL);
                }
            }
        }
       // kill(ppid, SIGTERM);
        //close(pipefd[1]);
        exit(1);        
    }
//parent process 
    else {
        //wait for init pipe
        //sleep(1);
        close(pipefd[1]);
	char* buf = (char*)malloc(MAX_PACKET_SIZE * num);
	//memset(buf, 0, MAX_PACKET_SIZE);
	int j = 0;
	while (j < LINE_NUM) {
		memset(buf, 0, MAX_PACKET_SIZE * num);
		sleep(HEARTBEAT_INTERVAL);
		if (read(pipefd[0], buf, MAX_PACKET_SIZE * num) == -1) {
			printf("Read pipe error!\n");
			exit(-1);
		}
		printf("buf[%d]: %s\n", j, buf);
		printf("++++++++++++++++++++++++++++++++++++++++\n");
		j++;
	}
    }
	return 1;
} 
