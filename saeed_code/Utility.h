#ifndef _UTILITY_H
#define _UTILITY_H



#include "Header.h"
#include "NetworkClass.h"
#include "RNG_MT.h"
#include "Cluster_BFS.h"



void InitializeTotalCosts (NETWORK &network1, NETWORK &network2, const string costType);

void PrintMCCDistribution(const NETWORK &network, FILE *file, const lint removed);

void UpdateAllDegrees(NETWORK &network1, NETWORK &network2);

void UpdateAllDegrees_EI(NETWORK &network1, NETWORK &network2);

void UpdateGMCCDegrees(NETWORK &network1, NETWORK &network2);

double OverlappingFraction (const NETWORK &network1, const NETWORK &network2, set <lint> &nodes);

void ActivateAllNodesLinks (NETWORK &network);

void RandomizeLabels(const NETWORK &network, NETWORK &networkRandomized, const ulint seed, const double randomization);

void AggregateLayers (const NETWORK &network1, const NETWORK &network2, NETWORK &networkAggregated);

void PermutationCommunityNodeList (vector <lint> &nodeList, class MTRand *RandNumb);

void PermutationNodeList (vector <lint> &nodeList, class MTRand *RandNumb);

void PermutationNodeScorePair (vector < pair<lint, lint> > &nodeScorePair, class MTRand *RandNumb);

void PermutationNodeScorePairDouble (vector < pair<lint, double> > &nodeScorePair, class MTRand *RandNumb);

bool ComparePair (const pair<lint,lint> &a, const pair<lint,lint> &b);

bool ComparePairDouble (const pair<lint,double> &a, const pair<lint,double> &b);

bool Compare (const lint &a, const lint &b);

/**************************************************************************************/


//void GetUserInputs(int argc, char* argv[], PARAMETERS &par);

int ReturnKeyWordValue(int argc, char* argv[], string key);

bool checkKeyWord(int argc, char* argv[], string key);




#endif 
