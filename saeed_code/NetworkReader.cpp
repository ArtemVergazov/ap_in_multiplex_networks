#ifndef _NETWORK_READER_CPP_
#define _NETWORK_READER_CPP_




#include "NetworkReader.h"



void ReadNodes (FILE *stream, vector <lint> &nodes, set <lint> &nodeSet)
{

    lint source, target, N;

    char line[LINELENGTH];


    while (fgets(line, LINELENGTH, stream) != NULL)
    {

        source = target = -ONE;

        sscanf(line, "%ld %ld",&source, &target);

        if (nodeSet.insert(source).second)
        {
            nodes.push_back(source);
            N++;
        }
        if (nodeSet.insert(target).second)
        {
            nodes.push_back(target);
            N++;
        }
    }
    
}


lint findNodeID (lint id, vector <lint> &nodes)
{

    lint top, bottom, split, idsplit;

    top = nodes.size();

    if (top<1) 
    {
        return -1;
    }
    
    bottom = 0;
    
    split = top/2;


    do
    {
        idsplit = nodes[split];
        
        if (id>idsplit)
        {
            bottom = split + 1;
            split = (top+bottom)/2;
        }
        else if (id<idsplit)
        {
            top = split;
            split = (top+bottom)/2;
        }
        else
            return split;
    } while (top>bottom);

    return -1;
}


void ReadLinks (NETWORK &network, FILE *stream, vector <lint> &nodes)
{

    lint i, source, target, numLinks;
    char line[LINELENGTH];

    LINK link;
    link.state=PRESENT;

    numLinks = ZERO;


    for (i=ZERO ; i<network.numNodes ; i++)
    {
        network.node[i].state=PRESENT;
        network.node[i].degree=ZERO;
        network.node[i].id=nodes[i];
    }


    while (fgets(line, LINELENGTH, stream) != NULL)
    {

        source = target = -ONE;

        sscanf(line, "%ld %ld", &source, &target);
        //printf("\nsource=%ld \t target=%ld \n", source, target);


        source = findNodeID (source, nodes);
        target = findNodeID (target, nodes);


        if ( (source>=ZERO) && (target>=ZERO) && (source != target) )
        {

            // Do not add a link twice.
            for (i=ZERO ; i<network.node[source].degree ; i++)
            {
                if (target == network.node[source].link[i].target)
                {
                    source = -ONE;
                    break;
                }
            }

            if (-ONE != source)
            {

                link.target = target;
                network.node[source].link.push_back(link);
                network.node[source].degree++;

                numLinks++;

                if(!network.directed)
                {
                    link.target = source;
                    network.node[target].link.push_back(link);
                    network.node[target].degree++; 
                }
                
            }
            else
            {
                printf("\n\nError in Reading Network: multi-link %ld \t %ld \t %ld \t %ld \n\n",
                    source, target, nodes[source], nodes[target]);
                //exit(1);
            }
            

        }
        else
        {
            printf("\n\nError in Reading Network: SelfLink %ld \t %ld \t %ld \t %ld \n\n", 
                source, target, nodes[source], nodes[target]);
            //exit(1);
        }       

    }


    network.numLinks = numLinks;
    network.BuildDegreeVector();

}

