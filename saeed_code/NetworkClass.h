#ifndef _CLASSNETWORK_H_
#define _CLASSNETWORK_H_



#include "Header.h"



typedef class Link
{
    public :

    lint id;
    lint target;     
    STATE state;              

} LINK;


typedef class Node
{
    public :

    lint id;                
    lint degree;           
    vector <LINK> link;     
    STATE state;    
    double cost;      

} NODE ;


typedef class Network
{

    public :

    string name;
    string coordsName;   
    bool directed;
    lint numNodes;
    lint numLinks;      
    vector <NODE> node;    
    vector <lint> communityID;
    vector <double> r;
    vector <double> theta;
    vector <lint> degree;
    double totalCost;

    /* BFS and Newman-Ziff component variables: For shortest path and Component structure */
    vector <lint> componentID ;    
    vector <lint> componentSize ;    
    vector <lint> ptr ;          
    vector <lint> Queue;
    vector <lint> Distance;    
    lint numComponents ;             
    lint gccSize ;                  
    lint gccRoot ;                   

    Network();
    Network(lint N , string net_name);
    Network(string net_name);
    Network(string net_name, string coords_name);
    void AllocateMemory();
    void FreeMemory();
    void BuildDegreeVector();

} NETWORK ;




#endif 
