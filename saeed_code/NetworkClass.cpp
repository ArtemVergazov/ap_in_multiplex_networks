#ifndef _CLASSNETWORK_CPP_
#define _CLASSNETWORK_CPP_

#include "NetworkClass.h"




Network::Network ()
{
    directed = false;
    numLinks      = ZERO;
    numComponents = ZERO;
    gccSize       = ZERO;
    gccRoot       = ZERO;
}


Network::Network (lint N , string net_name)
{
    numNodes = N;
    name = net_name;
    directed = false;
    numLinks      = ZERO;
    numComponents = ZERO;
    gccSize       = ZERO;
    gccRoot       = ZERO;
}


Network::Network (string net_name, string coords_name)
{
    name = net_name;
    coordsName = coords_name;
    directed = false;
    numLinks      = ZERO;
    numComponents = ZERO;
    gccSize       = ZERO;
    gccRoot       = ZERO;
}


Network::Network (string net_name)
{
    name = net_name;
    directed = false;
    numLinks      = ZERO;
    numComponents = ZERO;
    gccSize       = ZERO;
    gccRoot       = ZERO;
}


void Network::AllocateMemory ()
{

    if (ZERO == numNodes)
    {
        cout<<"\nERROR: before Memory allocation, set number of nodes!\n";
        exit(1);
    }

    node.resize		   (numNodes);
    communityID.resize (numNodes);
    r.resize           (numNodes);
    theta.resize       (numNodes);
    degree.resize      (numNodes);
    componentID.resize (numNodes);
    componentSize.resize(numNodes);
    ptr.resize         (numNodes);
    Queue.resize       (numNodes);
    Distance.resize    (numNodes);
}


void Network::FreeMemory ()
{
    lint i;

    for (i=0 ; i<numNodes ; i++)
        node[i].link.clear();

    node.clear();
    communityID.clear();
    r.clear();
    theta.clear();
    degree.clear();
    componentID.clear();
    componentSize.clear();
    ptr.clear();
    Queue.clear();
    Distance.clear();
}


void Network::BuildDegreeVector ()
{
    lint i, edges;
    
    edges = ZERO;

    for (i=ZERO ; i<numNodes ; i++)
	{    
	    degree[i] = node[i].link.size();
	    node[i].degree = degree[i];
	    edges += degree[i] ;
	}

    numLinks = edges/2;
}



#endif 