/*
void ReadMonoplexNetwork (Network &network, const PARAMETERS &par)
{

    printf("\nReading Networks: \nNetwork: %s \n\n", network.name.c_str());


    vector <lint> nodes;
    set <lint> nodeSet;


    char name[FILE_NAME_LENGTH];
    sprintf(name, "%s/%s", par.networkAddress.c_str(), network.name.c_str());
    FILE *file = fopen(name,"rb");
    if (NULL==file)
    {
        printf("\n\nError in openning the file: %s/%s \n\n", par.networkAddress.c_str(), network.name.c_str());
        exit(1);
    }
    

    ReadNodes (file, nodes, nodeSet);
    sort(nodes.begin(), nodes.end(), Compare);
    lint N = nodes.size();


    network.numNodes = N;
    network.AllocateMemory();


    rewind(file);
    ReadLinks(network, file, nodes);


    printf("\n%s \t Number of Nodes: %ld \t Number of Links: %ld \n", 
        network.name.c_str(), network.numNodes, network.numLinks);


    fclose(file);

}


void ReadMultiplexNetwork2 (Network &network1, Network &network2, const PARAMETERS &par)
{

    printf("\nReading Networks: \nNetwork1: %s \nNetwork2: %s \n\n", network1.name.c_str(), network2.name.c_str());


    vector <lint> nodes;
    set <lint> nodeSet;


    char name1[FILE_NAME_LENGTH];
    sprintf(name1, "%s/%s", par.networkAddress.c_str(), network1.name.c_str());
    FILE *file1 = fopen(name1,"rb");
    if (NULL==file1)
    {
        printf("\n\nError in openning the file: %s/%s \n\n", par.networkAddress.c_str(), network1.name.c_str());
        exit(1);
    }


    char name2[FILE_NAME_LENGTH];
    sprintf(name2, "%s/%s", par.networkAddress.c_str(), network2.name.c_str());
    FILE *file2 = fopen(name2,"rb");
    if (NULL==file2)
    {
        printf("\n\nError in openning the file: %s/%s \n\n", par.networkAddress.c_str(), network2.name.c_str());
        exit(1);
    }


    // Real all node Ids in all layers : total N.
    ReadNodes (file1, nodes, nodeSet);
    ReadNodes (file2, nodes, nodeSet);
    sort(nodes.begin(), nodes.end(), Compare);
    lint N = nodes.size();


    network1.numNodes = N;
    network2.numNodes = N;
    network1.AllocateMemory();
    network2.AllocateMemory();


    rewind(file1);
    rewind(file2);

    
    ReadLinks(network1, file1, nodes);
    ReadLinks(network2, file2, nodes);


    printf("\nNumber of Nodes in multiplex Layers: %ld \n", N);

    printf("\n%s \t Number of Nodes: %ld \t Number of Links: %ld \n", 
        network1.name.c_str(), network1.numNodes, network1.numLinks);

    printf("\n%s \t Number of Nodes: %ld \t Number of Links: %ld \n", 
        network2.name.c_str(), network2.numNodes, network2.numLinks);


    fclose(file1);
    fclose(file2);

}
*/


/*********************************************************************/

void ReadMultiplexNetwork (vector <NETWORK> &multiplex, int argc, char* argv[])
{

    lint layer, numLayers;
    vector <lint> nodes;
    set <lint> nodeSet;


    numLayers=atoi(argv[2]);
    printf("\nReading Multiplex: \n\n");
    for(layer=ZERO ; layer<numLayers; layer++)
        printf("Layer %ld :\n%s \n\n", layer+1, argv[layer+3]);
    
    
    // Real all node Ids in all layers : total N.
    for(layer=ZERO ; layer<numLayers ; layer++)
    {
        FILE *file = fopen(argv[layer+3],"rb");
        if (NULL==file)
        {
            printf("\n\nError in openning the file: %s \n\n", argv[layer+3]);
            exit(1);
        }

        ReadNodes(file, nodes, nodeSet);

        fclose(file);
    }
    

    // Allocate Multiplex.
    sort(nodes.begin(), nodes.end(), Compare);
    lint N = nodes.size();
    printf("Number of Nodes of multiplex: %ld\n", N);
    for(layer=ZERO ; layer<numLayers ; layer++)
    {
        NETWORK network = NETWORK((string)argv[layer+3]);
        network.numNodes = N;
        network.AllocateMemory();
        multiplex.push_back(network);
    }

    
    // reading links of all layers.
    for(layer=ZERO ; layer<numLayers ; layer++)
    {
        FILE *file = fopen(argv[layer+3],"rb");
        if (NULL==file)
        {
            printf("\n\nError in openning the file: %s \n\n", argv[layer+3]);
            exit(1);
        }

        ReadLinks(multiplex[layer], file, nodes);
    
        printf("Number of Links of layer %ld: %ld \n", layer+1, multiplex[layer].numLinks);

        fclose(file);
    }


    for(layer=ZERO ; layer<numLayers ; layer++)
        ActivateAllNodesLinks(multiplex[layer]);
}



