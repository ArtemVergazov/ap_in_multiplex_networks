#ifndef _EI_CPP
#define _EI_CPP



#include "EI.h"



void EffectiveDegree(const NETWORK &network, vector <double> &keff)
{

    double hubs, leaves, err=1.0;
    lint i, j, neighbor;
    vector <double> tmp_keff (network.numNodes);
    for(i=ZERO ; i<network.numNodes ; i++)
        {keff[i] = (double)network.degree[i];}

    while (err>1.0e-6)
    {
        err = 1e6;

        for(i=ZERO ; i<network.numNodes ; i++)
        {
            tmp_keff[i] = keff[i];
            keff[i] = 0.0;
        }

        for(i=ZERO ; i<network.numNodes ; i++)
        {

            hubs = leaves = 0.0;

            for(j=ZERO ; j<network.degree[i] ; j++)
            {
                neighbor=network.node[i].link[j].target;
                if (network.degree[neighbor] == ONE)
                {
                    leaves += 1.0;
                }
                if (tmp_keff[neighbor] >= K_HUB)
                {
                    hubs += 1.0;
                }
            }

            keff[i] = (double)network.degree[i]-leaves-hubs;

            if (fabs(keff[i]-tmp_keff[i]) < err)
                {err = fabs(keff[i]-tmp_keff[i]);}
        }
    }

}



double Kernel (NETWORK &network1, NETWORK &network2, lint node)
{

    lint i, neighbor, neighborRoot;
    lint totalDegree = network1.degree[node]+network2.degree[node];
    double score = 0.0;
    set <lint> cluster1, cluster2;

    // detect MCCs of 'node' in network1. [MCC:Mutually Conected Cluster]
    for (i=ZERO ; i<network1.degree[node] ; i++)
    {
        neighbor = network1.node[node].link[i].target;

        if (PRESENT != network1.node[neighbor].state)
            {continue;}

        neighborRoot = findClusterRoot(neighbor, network1.ptr);

        if (cluster1.insert(neighborRoot).second)
            {score += sqrt(-network1.ptr[neighborRoot])-ONE;}

    }

    // detect MCCs of 'node' in network2.
    for (i=ZERO ; i<network2.degree[node] ; i++)
    {
        neighbor = network2.node[node].link[i].target;

        if (PRESENT != network2.node[neighbor].state)
            {continue;}

        neighborRoot = findClusterRoot(neighbor, network2.ptr);

        if ( cluster2.insert(neighborRoot).second )
            {score += sqrt(-network2.ptr[neighborRoot])-ONE;}

    }

    score = score/2.0+totalDegree;

    return score;
}



double KernelWithEffectiveDegree (NETWORK &network1, 
                                  NETWORK &network2,
                                  const vector <double> &keff1, 
                                  const vector <double> &keff2, 
                                  lint node)
{

    lint i, neighbor, neighborRoot;
    double score=0.0, totalDegree = sqrt(keff1[node]*keff2[node]);
    set <lint> cluster1, cluster2;

    // detect MCCs of 'node' in network1. [MCC:Mutually Conected Cluster]
    for (i=ZERO ; i<network1.degree[node] ; i++)
    {
        neighbor = network1.node[node].link[i].target;

        if (PRESENT != network1.node[neighbor].state)
            {continue;}

        neighborRoot = findClusterRoot(neighbor, network1.ptr);

        if (cluster1.insert(neighborRoot).second)
            {score += (sqrt(-network1.ptr[neighborRoot]));}
    }


    // detect MCCs of 'node' in network2.
    for (i=ZERO ; i<network2.degree[node] ; i++)
    {
        neighbor = network2.node[node].link[i].target;

        if (PRESENT != network2.node[neighbor].state)
            {continue;}

        neighborRoot = findClusterRoot(neighbor, network2.ptr);

        if (cluster2.insert(neighborRoot).second)
            {score += (sqrt(-network2.ptr[neighborRoot]));}

    }

    score = score/2.0+totalDegree;

    return score;
}



lint One_Step_of_Multiplex_EI (NETWORK &network1, 
                               NETWORK &network2, 
                               vector <lint> &nodeList, 
                               const vector <double> &keff1, 
                               const vector <double> &keff2,
                               class MTRand *RandNumb)
{
    PermutationNodeList(nodeList, RandNumb);
#ifdef _MSC_VER
#pragma warning( push ) // save current warning settings
#pragma warning( disable : 4305 )
#pragma warning( disable : 4309 )
#endif
    lint id=-1, minScore=LARGE_NUMBER, minID=-1;
    lint size=(nodeList.size()>=CANDIDATE)?CANDIDATE:nodeList.size();
    double score=0.0;

    for (lint i=ZERO ; i<size ; i++)
    {
        score = KernelWithEffectiveDegree(network1, network2, keff1, keff2, nodeList[i]);

        if (score<minScore)
        {
            minScore = static_cast<lint>(score);
            minID = i;
        }
    }

    id=nodeList[minID];
    nodeList[minID] = nodeList.back();
    nodeList.pop_back();

    return id;
}


