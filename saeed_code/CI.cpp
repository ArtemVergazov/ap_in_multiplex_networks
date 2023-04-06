#ifndef _CI_CPP_
#define _CI_CPP_




#include "CI.h"


lint CI_Score (lint source, NETWORK &network)
{
    
    lint i, ci=ZERO, node, neighbor, d=ZERO, read_pointer, write_pointer;

    // distance of 'source' from itself is ZERO.
    network.Distance[source] = ZERO;
    network.Queue[ZERO] = source;

    vector <lint> ball;
    ball.push_back(source);

    read_pointer  = ZERO;
    write_pointer = ONE;

    // go through neighborhood of 'source' untill reach surface of ball(Gradius).
    while(read_pointer != write_pointer)
    {
        // read a node from Queue to determine it's neighbors distances.
        node = network.Queue[read_pointer];
        read_pointer++ ;

        // distance of 'source' from node.
        d = network.Distance[node];

        // when you reach the surface of the ball, 
        //Do not go outside the Ball(Gradius).
        if (Gradius==d)
        {
            ci += (network.node[node].degree-ONE);
            continue ;
        }

        // go through node's neighbors and set 
        //its unknown neighbor's distance to distance+1
        for (i=ZERO ; i<network.degree[node] ; i++)
        {
            neighbor = network.node[node].link[i].target;
      
            if (UNKNOWN == network.Distance[neighbor] && network.gccRoot == network.componentID[neighbor])
            {
                network.Distance[neighbor] = d+ONE;
                ball.push_back(neighbor);
                network.Queue[write_pointer] = neighbor;
                write_pointer++ ;
            }
        }
    }


    lint size = ball.size();
    for (i=ZERO ; i<size ; i++)
        network.Distance[ball[i]] = UNKNOWN;

    return ci;
}


lint One_Step_of_Multiplex_CI (NETWORK &network1, NETWORK &network2, class MTRand *RandNumb)
{
    vector <lint> nodes;
    lint min=ZERO, id=-1, score, ci1, ci2;

    for (lint i=ZERO ; i<network1.numNodes ; i++)
    {
        network1.Distance[i]=network2.Distance[i]=UNKNOWN;

        if (PRESENT == network1.node[i].state && network1.gccRoot == network1.componentID[i])
        {
            ci1 = CI_Score(i, network1);
            ci2 = CI_Score(i, network2);

            score  = (network1.node[i].degree)*ci1;
            score *= (network2.node[i].degree)*ci2;

            if(score>min)        
            {
                nodes.clear();
                nodes.push_back(i);
                min=score;
            }
            else if(score==min)
            {
                nodes.push_back(i);
            }        
        }
    }

    id=RandNumb->randInt(nodes.size()-1);
    id=nodes[id];

    return id;
}




pair<lint, double> Multiplex_CI (NETWORK &network1, 
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
    printf("Process: Collective Influence [cost type=%s] \n", costType.c_str());
    printf("Network1: %s \t Network2: %s \n", network1.name.c_str(), network2.name.c_str());
    printf("SEED : %ld \n", seed);
    printf("Alpha=%lf \t beta=%lf \n", alpha, beta);
    // ******************************************** //


    ActivateAllNodesLinks(network1);
    ActivateAllNodesLinks(network2);


    char outputFileName1[FILE_NAME_LENGTH];
    sprintf(outputFileName1, "%s/CI_%s._SEED_%ld._%s._%s.gmcc", outputAddress.c_str(), costType.c_str(), seed, 
        network1.name.c_str(), network2.name.c_str());
    FILE *file1 = fopen(outputFileName1, "w");
    if (NULL==file1)
    {
        printf("\n\nError in openning the file: %s \n\n", outputFileName1);
        exit(1);
    }


    char outputFileName2[FILE_NAME_LENGTH];
    sprintf(outputFileName2, "%s/CI_%s._SEED_%ld._%s._%s.mcc",
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
        UpdateAllDegrees(network1, network2);

        id=One_Step_of_Multiplex_CI(network1, network2, RandNumb);
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