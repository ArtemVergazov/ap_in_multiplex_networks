#ifndef _CLUSTER_BFS_CPP_
#define _CLUSTER_BFS_CPP_

/********************************************************************

    For a monoplex network:

        Detect all connected components.

    For a multiplex network:

        Detect all mutually connected components.

********************************************************************/


#include "Cluster_BFS.h"



void DetectAllIsolatedComponents (NETWORK &network)
{

    lint i, j, n, compSize, numComponents, gccSize, gccRoot, neighbor, read_pointer, write_pointer;


    gccSize = numComponents = ZERO;
    gccRoot = UNKNOWN;
    

    // Set component ID of all nodes to unknown
    for (i=0 ; i<network.numNodes ; i++)
        network.componentID[i] = UNKNOWN;


    // Clear component size vector.
    network.componentSize.clear();


    // go through nodes and determine their componentIDs.
    for (i=0 ; i<network.numNodes ; i++)
    {

        // check that current node's label is determined or not.
        if (UNKNOWN == network.componentID[i] && PRESENT == network.node[i].state)
        {

        	// when there's an unknown componentID we have searched for new component.
            numComponents++;

		    // in the beginning component size is one because of node 'i' itself.
		    compSize = ONE;

		    //set componentID of 'i'.
		    network.componentID[i] = numComponents;

		    // Set first element of Queue equal to i.
		    network.Queue[ZERO] = i ;

		    // start reading nodes from first element.
		    read_pointer  = ZERO ;

		    // start writing nodes from second element.
		    write_pointer = ONE ;


		    // go through neighborhood of i node untill all nodes in its component are detected.
		    while(read_pointer != write_pointer)
		    {

		        // read a vertex from Queue to set it's neighbors compID.
		        n = network.Queue[read_pointer];


		        // increase read pointer by one
		        read_pointer++ ;


		        // go through neighbors of n and set compID for nodes that are (PRESENT && NOT determined ID yet).
		        for (j=ZERO ; j<network.degree[n] ; j++)
		        {

		            if(PRESENT == network.node[n].link[j].state)
		            {
		            	// neighboring node
			            neighbor = network.node[n].link[j].target;

			            // is distance determined or it is unknown.
			            if (UNKNOWN == network.componentID[neighbor] && PRESENT == network.node[neighbor].state)
			            {
			                // set neighbors componentID.
			                network.componentID[neighbor] = numComponents;

			                // write neighbor to queue
			                network.Queue[write_pointer] = neighbor;

			                // increase write pointer by one
			                write_pointer++;

			                // increase number of nodes in the component by one
			                compSize++;
			            }

		            }		            

		        }

		    }

            network.componentSize.push_back(compSize);
            if (compSize>gccSize)
            {
                gccSize = compSize;
                gccRoot = numComponents;
            }
            
        }

    }

    // set number of components
    network.numComponents = numComponents;

    network.gccSize = gccSize;
    network.gccRoot = gccRoot;
}


lint DetectAllMutuallyConnectedComponents (NETWORK &networkA, NETWORK &networkB)
{

    lint i, j, target;

    bool condition = true;


    // before doing cascading failures, all links are present.
    for (i=ZERO ; i<networkA.numNodes ; i++)
    {
        for (j=ZERO ; j<networkA.degree[i] ; j++)
            networkA.node[i].link[j].state = PRESENT;

        for (j=ZERO ; j<networkB.degree[i] ; j++)
            networkB.node[i].link[j].state = PRESENT;
    }


    // do percolation process in network A (no need to do percolation in first step to network B.)
    DetectAllIsolatedComponents(networkA);


    // Do Cascading Failure.
    while (condition)
    {

        condition = false;


        // B-Links that Connect B-nodes who are DEPENDENT to Separate A-Clusters are Removed.
        for (i=ZERO ; i<networkB.numNodes ; i++)
        {
            // check that node is active or not.
            if(PRESENT == networkB.node[i].state)
            {

                for (j=ZERO ; j<networkB.degree[i] ; j++)
	            {

	                // neighboring node.
	                target = networkB.node[i].link[j].target;


	                // check that link or node is active or not.
	                if(PRESENT == networkB.node[i].link[j].state && PRESENT == networkB.node[target].state)
	                {
	                    // check dependent nodes in A network.
		                if (networkA.componentID[i] != networkA.componentID[target])
		                {
		                    // deactivate the link.
		                    networkB.node[i].link[j].state = ABSENT;
		                    condition = true;
		                }
	                }
	            }
            }       
        }


        DetectAllIsolatedComponents (networkB);


        // A-Links that Connect A-nodes who are DEPENDENT to Separate B-Clusters are Removed.
        for (i=ZERO ; i<networkA.numNodes ; i++)
        {
            // check that node is active or not.
            if(PRESENT == networkA.node[i].state)
            {

                for (j=ZERO ; j<networkA.degree[i] ; j++)
	            {

	                // neighboring node.
	                target = networkA.node[i].link[j].target;


	                // check that node is active or not.
	                if(PRESENT == networkA.node[i].link[j].state && PRESENT == networkA.node[target].state)
	                {
	                    // check dependent nodes in B network.
		                if (networkB.componentID[i] != networkB.componentID[target])
		                {
		                    // deactivate the link.
		                    networkA.node[i].link[j].state = ABSENT;
		                    condition = true;
		                }
	                }
	            }
            }
        }


        DetectAllIsolatedComponents (networkA);


    }


    //void CheckCorrectnessOfDetectingMCC (NETWORK &networkA, NETWORK &networkB);
    //CheckCorrectnessOfDetectingMCC (networkA, networkB);


    if (networkA.gccSize != networkB.gccSize)
    {
    	printf("\n\nERROR in detecting Mutually Connected Componenets. \n\n");
        exit(1);
    }

    return networkA.gccSize;

}


void CheckCorrectnessOfDetectingMCC (NETWORK &networkA, NETWORK &networkB)
{

    lint i, j, k, target;

    for (i=ZERO ; i<networkB.numNodes ; i++)
    {

        for (j=ZERO ; j<networkA.degree[i] ; j++)
        {
            // neighboring node.
            target = networkA.node[i].link[j].target;
            
            for (k=ZERO ; k<networkA.degree[target] ; k++)
            {
                if (i == networkA.node[target].link[k].target)
                {
                    if (networkA.node[i].link[j].state != networkA.node[target].link[k].state)
                    {
                        printf("\n\nERROR in detecting Mutually Connected Componenets. \n\n");
                        exit(1);
                    }

                    break;
                }
            }      

        }

        for (j=ZERO ; j<networkB.degree[i] ; j++)
        {
            // neighboring node.
            target = networkB.node[i].link[j].target;
            
            for (k=ZERO ; k<networkB.degree[target] ; k++)
            {
                if (i == networkB.node[target].link[k].target)
                {
                    if (networkB.node[i].link[j].state != networkB.node[target].link[k].state)
                    {
                        printf("\n\nERROR in detecting Mutually Connected Componenets. \n\n");
                        exit(1);
                    }
                }
            }      

        }

    }

}




#endif