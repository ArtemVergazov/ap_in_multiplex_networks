#ifndef _UTILITY_CPP
#define _UTILITY_CPP


#include "Utility.h"
#include "Cluster.h"
#include "Cluster_BFS.h"



void InitializeTotalCosts (NETWORK &network1, NETWORK &network2, const string costType)
{

    if("UNIT"==costType)
    {
        network1.totalCost=(double)(network1.numNodes);
        network2.totalCost=(double)(network2.numNodes);
    }
    else if("DEGREE"==costType)
    {
        network1.totalCost=(double)(network1.numLinks);
        network2.totalCost=(double)(network2.numLinks);
    }
    else if("WEIGHT"==costType)
    {
        network1.totalCost=network2.totalCost=0.0;

        for(lint i=ZERO ; i<network1.numNodes; i++)
            network1.totalCost+=network1.node[i].cost;

        for(lint i=ZERO ; i<network2.numNodes; i++)
            network2.totalCost+=network2.node[i].cost;
    }
}



void PrintMCCDistribution(const NETWORK &network, FILE *file, const lint removed)
{
    lint i, N, size;
    vector <lint> dist(1+network.gccSize);
    set <lint> ids;

    N=0;

    for(i=0 ; i<network.numComponents ; i++)
    {
        size=network.componentSize[i];
        ids.insert(size);
        dist[size]++;
        N+=size;
    }

    if(N!=network.numNodes-removed)
    {
        printf("\n\nERROR:: total size of components do not match with number of nodes.\n\n");
        exit(1);
    }

    set<lint>::iterator it;
    for(it=ids.begin() ; it!=ids.end() ; it++)
        fprintf(file, "%ld %ld\n", *it, dist[*it]);
}


void UpdateAllDegrees(NETWORK &network1, NETWORK &network2)
{
    lint degree1, degree2;

    for (lint i=ZERO ; i<network1.numNodes ; i++)
    {
        if (PRESENT == network1.node[i].state)
        {
            degree1=degree2=ZERO;

            for (lint j=ZERO ; j<network1.degree[i] ; j++)     
                if (network1.componentID[i] == network1.componentID[network1.node[i].link[j].target])
                    degree1++;

            for (lint j=ZERO ; j<network2.degree[i] ; j++)   
                if (network2.componentID[i] == network2.componentID[network2.node[i].link[j].target]) 
                    degree2++;

            network1.node[i].degree=degree1;
            network2.node[i].degree=degree2;
        }
    }
}


void UpdateAllDegrees_EI(NETWORK &network1, NETWORK &network2)
{
    lint degree1, degree2;

    for (lint i=ZERO ; i<network1.numNodes ; i++)
    {
        if (PRESENT == network1.node[i].state)
        {
            degree1=degree2=ZERO;

            for (lint j=ZERO ; j<network1.degree[i] ; j++)   
                if(findClusterRoot(i, network1.ptr) == findClusterRoot(network1.node[i].link[j].target, network1.ptr))  
                    degree1++;

            for (lint j=ZERO ; j<network2.degree[i] ; j++)   
                if(findClusterRoot(i, network2.ptr) == findClusterRoot(network2.node[i].link[j].target, network2.ptr))  
                    degree2++;

            network1.node[i].degree=degree1;
            network2.node[i].degree=degree2;
        }
    }
}


void UpdateGMCCDegrees(NETWORK &network1, NETWORK &network2)
{
    lint degree1, degree2;

    for (lint i=ZERO ; i<network1.numNodes ; i++)
    {
        if (network1.gccRoot == network1.componentID[i])
        {
            degree1=degree2=ZERO;

            for (lint j=ZERO ; j<network1.degree[i] ; j++)      
                if (network1.gccRoot == network1.componentID[network1.node[i].link[j].target])
                    degree1++;

            for (lint j=ZERO ; j<network2.degree[i] ; j++)                     
                if (network2.gccRoot == network2.componentID[network2.node[i].link[j].target]) 
                    degree2++;

            network1.node[i].degree=degree1;
            network2.node[i].degree=degree2;
        }
    }
}


