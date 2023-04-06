#ifndef _HDA_H_
#define _HDA_H_



#include "Header.h"
#include "NetworkClass.h"
#include "RNG_MT.h"
#include "Cluster.h"
#include "Cluster_BFS.h"
#include "Utility.h"



lint One_Step_of_Multiplex_HDA (NETWORK &network1, NETWORK &network2, class MTRand *RandNumb);

pair<lint, double> Multiplex_HDA (NETWORK &network1, 
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
