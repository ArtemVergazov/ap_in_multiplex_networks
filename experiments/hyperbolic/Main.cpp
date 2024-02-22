#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <time.h>
#include <list>

//include <windows.h>

#include <limits> // for numeric_limits

#include <set>
#include <utility> // for pair
#include <algorithm>
#include <iterator>
#include <cmath>
#include <math.h>
#include <numeric> // for accumulate

using namespace std;

#include "RNG_MT.h"

typedef long int lint;

const double PI=3.141592653589793;



void ReadCoordinates(vector<double> &kappa, vector<double> &theta, FILE *file)
{
    lint id;
    double r, th, ka;
    char line[100];

    while (fgets(line, 100, file) != NULL)
    {
        sscanf(line, "%ld %lf %lf %lf",&id, &r, &th, &ka);

        if ( (id>=0) && (r>=0.0) && (th>=0.0) && (ka>=0.0) )
        {
            kappa.push_back(ka);
            theta.push_back(th);

            //printf("%lf %lf\n",ka,th);
        }
        else
        {
            printf("\n\nError in Reading Network: incorrect ID or self-edge!\n\n");
            exit(1);
        }
    }
}


void MakeLinks(const lint N, const double kbar, const double T, vector<double> &kappa,vector<double> &theta, class MTRand *RandNumb, char *name)
{
    lint i, j;
    double mu, dTheta,r,twoPI,invT;


    mu=sin(T*PI)/(2.0*kbar*T*PI);

    char fileName[100];
    sprintf(fileName, "links.%s", name);
    FILE *file=fopen(fileName,"w");
    

    twoPI = 2.0*PI;
    invT = 1.0/T;

    
    for(i=0 ; i<N-1 ; i++)
    {
        for(j=i+1 ; j<N ; j++)
        {
            dTheta=N/(twoPI)*abs(PI-abs(PI-abs(theta[i]-theta[j])));

            r=1.0/(1.0+pow((dTheta/(mu*kappa[i]*kappa[j])),invT) );

            if (RandNumb->randExc()<r)
            {
                fprintf(file, "%ld %ld\n", i,j);  
            }
        }
    }

    fclose(file);
}



int main(int argc, char* argv[])
{

    double time_begin=time(NULL);
    printf("\n\n############ Get in C++! ############\n");
    //############################//

    lint seed, N;
    double kbar, T;



    if(argc<6)
    {
        printf("\nUsage: coordinates.txt seed N kbar T \n");
        exit(1);
    }
    
    seed = atoi(argv[2]);
    N = atoi(argv[3]);
    kbar = atof(argv[4]);
    T = atof(argv[5]);


    cout<<"seed:"<<seed<<"\tN:"<<N<<"\tkbar:"<<kbar<<"\tT:"<<T<<endl;


    FILE *file = fopen(argv[1],"r");
    class MTRand *RandNumb = new MTRand((unsigned long int)seed);

    vector <double> kappa;
    vector <double> theta;

    ReadCoordinates(kappa,theta,file);
    
    MakeLinks(N,kbar,T,kappa,theta,RandNumb, argv[1]);



    fclose(file); 

    //############################//
    double time_end=time(NULL);
    printf ( "RUN TIME: %2lf \n",time_end-time_begin);
    printf("############ Get out of C++! ############\n\n");
    return 0;
}




