#ifndef _EI_H
#define _EI_H



#include "Header.h"
#include "NetworkClass.h"
#include "RNG_MT.h"
#include "Cluster.h"
#include "Cluster_BFS.h"
#include "Utility.h"



#define LARGE_NUMBER 1000000000000L
#define K_HUB 6.00
#define CANDIDATE 1000



pair<lint, double> Multiplex_EI (NETWORK &network1, 
                                 NETWORK &network2, 
                                 vector <lint> &dismantlingSet,
                                 class MTRand *RandNumb, 
                                 const string costType,
                                 const ulint seed, 
                                 const string outputAddress, 
                                 const double alpha, 
                                 const double beta,
                                 const bool printMCC);




#endif 
