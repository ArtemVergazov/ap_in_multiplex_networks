#ifndef _CLUSTER_H_
#define _CLUSTER_H_



#include "Header.h"
#include "NetworkClass.h"





lint findClusterRoot(lint i, vector <lint> &ptr);

lint AddNode (Network &network, lint n, lint EMPTY);

void RemoveNode (Network &network, lint node, lint EMPTY);

pair <lint,lint> FindAllClusters (Network &network);

lint FindGMCC (NETWORK &networkA, NETWORK &networkB);




#endif // _CLUSTER_H_
