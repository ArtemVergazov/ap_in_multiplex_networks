#ifndef _CLUSTER_CPP_
#define _CLUSTER_CPP_

/********************************************************************
    We use Part of Newman-Ziff algorithm to do :

        For a single network:

            Detect all components and GCC.
            Easy node addition.
            Easy node Removal.

        For an interdependent network:

            Detect all mutually connected components.


        Add a node:

            this step is the same as original node addition in
            Newman-ziff algorithm.


    Developer : Saeed Osat.
********************************************************************/


#include "Cluster.h"



/*****
    find root node of the cluster that node i belongs to it in Site Percolation.
    when ptr[i]<0 it shows it is a root node and although size of its cluster is -ptr[i]
*****/
lint findClusterRoot(lint i, vector <lint> &ptr)
{
    if (ptr[i]<0)
    {
        return i ;
    }

    return ptr[i] = findClusterRoot(ptr[i], ptr) ;
}



/*****
    Add a node to the network and return its cluster size.
*****/
lint AddNode (Network &network, lint node, lint EMPTY)
{

    /* Change node state and ptr. */
    network.node[node].state = PRESENT;



    /* variables. */
    lint i, source, target, source_root, target_root;



    /* Add node and measure component structure. */
    // set 'node' as source node.
    source = node ;
    source_root = node ;


    // adding 'node' to network increases number of components.
    network.numComponents++ ;


    // the new 'node' by itself is a root node of size 1.
    network.ptr[source] = -ONE ;


    // add links of 'node' to network check merging with present neighbors.
    for (i=0 ; i<network.degree[source] ; i++)
    {

        // pick a neighbor of 'node'.
        target = network.node[source].link[i].target ;


        // check target node is present or not or that link is deactivated or not.
        if (EMPTY == network.ptr[target] || ABSENT == network.node[source].link[i].state)
        {
            continue ;
        }
        else
        {

            // find root node of target.
            target_root = findClusterRoot(target,network.ptr) ;


            // if source and target do not point to same root node.
            if (target_root != source_root)
            {

                if (network.ptr[source_root] > network.ptr[target_root])
                {
                    network.ptr[target_root] += network.ptr[source_root] ;

                    network.ptr[source_root] = target_root ;

                    source_root = target_root ;
                }
                else
                {
                    network.ptr[source_root] += network.ptr[target_root] ;

                    network.ptr[target_root] = source_root ;
                }


                // union of components decrease number of components by one.
                network.numComponents-- ;

            }

        }

    }


    // track gcc and save it's root ID
    if (-network.ptr[source_root]>network.gccSize)
    {
        network.gccSize = -network.ptr[source_root];

        network.gccRoot = source_root ;
    }

    return (-network.ptr[source_root]);

}




/*****
    Find gcc and All other components and store their id's.

    return : pair <gccSize,gccRoot>.
*****/
pair<lint,lint> FindAllClusters (Network &network)
{

    /* variables. */
    lint i, EMPTY;

    pair <lint,lint> gcc;

    // set CONST EMPTY.
    EMPTY = -(network.numNodes+(lint)ONE);




    /* Initializations. */
    network.numComponents = ZERO;

    network.gccSize = ZERO ;

    //All nodes are absent(EMPTY) in the beginning.
    for (i=0 ; i<network.numNodes ; i++)
    {
        network.ptr[i] = EMPTY ;
    }



    /* Add nodes and measure component structure. */
    // Add PRESENT nodes one-by-one to the network and measure quantities.
    for (i=0 ; i<network.numNodes ; i++)
    {
        if(PRESENT == network.node[i].state)
        {
            AddNode(network, i, EMPTY);
        }
    }


    gcc.first = network.gccSize;

    gcc.second = network.gccRoot;

    return gcc ;
}




/*****
    Find gmcc and All other mutually connected components.

    return : size of gmcc.
*****/
lint FindGMCC (NETWORK &networkA, NETWORK &networkB)
{

    /* Variables. */
    lint i, j, target, gmcc;

    bool condition = true ;


    // before doing cascading failures, all links are present.
    for (i=ZERO ; i<networkA.numNodes ; i++)
    {
        for (j=ZERO ; j<networkA.degree[i] ; j++)
        {
            networkA.node[i].link[j].state = PRESENT;
        }

        for (j=ZERO ; j<networkB.degree[i] ; j++)
        {
            networkB.node[i].link[j].state = PRESENT;
        }

    }



    // do percolation process in network A (no need to do percolation in first step to network B.)
    FindAllClusters (networkA);




    /* Do Cascading Failure. */
    while (condition)
    {

        condition = false ;


        /* B-Links that Connect B-nodes who are DEPENDENT to Separate a-Clusters are Removed */
        for (i=ZERO ; i<networkB.numNodes ; i++)
        {

            // check that node is active or not.
            if(PRESENT != networkB.node[i].state)
            {
                continue;
            }

            for (j=ZERO ; j<networkB.degree[i] ; j++)
            {

                // neighboring node.
                target = networkB.node[i].link[j].target;


                // check that link or node is active or not.
                if(ABSENT == networkB.node[i].link[j].state || PRESENT != networkB.node[target].state)
                {
                    continue;
                }


                // check dependent nodes in A network.
                if (findClusterRoot(i, networkA.ptr) != findClusterRoot(target, networkA.ptr))
                {

                    // deactivate the link.
                    networkB.node[i].link[j].state = ABSENT;

                    condition = true;

                }

            }

        }

        FindAllClusters (networkB);




        /* A-Links that Connect A-nodes who are DEPENDENT to Separate b-Clusters are Removed */
        for (i=ZERO ; i<networkA.numNodes ; i++)
        {

            // check that node is active or not.
            if(PRESENT != networkA.node[i].state)
            {
                continue;
            }

            for (j=ZERO ; j<networkA.degree[i] ; j++)
            {

                // neighboring node.
                target = networkA.node[i].link[j].target;


                // check that node is active or not.
                if(ABSENT == networkA.node[i].link[j].state || PRESENT != networkA.node[target].state)
                {
                    continue;
                }


                // check dependent nodes in B network.
                if (findClusterRoot(i, networkB.ptr) != findClusterRoot(target, networkB.ptr))
                {

                    // deactivate the link.
                    networkA.node[i].link[j].state = ABSENT;

                    condition = true;

                }

            }

        }

        gmcc = FindAllClusters (networkA).first;


    }


    /*
    lint k;
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
    */


    return gmcc ;

}






#endif // _CLUSTER_CPP_
