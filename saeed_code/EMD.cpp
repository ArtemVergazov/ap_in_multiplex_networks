#ifndef _EMD_CPP_
#define _EMD_CPP_



#include "EMD.h"



lint One_Step_of_Multiplex_EMD (NETWORK &network1, NETWORK &network2, class MTRand *RandNumb)
{
    vector <double> weights1 (network1.numNodes, 0.0);

    lint i, j, target, id, it, ITERATION;

    ITERATION=10;
    double min=0.0;
    id=-1;


    // Update Degrees.
    UpdateAllDegrees(network1, network2);


    // Initial weights
    for(i=ZERO ; i<network1.numNodes ; i++)
    {
        if(network1.gccRoot == network1.componentID[i])
            weights1[i]=(double)(network1.node[i].degree+network2.node[i].degree);
    }


    // Calculate weights
    for(it=ZERO ; it<ITERATION ; it++)
    {
        vector <double> weights2 (network1.numNodes, 0.0);

        for (i=ZERO ; i<network1.numNodes ; i++)
        {
            if (network1.gccRoot == network1.componentID[i])
            {
                for (j=ZERO ; j<network1.degree[i] ; j++)
                {       
                    target = network1.node[i].link[j].target;   
                    if (network1.gccRoot == network1.componentID[target]) 
                        weights2[i]+=weights1[target]/network1.node[target].degree;
                }

                for (j=ZERO ; j<network2.degree[i] ; j++)
                {       
                    target = network2.node[i].link[j].target;   
                    if (network2.gccRoot == network2.componentID[target]) 
                        weights2[i]+=weights1[target]/network2.node[target].degree;
                }

                weights2[i]/=2.0;
            }
        }

        weights1=weights2;
    }


    // Find the node with highest EMD.
    for(i=ZERO ; i<network1.numNodes ; i++)
    {
        if(weights1[i]>min)
        {
            id=i;
            min=weights1[i];
        }
    }


    if(-1==id)
    {
        vector <lint> nodes;
        for (lint i=ZERO ; i<network1.numNodes ; i++)
            if (PRESENT == network1.node[i].state) 
                nodes.push_back(i);

        id=RandNumb->randInt(nodes.size()-1);
        id=nodes[id];
    }


    return id;
}


