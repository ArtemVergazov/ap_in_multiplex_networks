#ifndef _AP_H_
#define _AP_H_



#include "Header.h"
#include "NetworkClass.h"
#include "RNG_MT.h"
#include "Cluster.h"
#include "Cluster_BFS.h"
#include "Utility.h"



//const double GmaxDegree = 7.0;
//const double GminDegree = 2.0;

//const lint Gtime = (lint) 10;

//const double Gclustering = 5.0;

//const double Gthreshold = 5.0;


lint NumberOfArticulationPoints (NETWORK &network1, NETWORK &network2);


//APAV
void ArticulationPointsAverageDegree();

//GAPR
void GreedyArticulationPointsRemovalAverageDegree();
void GreedyArticulationPointsRemovalRealMultiplex(NETWORK &network1, NETWORK &network2);

//APTA
pair <double, double> ArticulationPointsTargetedAttack(NETWORK &network, class MTRand *RandNumb, FILE *file, FILE *SNfile);





#endif 
