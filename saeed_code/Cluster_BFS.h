#ifndef _CLUSTER_BFS_H_
#define _CLUSTER_BFS_H_



#include "Header.h"
#include "NetworkClass.h"




void DetectAllIsolatedComponents (NETWORK &network);

lint DetectAllMutuallyConnectedComponents (NETWORK &networkA, NETWORK &networkB);

void CheckCorrectnessOfDetectingMCC (NETWORK &networkA, NETWORK &networkB);



#endif // _CLUSTER_H_