pair<lint, double> Multiplex_EMD (NETWORK &network1, 
                                  NETWORK &network2, 
                                  vector <lint> &dismantlingSet,
                                  class MTRand *RandNumb, 
                                  const string costType,
        	                      const ulint seed, 
                                  const string outputAddress, 
                                  const double alpha, 
                                  const double beta,
                                  const bool printMCC)
{

    printf("\n***********************************\n");
    printf("Process: Effective Multiplex Degree [cost type=%s] \n", costType.c_str());
    printf("Network1: %s \t Network2: %s \n", network1.name.c_str(), network2.name.c_str());
    printf("SEED : %ld \n", seed);
    printf("Alpha=%lf \t beta=%lf \n", alpha, beta);
    // ******************************************** //


    ActivateAllNodesLinks(network1);
    ActivateAllNodesLinks(network2);


    char outputFileName1[FILE_NAME_LENGTH];
    sprintf(outputFileName1, "%s/EMD_%s._SEED_%ld._%s._%s.gmcc", outputAddress.c_str(), costType.c_str(), seed, 
        network1.name.c_str(), network2.name.c_str());
    FILE *file1 = fopen(outputFileName1, "w");
    if (NULL==file1)
    {
        printf("\n\nError in openning the file: %s \n\n", outputFileName1);
        exit(1);
    }


    char outputFileName2[FILE_NAME_LENGTH];
    sprintf(outputFileName2, "%s/EMD_%s._SEED_%ld._%s._%s.mcc",
            outputAddress.c_str(), costType.c_str(), seed, 
            network1.name.c_str(), network2.name.c_str());
    FILE *file2 = fopen(outputFileName2, "w");
    if (NULL==file2)
    {
        printf("\n\nError in openning the file: %s \n\n", outputFileName2);
        exit(1);
    }
    

    lint removed=ZERO, id=-1, deltaN=ZERO;
    double q=0.0, gmcc, threshold1, threshold2, cost=0.0, totalCost;


    InitializeTotalCosts(network1, network2, costType);
    totalCost=network1.totalCost+network2.totalCost;


    DetectAllIsolatedComponents(network1);
    DetectAllIsolatedComponents(network2);
    

    lint gcc1=network1.gccSize, gcc2=network2.gccSize;
    lint gmcc12=DetectAllMutuallyConnectedComponents(network1, network2);
    gmcc = (double)gmcc12/(double)network1.numNodes;
    threshold1 = (alpha<0.0)?alpha:(alpha*gmcc12);
    threshold2 = pow(gmcc12, beta);
    printf("Threshold1: %lf \t Threshold2: %lf\n", threshold1, threshold2);
    printf("\ngcc1: %ld \t gcc2: %ld \t gmcc12: %ld\n", gcc1, gcc2, gmcc12);
    // print detail to the file.
    fprintf(file1, "##### q \t cost \t gmcc\n");
    fprintf(file1, "%lf \t %lf \t %lf \n", q, cost, gmcc);
    if(printMCC)
    {
        fprintf(file2, "##### id \t q \t cost \t gmcc #####\n");
        fprintf(file2, "#None %lf \t %lf \t %lf \n", q, cost, gmcc);
    	PrintMCCDistribution(network1, file2, removed);
    }


    //##### Main ALgorithm #####//
    vector <lint> nodes (network1.numNodes); 
    for (lint i=ZERO ; i<network1.numNodes ; i++) {nodes[i] = i;}
    PermutationNodeList (nodes, RandNumb);

    while (removed<network1.numNodes)
    {        
        id=One_Step_of_Multiplex_EMD(network1, network2, RandNumb);
        if(id<ZERO)
        {
            printf("\n\nERROR:: Negative id for removal in SP! \n\n");
            exit(1);
        }

        if (PRESENT == network1.node[id].state)
        {
            network1.node[id].state = ABSENT;
            network2.node[id].state = ABSENT;
        }
        else
        {
            printf("\n\nERROR in SP Method: trying to remove ABSENT node! \n\n");
            exit(1);
        }
        
        removed++;
        q=(double)(removed)/(double)network1.numNodes;
        dismantlingSet.push_back(id);


        if("UNIT"==costType)
            cost=q;
        else if ("DEGREE"==costType)
            cost+=(double)(network1.node[id].degree+network2.node[id].degree)/totalCost;
        else if ("WEIGHT"==costType)
            cost+=(network1.node[id].cost+network2.node[id].cost)/totalCost;
        else
        {
            printf("\nERROR:: Invalid Cost Type!\n\n");
            exit(1);
        }


        gmcc12=DetectAllMutuallyConnectedComponents(network1, network2);
        gmcc = (double)gmcc12/(double)network1.numNodes;


        fprintf(file1, "%g \t %g \t %g \n", q, cost, gmcc);
        fprintf(stderr,"%ld \t %lf \t %lf \t %lf\r", id, q, cost, gmcc);
        if(printMCC)
        {
        	fprintf(file2, "%ld %lf %lf %lf\n", id, q, cost, gmcc);
        	PrintMCCDistribution(network1, file2, removed);	
        }
        

        if(gmcc12<threshold1)
        {
            deltaN++;
            if (gmcc12<=threshold2)
                break;
        }

    }

    
    if(printMCC){fprintf(file2, "#deltaN %ld \n", deltaN);}
    fclose(file1);
    fclose(file2);


    printf("\nremoved : %ld \t cost: %lf \t gmcc : %lf \t deltaN : %ld\n\n", removed, cost, gmcc, deltaN);

    
    return make_pair(deltaN,q);
}


#endif 