void ReadHyperbolicMultiplexNetwork (vector <NETWORK> &multiplex, int argc, char* argv[])
{

    lint id, n, layer, numLayers, idr;
    double r, theta, dummy;
    vector <lint> nodes;
    set <lint> nodeSet;


    numLayers=atoi(argv[2]);
    idr=atoi(argv[2*(numLayers+1)+1]);
    printf("\nReading Multiplex: \n\n");
    for(layer=ZERO ; layer<numLayers; layer++)
        printf("Layer %ld :\n%s \n%s\n\n", layer+1, argv[2*(layer+1)+1],argv[2*(layer+2)]);
    
    
    // Real all node Ids in all layers : total N.
    for(layer=ZERO ; layer<numLayers ; layer++)
    {
        FILE *file = fopen(argv[2*(layer+2)],"rb");
        if (NULL==file)
        {
            printf("\n\nError in openning the file: %s \n\n", argv[2*(layer+2)]);
            exit(1);
        }

        n=ZERO;
        char line[LINELENGTH];
        while (fgets(line, LINELENGTH, file) != NULL)
        {
            id=-1;
            sscanf(line, "%ld ",&id);

            n++;

            if(id>0)
            {
                if (nodeSet.insert(id).second)
                    nodes.push_back(id);  
            }
            else
            {
                printf("\n\nError:: negative node id in multiplex! \n\n");
                exit(1);
            }
        }

        printf("Number of Nodes in Layer %ld : %ld\n", layer+1, n);
        fclose(file);
    }
    

    // Allocate Multiplex.
    sort(nodes.begin(), nodes.end(), Compare);
    lint N = nodes.size();
    printf("Number of Nodes of multiplex: %ld\n", N);
    for(layer=ZERO ; layer<numLayers ; layer++)
    {
        NETWORK network = NETWORK((string)argv[2*(layer+1)+1],(string)argv[2*(layer+2)]);
        network.numNodes = N;
        network.AllocateMemory();
        multiplex.push_back(network);
    }


    // Read coordinates of all nodes of all layers.
    for(layer=ZERO ; layer<numLayers ; layer++)
    {
        FILE *file = fopen(argv[2*(layer+2)],"rb");
        if (NULL==file)
        {
            printf("\n\nError in openning the file: %s \n\n", argv[2*(layer+2)]);
            exit(1);
        }
        else
            printf("Read Coordinates : %s \n", argv[2*(layer+2)]);

        n=ZERO;
        char line[LINELENGTH];
        while (fgets(line, LINELENGTH, file) != NULL)
        {
            id=-1;
            r=theta=-1.0;
            sscanf(line, "%ld %lf %lf",&id, &r, &theta);
            
            if(2==idr)
            {
                dummy=r;
                r=theta;
                theta=dummy;
            }

            id=findNodeID(id,nodes);
            if( (id>=0) & (r>-0.1) & (theta>-0.1) )
            {
                multiplex[layer].r[id]=r;
                multiplex[layer].theta[id]=theta;
            }
            else
            {
                printf("\n\nNegative number in multiplex! %ld %lf %lf \n\n", id, r, theta);
                exit(1);
            }
        }

        fclose(file);
    }

    
    // reading links of all layers.
    for(layer=ZERO ; layer<numLayers ; layer++)
    {
        FILE *file = fopen(argv[2*(layer+1)+1],"rb");
        if (NULL==file)
        {
            printf("\n\nError in openning the file: %s \n\n", argv[2*(layer+1)+1]);
            exit(1);
        }

        ReadLinks(multiplex[layer], file, nodes);
    
        printf("Number of Links of layer %ld: %ld \n", layer+1, multiplex[layer].numLinks);

        fclose(file);
    }


    for(layer=ZERO ; layer<numLayers ; layer++)
        ActivateAllNodesLinks(multiplex[layer]);
}



#endif 
