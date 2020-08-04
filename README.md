# ChunkLoadBalance
Load balance for between CNs and DNs or chunks

#souce-code files:
1.HeartBeat.h
2.DerivedHeartBeat.h
3.Pipe4HeartBeat.h
4.DerivedHeartBeat.cpp
5.Pipe4HeartBeat.cpp
6.main.cpp

#input files
1.DN1
2.DN2
3.Chunk1
4.Chunk2
5.Chunk3

#temp files
*.fifo

#compile
$ g++ -Wall -Werror -std=c++11 DerivedHeartBeat.cpp Pipe4HeartBeat.cpp main.cpp