double OverlappingFraction (const NETWORK &network1, const NETWORK &network2)
{
    lint i, j, k, n, overlap;
    set <lint> nodes;
    overlap=ZERO;


    for(i=ZERO ; i<network1.numNodes ; i++)
    {
        for (j=ZERO ; j<network1.degree[i] ; j++)
        {
            n = network1.node[i].link[j].target;

            if(i>n)
            {
                for (k=ZERO ; k<network2.degree[i] ; k++)
                {
                    if (network2.node[i].link[k].target == n)
                    {
                        overlap++;
                        nodes.insert(i);
                        nodes.insert(n);
                        break ;
                    }

                }
            }
            
        }
    }


    i=(network1.numLinks<network2.numLinks)?network1.numLinks:network2.numLinks;

    printf("\nNumber of links\n%s: %ld \n%s: %ld", 
        network1.name.c_str(), network1.numLinks, network2.name.c_str(), network2.numLinks);
    printf("\nOverlapping fraction: %lf", (double)overlap/(double)i);
    printf("\nNumber of nodes with shared links (overlapping): %ld\n", nodes.size());

    return (double)overlap/(double)i;    
}


void ActivateAllNodesLinks (NETWORK &network)
{
    lint i, j;

    for(i=ZERO ; i<network.numNodes ; i++)
    {
        network.node[i].state = PRESENT;

        for (j=ZERO ; j<network.degree[i] ; j++)
            network.node[i].link[j].state = PRESENT;
    }
}


void CreateLink (NETWORK &network, lint &source, lint &target)
{

    lint i, size;

    LINK link;
    link.state = PRESENT;

    bool state = true;

    size = network.node[source].link.size();


    for (i=ZERO ; i<size ; i++)
    {

        if (network.node[source].link[i].target == target)
        {
            state = false;
            break;
        }
    }


    if (state)
    {
        link.target = target;
	    network.node[source].link.push_back(link);
    }
    else
    {
    	printf("\n\nMulti-link detected in aggregated or relabeled network! \n\n");
    	//exit(1);
    }
    
}


void RandomizeLabels(const NETWORK &network, NETWORK &networkRandomized, const ulint seed, const double randomization)
{

	lint i, j, source, target, id, temp;
    vector <lint> labels (network.numNodes);


    printf("\nRandomization SEED : %ld \n", seed+1);
    class MTRand *RandNumb = new MTRand((ulint)seed+1);


    // set number of nodes and allocate memory for randomized network.
    networkRandomized.numNodes = network.numNodes;
    networkRandomized.AllocateMemory();


    for (i=ZERO ; i<network.numNodes ; i++)
        labels[i] = i;


    // do randomization of labels.
    for (i=ZERO ; i<network.numNodes ; i++)
    {
        if (RandNumb->randExc()<randomization)
        {
            id = RandNumb->randInt(network.numNodes-ONE);

            // exchange labels.
            temp = labels[i];
            labels[i] = labels[id];
            labels[id] = temp;
        }

    }
    

    // copy network (randomized version) to networkRandomized
    for (i=ZERO ; i<network.numNodes ; i++)
    {
    	networkRandomized.node[labels[i]].id=i;

        source = i;

        for (j=ZERO ; j<network.degree[i] ; j++)
        {
            target = network.node[i].link[j].target;
            
            CreateLink(networkRandomized, labels[source], labels[target]);
        }
    }


    networkRandomized.BuildDegreeVector();
    ActivateAllNodesLinks(networkRandomized);


    free (RandNumb); 


    // check correctness.
    for(i=ZERO ; i<network.numNodes ; i++)
    {
    	id=labels[i];

    	if(network.degree[i] != networkRandomized.degree[id])
    	{
    		printf("\n\nError:: randomization failed!");
    		exit(1);
    	}

    	vector <lint> n1,n2;
    	lint k;

    	for(j=ZERO ; j<network.degree[i] ; j++)
    	{
    		n1.push_back(network.node[i].link[j].target);
    		k=networkRandomized.node[id].link[j].target;
    		n2.push_back(networkRandomized.node[k].id);
    	}

    	sort(n1.begin(), n1.end(), Compare);
    	sort(n2.begin(), n2.end(), Compare);

    	for(j=ZERO ; j<network.degree[i] ; j++)
    	{
    		if(n1[j]!=n2[j])
    		{
    			printf("\n\nError:: randomization labels do nut match! \n\n");
    			exit(1);
    		}
    	}
    		
    }
    
}


void AggregateLayers (const NETWORK &network1, const NETWORK &network2, NETWORK &networkAggregated)
{
    
    lint i, j, target;


    networkAggregated.numNodes = network1.numNodes;
    networkAggregated.AllocateMemory();


    for(i=ZERO ; i<network1.numNodes ; i++)
    {
        for (j=ZERO ; j<network1.degree[i] ; j++)
        {
            target = network1.node[i].link[j].target ;
            CreateLink(networkAggregated, i, target);
        }
    }


    for(i=ZERO ; i<network2.numNodes ; i++)
    {
        for (j=ZERO ; j<network2.degree[i] ; j++)
        {
            target = network2.node[i].link[j].target ;
            CreateLink(networkAggregated, i, target);
        }
    }

    networkAggregated.BuildDegreeVector ();
}