pair<lint, double> Multiplex_EI (NETWORK &network1, 
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
    printf("Process: Explosive Immunization [cost type=%s] \n", costType.c_str());
    printf("Network1: %s \t Network2: %s \n", network1.name.c_str(), network2.name.c_str());
    printf("SEED : %ld \n", seed);
    printf("Alpha=%lf \t beta=%lf \n", alpha, beta);
    // ******************************************** //


    ActivateAllNodesLinks(network1);
    ActivateAllNodesLinks(network2);


    char outputFileName1[FILE_NAME_LENGTH];
    sprintf(outputFileName1, "%s/EI_%s._SEED_%ld._%s._%s.gmcc", outputAddress.c_str(), costType.c_str(), seed, 
        network1.name.c_str(), network2.name.c_str());
    FILE *file1 = fopen(outputFileName1, "w");
    if (NULL==file1)
    {
        printf("\n\nError in openning the file: %s \n\n", outputFileName1);
        exit(1);
    }


    char outputFileName2[FILE_NAME_LENGTH];
    sprintf(outputFileName2, "%s/EI_%s._SEED_%ld._%s._%s.mcc",
            outputAddress.c_str(), costType.c_str(), seed, 
            network1.name.c_str(), network2.name.c_str());
    FILE *file2 = fopen(outputFileName2, "w");
    if (NULL==file2)
    {
        printf("\n\nError in openning the file: %s \n\n", outputFileName2);
        exit(1);
    }
    

    lint removed=ZERO, insert=network1.numNodes, id=-1, deltaN=ZERO;
    double q=0.0, gmcc, threshold1, threshold2, cost=0.0, totalCost;

    InitializeTotalCosts(network1, network2, costType);
    totalCost=network1.totalCost+network2.totalCost;
    printf("totalCost %lf\n", totalCost);


    DetectAllIsolatedComponents(network1);
    DetectAllIsolatedComponents(network2);
    

    lint gcc1=network1.gccSize, gcc2=network2.gccSize;
    lint gmcc12=DetectAllMutuallyConnectedComponents(network1, network2);
    gmcc = (double)gmcc12/(double)network1.numNodes;
    threshold1 = alpha;
    threshold2 = pow(gmcc12, beta)/(double)gmcc12;
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

    vector <lint> nodeList(network1.numNodes), nodeOrdered (network1.numNodes);
    vector <double> keff1 (network1.numNodes), keff2 (network2.numNodes);
    vector <double> costOrdered(network1.numNodes, 0.0), gmccOrdered(network1.numNodes+1);

    for (lint i=ZERO ; i<network1.numNodes ; i++)
        {nodeList[i] = i;}
    // Initially All nodes are removed, but all links are present.
    for (lint i=ZERO ; i<network1.numNodes ; i++)
        {network1.node[i].state = network2.node[i].state = ABSENT;}
    // calculate keff for two networks.
    EffectiveDegree(network1, keff1);
    EffectiveDegree(network2, keff2);


    while (insert>ZERO)
    {        
        id= One_Step_of_Multiplex_EI(network1, network2, nodeList, keff1, keff2, RandNumb);

        if(id<ZERO || id>network1.numNodes-1)
        {
            printf("\n\nERROR:: Negative id for addition in EI! \n\n");
            exit(1);
        }

        if (PRESENT == network1.node[id].state)
        {
            printf("\n\nERROR in EI :: trying to add already PRESNET node!\n\n");
            exit(1);
        }
        else
        {
            network1.node[id].state = PRESENT;
            network2.node[id].state = PRESENT;
        }
        
        insert--;
        gmcc = (double)FindGMCC(network1, network2)/(double)network1.numNodes;
        
        nodeOrdered[insert]=id;
        gmccOrdered[insert]=gmcc;

        UpdateAllDegrees_EI(network1, network2);

        if("DEGREE"==costType)
            {costOrdered[insert]=(double)(network1.node[id].degree+network2.node[id].degree);}
        
        
    }

    cost=0.0;
    gmccOrdered[network1.numNodes]=0.0;

    for(lint i=ZERO; i<network1.numNodes; i++)
    {
        q=(double)(i+1)/(double)network1.numNodes;
        dismantlingSet.push_back(nodeOrdered[i]);

        
        if("UNIT"==costType)
            cost=q;
        else if("DEGREE"==costType)
            cost+=(costOrdered[i])/totalCost;
        else if("WEIGHT"==costType)
            cost+=(network1.node[id].cost+network2.node[id].cost)/totalCost;
        else
        {
            printf("\nERROR:: Invalid Cost Type!\n\n");
            exit(1);
        }


        fprintf(file1, "%g \t %g \t %g \n", q, cost, gmccOrdered[i+1]);
        fprintf(stderr,"%ld \t %lf \t %lf \t %lf\r", nodeOrdered[i], q, cost, gmccOrdered[i+1]);
        if(printMCC)
        {
            fprintf(file2, "%ld %lf %lf %lf\n", nodeOrdered[i], q, cost, gmccOrdered[i+1]);
            PrintMCCDistribution (network1, file2, removed); 
        }
        

        if(gmccOrdered[i+1]<threshold1)
        {
            removed++;
            deltaN++;
            if (gmccOrdered[i+1]<=threshold2)
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
