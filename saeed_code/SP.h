#ifndef _SP_H_
#define _SP_H_



#include "Header.h"
#include "NetworkClass.h"
#include "RNG_MT.h"
#include "Cluster.h"
#include "Cluster_BFS.h"
#include "Utility.h"


pair<lint, double> Multiplex_SP (NETWORK &network1, 
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