void PermutationCommunityNodeList (vector <lint> &nodeList, class MTRand *RandNumb)
{

    lint i, j, temp, N;
    N = nodeList.size();


    for (i=ZERO ; i<N ; i++)
    {
        j = RandNumb->randInt(N-ONE);

        temp = nodeList[i];
        nodeList[i] = nodeList[j];
        nodeList[j] = temp;
    }

}


void PermutationNodeList (vector <lint> &nodeList, class MTRand *RandNumb)
{

    lint i, j, temp, N;
    N = nodeList.size();


    for (i=ZERO ; i<N ; i++)
    {
        j = i+(N-i)*RandNumb->randExc() ;

        temp = nodeList[i] ;

        nodeList[i] = nodeList[j] ;

        nodeList[j] = temp ;
    }

}


void PermutationNodeScorePair (vector < pair<lint, lint> > &nodeScorePair, class MTRand *RandNumb)
{

    lint i, j, N;
    N = nodeScorePair.size();
    pair<lint, lint> temp;


    for (i=ZERO ; i<N ; i++)
    {
        j = i+(N-i)*RandNumb->randExc();

        temp = nodeScorePair[i];

        nodeScorePair[i] = nodeScorePair[j];

        nodeScorePair[j] = temp;
    }

}


void PermutationNodeScorePairDouble (vector < pair<lint, double> > &nodeScorePair, class MTRand *RandNumb)
{

    lint i, j, N;
    N = nodeScorePair.size();
    pair<lint, double> temp;


    for (i=ZERO ; i<N ; i++)
    {
        j = i+(N-i)*RandNumb->randExc();

        temp = nodeScorePair[i];

        nodeScorePair[i] = nodeScorePair[j];

        nodeScorePair[j] = temp;
    }

}


bool ComparePair (const pair<lint,lint> &a, const pair<lint,lint> &b)
{
    return a.second > b.second ;
}


bool ComparePairDouble (const pair<lint,double> &a, const pair<lint,double> &b)
{
    return a.second > b.second ;
}


bool Compare (const lint &a, const lint &b)
{
    return a < b;
}



/**************************************************************************************/

/*
void GetUserInputs(int argc, char* argv[], PARAMETERS &par)
{
    int id;


    if(argc<6)
    {
        printf("\n input:: network1 coords1 network2 coords2 seed\n");
        exit(1);
    }

    par.networkName1=argv[1];
    par.coordsName1=argv[2];
    par.networkName2=argv[3];
    par.coordsName2=argv[4];
    par.seed=ulint(atoi(argv[5]));
    

    id=ReturnKeyWordValue(argc, argv, "--randomization");
    if(-1 != id)
        par.randomization=atof(argv[id]);
        

    id=ReturnKeyWordValue(argc, argv, "--decimation");
    if(-1 != id)
        par.decimation=atof(argv[id]);


    id=ReturnKeyWordValue(argc, argv, "--networkAddress");
    if(-1 != id)
        par.networkAddress=argv[id];
        

    id=ReturnKeyWordValue(argc, argv, "--outputAddress");
    if(-1 != id)
        par.outputAddress=argv[id];


    printf("\nNetwork1 name: %s\n", par.networkName1.c_str());
    printf("Network1 coords: %s\n", par.coordsName1.c_str());
    printf("Network2 name: %s\n", par.networkName2.c_str());
    printf("Network2 coords: %s\n", par.coordsName2.c_str());
    printf("networkAddress:: %s\n", par.networkAddress.c_str());
    printf("outputAddress:: %s\n", par.outputAddress.c_str());
    printf("seed:: %ld\n", par.seed);
    printf("randomization:: %lf\n", par.randomization);
    printf("decimation:: %lf\n", par.decimation);

}   
*/

int ReturnKeyWordValue(int argc, char* argv[], string key)
{
    int value=-1;

    for (int i=1; i<argc; i++) 
    {
        if (string(argv[i]) == key) 
        {
            if (i+1 < argc)
            {
                value = i+1;
                break;
            } 
            else 
            { 
                printf("%s option requires one argument.", key.c_str());
                exit(1);
            }  
        } 
    }   

    return value;
}


bool checkKeyWord(int argc, char* argv[], string key)
{
    bool value=false;

    for (int i=1; i<argc; i++) 
    {
        if (string(argv[i]) == key) 
        {
            value=true; 
        } 
    }   

    return value;
}




#endif 
