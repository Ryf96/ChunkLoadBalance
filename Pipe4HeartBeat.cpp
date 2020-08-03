#include <unistd.h>
#include <string>
#include <stdio.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include "Pipe4HeartBeat.h"
#include "DerivedHeartBeat.h"
#include "HeartBeat.h"

#define HEARTBEAT_INTERVAL 1
#define MAX_PACKET_SIZE 1024
#define LINE_NUM 3

int Pipe4HeartBeat::StaticPipeBetweenSerAndCli(int DNnum, int chunknum)   { 
    int num = DNnum + chunknum;
	const char* suffix = ".fifo";

	pid_t cpid;
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
        if (i < DNnum) {
            std::string filename;
            filename = "DN" + std::to_string(i + 1);
			std::string pipename;
			pipename = filename + suffix;

			if((mkfifo(const_cast<char*>(pipename.c_str()), 0777) < 0) && (errno != EEXIST)) {
				perror("mkfifo error when reading");
			}
			int pipefd = open(const_cast<char*>(pipename.c_str()), O_WRONLY);
			if (pipefd < 0) {
				perror("open pipe error when writing");
				exit(-1);
			}

            DNHeartBeat* DN_heartbeat = new DNHeartBeat(const_cast<char*>(filename.c_str()));
            int j = 0;
            while (j < LINE_NUM) {
                char* packet = DN_heartbeat->CreateHeartBeatPacket();
		        
                if(write(pipefd, packet, DN_heartbeat->PacketSize()) == -1) {
			        printf("Process %d write packet: %s failed!\n", getpid(), packet);
			        exit(-1);
		        }

                time_t ts = DN_heartbeat->LastTimeStamp();
                while ((int) (ts - DN_heartbeat->LastTimeStamp()) < HEARTBEAT_INTERVAL) {
                    ts = time(NULL);
                }
                j++;
            }
        }

        else {
			std::string filename;
            filename = "Chunk" + std::to_string(i + 1 - DNnum);
			std::string pipename;
			pipename = filename + suffix;

			if((mkfifo(const_cast<char*>(pipename.c_str()), 0777) < 0) && (errno != EEXIST)) {
				perror("mkfifo error when reading");
			}
			int pipefd = open(const_cast<char*>(pipename.c_str()), O_WRONLY);
			if (pipefd < 0) {
				perror("open pipe error when writing");
				exit(-1);
			}

            ChunkHeartBeat* Chunk_heartbeat = new ChunkHeartBeat(const_cast<char*>(filename.c_str()));
            int j = 0;
            while (j < LINE_NUM) {
                char* packet = Chunk_heartbeat->CreateHeartBeatPacket();
		        
                if(write(pipefd, packet, Chunk_heartbeat->PacketSize()) == -1) {
			        printf("Process %d write packet: %s failed!\n", getpid(), packet);
			        exit(-1);
			    }
                
                time_t ts = Chunk_heartbeat->LastTimeStamp();
                while ((int) (ts - Chunk_heartbeat->LastTimeStamp()) < HEARTBEAT_INTERVAL) {
                    ts = time(NULL);
                }
                j++;
            }
		}      
    }
//parent process 
    else {
        char* buf = (char*)malloc(MAX_PACKET_SIZE * num);
        int j = 0;
        while (j < LINE_NUM) {
            for (int i = 0; i < num; i++) {
                std::string pipename;
                if (i < DNnum) {
                    pipename = "DN" + std::to_string(i + 1) + suffix;
                }
                else {
                    pipename = "Chunk" + std::to_string(i + 1 - DNnum) + suffix;
                }

                if((mkfifo(const_cast<char*>(pipename.c_str()), 0777) < 0) && (errno != EEXIST)) {
                    perror("mkfifo error when reading");
                }
                int pipefd = open(const_cast<char*>(pipename.c_str()), O_RDONLY);
                if (pipefd < 0) {
                    perror("open pipe error when writing");
                    exit(-1);
                }

                memset(buf, 0, MAX_PACKET_SIZE * num);
                if (read(pipefd, buf, MAX_PACKET_SIZE) == -1) {
                    printf("Read pipe error!\n");
                    exit(-1);
                }

                printf("buf[%d][%d]: %s\n", j, i, buf);
                printf("++++++++++++++++++++++++++++++++++++++++\n");
            }
            j++;
            sleep(HEARTBEAT_INTERVAL);
        }
    }
	return 1;
} 
