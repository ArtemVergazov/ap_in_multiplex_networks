/********************************************************************
		Articulation Points of multiplex networks
   						saeedosat13@gmail.com
********************************************************************/


#include "Header.h"
#include "NetworkClass.h"
#include "NetworkReader.h"
#include "Cluster.h"
#include "Cluster_BFS.h"
#include "Utility.h"
#include "NetworkModels.h"


#include "SP.h"
#include "HDA.h"
#include "EMD.h"
#include "CI.h"
#include "EI.h"
#include "CoreHD.h"
#include "AP.h"


int KeepGMCC (int argc, char* argv[]);
void FIG1 (int argc, char* argv[]);
void RealNetwork (int argc, char* argv[]);
void FIG2 (int argc, char* argv[]);



int main (int argc, char* argv[])
{

	clock_t begin = clock();
    cout<<endl<<"***********************************************************"<<endl;



    if(argc<2 || checkKeyWord(argc, argv, "--help"))
    {
        printf("\nFirst you should specify one of the the following processes:\n\n \
        	      AttackRealMultiplexNetworks\n \
        	      AttackRandomMultiplexNetworks\n\n \
        	      For further help of processes type: process_name --help \n\n");
        exit(1);
    }
	else if(string(argv[1]) == "KeepGMCC")
	    KeepGMCC(argc, argv);
	else if(string(argv[1]) == "FIG1")
	    FIG1(argc, argv);
	else if(string(argv[1]) == "REAL")
	    RealNetwork(argc, argv);
	/*
	else if(string(argv[1]) == "FIG2")
	    FIG2(argc, argv);
	else if(string(argv[1]) == "AttackRealMultiplexNetworks")
		AttackRealMultiplexNetworks(argc, argv);
	else if(string(argv[1]) == "RandomMultiplex")
		AttackRandomMultiplexNetworks(argc, argv);
	*/
	else
	{
		printf("\n\nPrint --help for more information! \n\n");
		exit(1);
	}



    clock_t end = clock();
    double time_spent = (double)(end-begin)/CLOCKS_PER_SEC;
    printf ( "\n****************     RUN TIME %2lf     ****************\n\n" , time_spent);

    return 0 ;

}



void FIG1 (int argc, char* argv[])
{

	// ***** Get user inputs ***** //
	int id=2;
	lint N=atoi(argv[id++]);
   	string outputAddress=argv[id++];
	// ************************** //

   	char outputFileName[FILE_NAME_LENGTH];
    sprintf(outputFileName, "%s/AP-K.txt", outputAddress.c_str());
    FILE *file = fopen(outputFileName, "w");
    if (NULL==file)
    {
        printf("\n\nError in openning the file: %s \n\n", outputFileName);
        exit(1);
    }


    double kmin=1.0, kmax=5.0;
	double ap, k=kmax;
	ulint seed=ONE;


	while (k>=kmin)
	{

	    NETWORK network1 = NETWORK ("POISSON1");
		NETWORK network2 = NETWORK ("POISSON2");

	    CreateConfigurationModelPoissonNetwork (network1, N, k, seed+seed-ONE);
  		CreateConfigurationModelPoissonNetwork (network2, N, k, seed+seed);

  		ActivateAllNodesLinks(network1);
  		ActivateAllNodesLinks(network2);

		seed++;

        ap=(double)NumberOfArticulationPoints(network1, network2)/(double)network1.numNodes;


        fprintf(file, "%lf \t %lf \n", k, ap);
        printf("<k> : %lf \t S : %lf \n", k, ap);


        network1.FreeMemory();
        network2.FreeMemory();

        k -= 0.05;
    }


	fclose(file);

}


/*
int AttackRealMultiplexNetworks (int argc, char* argv[])
{

	// ***** Get user inputs ***** //

	int numLayers;
	double randomization, alpha, beta;
	ulint seed;
	string outputAddress, strategy;

	alpha=0.4;
	beta=0.5;

    if(argc<8 || checkKeyWord(argc, argv, "--help"))
    {
    	printf("\ninput:: AttackRealMultiplexNetworks\n \
        		  numLayers (links1 links2 ...)\n \
        		  strategy randomization seed outputAddress (optional:--alpha --beta)\n\n");
        exit(1);
    }
    else
    {
    	int id=2;

    	numLayers=atoi(argv[id++]);
    	id=id+numLayers;
    	strategy=argv[id++];
    	randomization=atof(argv[id++]);
	    seed=ulint(atoi(argv[id++]));
       	outputAddress=argv[id++];

		id=ReturnKeyWordValue(argc, argv, "--alpha");
    	if(-1 != id)
        	alpha=atof(argv[id]);

        id=ReturnKeyWordValue(argc, argv, "--beta");
    	if(-1 != id)
        	beta=atof(argv[id]);

	    printf("\nSimulation=%s\nnumLayers=%d\nstrategy=%s\nrandomization=%lf\nseed=%ld\noutputAddress=%s\nalpha=%lf\nbeta=%lf\n\n",
	    	argv[1], numLayers, strategy.c_str(), randomization, seed, outputAddress.c_str(), alpha, beta);
    }

	// **************************** //

    lint it, gmcc;
    double q1, q2, deltaN1, deltaN2;
    pair<lint, double> dN_q;
  	deltaN1=deltaN2=q1=q2=0.0;
  	bool printMCC=false;

    vector<NETWORK> multiplex;
	ReadMultiplexNetwork(multiplex,argc, argv);


	class MTRand *RandNumb = new MTRand(seed);

	for(int layer=0 ; layer<numLayers ; layer++)
	{
		DetectAllIsolatedComponents(multiplex[layer]);
		printf("layer: %d \t gcc: %ld \n", layer+1, multiplex[layer].gccSize);
	}

	gmcc=DetectAllMutuallyConnectedComponents(multiplex[0], multiplex[1]);
	printf("gmcc: %ld\n",gmcc);


  	NETWORK networkRandomized("randomized");
  	RandomizeLabels(multiplex[0], networkRandomized, seed, 1.0);


  	for (it=ZERO ; it<100 ; it++)
  	{
  		if(it<10)
  			dN_q=MultiplexDismantling_PhaseDiagram(multiplex[0], multiplex[1], RandNumb,
  												   strategy, it, outputAddress, alpha, beta, printMCC);
  		else
  			dN_q=MultiplexDismantling(multiplex[0], multiplex[1], RandNumb,
  									  strategy, it, outputAddress, alpha, beta, printMCC);
  		deltaN1+=(double)dN_q.first;
  		q1+=dN_q.second;
  	}

  	for (it=ZERO ; it<100 ; it++)
  	{
  		if(it<10)
  			dN_q=MultiplexDismantling_PhaseDiagram(networkRandomized, multiplex[1], RandNumb,
  												   strategy, it, outputAddress, alpha, beta, printMCC);
  		else
  			dN_q=MultiplexDismantling(networkRandomized, multiplex[1], RandNumb,
  									  strategy, it, outputAddress, alpha, beta, printMCC);
  		deltaN2+=(double)dN_q.first;
  		q2+=dN_q.second;
  	}

  	q1=q1/100.0;
  	q2=q2/100.0;
  	deltaN1=deltaN1/100.0;
  	deltaN2=deltaN2/100.0;


  	printf("\nq1=%lf \t deltaN1=%lf \t q2=%lf \t deltaN2=%lf \n\n", q1, deltaN1, q2, deltaN2);

  	char outputFileName[FILE_NAME_LENGTH];
  	sprintf(outputFileName, "%s/DeltaN_q_%s._%s._%s.gmcc", outputAddress.c_str(), strategy.c_str(),
        multiplex[0].name.c_str(), multiplex[1].name.c_str());
    FILE *file = fopen(outputFileName, "w");
    fprintf(file, "%lf \t %lf\n%lf \t %lf\n", q1, deltaN1, q2, deltaN2);
    fclose(file);


	for(int layer=0 ; layer<numLayers ; layer++)
		multiplex[layer].FreeMemory();

	free (RandNumb);


    return 0 ;
}


void AttackRandomMultiplexNetworks (int argc, char* argv[])
{

	//####### Get user inputs ########//
	int numLayers;
	double randomization, alpha, beta;
	ulint seed;
	string model, outputAddress, strategy;
	vector <int> par;

	alpha=0.4;
	beta=0.5;

    if(argc<8 || checkKeyWord(argc, argv, "--help"))
    {
    	printf("\ninput:: RandomMultiplex\n \
        		  numLayers MODEL MODEL_PARAMETERS (ex. poisson N <K>)\n \
        		  strategy randomization seed outputAddress (optional:--alpha --beta)\n\n");
        exit(1);
    }
    else
    {
    	int id=2;
    	numLayers=atoi(argv[id++]);
    	model=argv[id++];
    	if ("poisson"==model)
    	{
    		par.push_back(id++);
    		par.push_back(id++);
    	}
    	strategy=argv[id++];
    	randomization=atof(argv[id++]);
	    seed=ulint(atoi(argv[id++]));
       	outputAddress=argv[id++];

		id=ReturnKeyWordValue(argc, argv, "--alpha");
    	if(-1 != id)
        	alpha=atof(argv[id]);

        id=ReturnKeyWordValue(argc, argv, "--beta");
    	if(-1 != id)
        	beta=atof(argv[id]);

	    printf("\nSimulation=%s\nnumLayers=%d\nstrategy=%s\nrandomization=%lf\nseed=%ld\noutputAddress=%s\nalpha=%lf\nbeta=%lf\n\n",
	    	argv[1], numLayers, strategy.c_str(), randomization, seed, outputAddress.c_str(), alpha, beta);
    }

	// **************************** //

    lint gmcc, deltaN;
    pair<lint, double> dN_q;
    double q;
    bool printMCC=false;


    vector<NETWORK> multiplex;

    multiplex.push_back(NETWORK (model));
    multiplex.push_back(NETWORK (model));
    if("poisson"==model)
    {
    	CreatePoissonNetwork (multiplex[0], atoi(argv[par[0]]), atof(argv[par[1]]), seed);
    	CreatePoissonNetwork (multiplex[1], atoi(argv[par[0]]), atof(argv[par[1]]), seed+1);
    }


	class MTRand *RandNumb = new MTRand(seed+2);


	for(int layer=0 ; layer<numLayers ; layer++)
	{
		DetectAllIsolatedComponents(multiplex[layer]);
		printf("layer: %d \t gcc: %ld \n", layer+1, multiplex[layer].gccSize);
	}


	gmcc=DetectAllMutuallyConnectedComponents(multiplex[0], multiplex[1]);
	printf("gmcc: %ld\n",gmcc);


	InitializeCosts(multiplex[0], multiplex[1], "DEGREE");
  	//dN_q=MultiplexDismantling(multiplex[0], multiplex[1], RandNumb,
  	//						  strategy, seed, outputAddress, alpha, beta, printMCC);
  	deltaN=dN_q.first;
  	q=dN_q.second;


  	printf("\nq=%lf \t deltaN=%ld \n\n", q, deltaN);


	for(int layer=0 ; layer<numLayers ; layer++)
		multiplex[layer].FreeMemory();


	free (RandNumb);
}
*/


int KeepGMCC (int argc, char* argv[])
{
	int numLayers;

    if(argc<3 || checkKeyWord(argc, argv, "--help"))
    {
        printf("\ninput:: KeepGMCC\n \
        		  numLayers (links1 links2 ...)\n \n\n");
        exit(1);
    }
    else
    {
    	numLayers=atoi(argv[2]);
	    printf("\nSimulation=%s\nnumLayers=%d\n\n", argv[1], numLayers);
    }

	// **************************** //

    vector<NETWORK> multiplex;
	ReadMultiplexNetwork(multiplex,argc, argv);


	for(int layer=0 ; layer<numLayers ; layer++)
	{
		DetectAllIsolatedComponents(multiplex[layer]);
		printf("layer: %d \t gcc: %ld \n", layer+1, multiplex[layer].gccSize);
	}

	lint gmcc=DetectAllMutuallyConnectedComponents(multiplex[0], multiplex[1]);
	printf("gmcc: %ld\n",gmcc);


	for(int layer=0 ; layer<numLayers ; layer++)
	{
		char name[1000];
	    sprintf(name, "GMCC_%s", multiplex[layer].name.c_str());
	    FILE *file=fopen(name,"w");

	    for(lint i=ZERO ; i<multiplex[layer].numNodes ; i++)
	    {
	        if(multiplex[layer].gccRoot == multiplex[layer].componentID[i])
	        {
	            for(lint j=ZERO ; j<multiplex[layer].degree[i] ; j++)
	            {
	                lint target=multiplex[layer].node[i].link[j].target;

	                if(multiplex[layer].gccRoot == multiplex[layer].componentID[target])
	                {
	                    if(i<target)
	                        fprintf(file, "%ld %ld\n", multiplex[layer].node[i].id, multiplex[layer].node[target].id);
	                }
	            }
	        }
	    }

	    fclose(file);
	}


	for(int layer=0 ; layer<numLayers ; layer++)
		multiplex[layer].FreeMemory();


    return 0 ;
}



/**********************************************************************************/
/**********************************************************************************/
/************************************ Main Plots  *********************************/
/**********************************************************************************/
/**********************************************************************************/




void RealNetwork (int argc, char* argv[])
{
	// ***** Get user inputs ***** //
	ulint seed;
	double alpha=0.4, beta=0.5;
	string outputAddress, costType;

	int id=2;
	int numLayers=atoi(argv[id++]);
    id=id+numLayers;
   	seed=atoi(argv[id++]);
   	costType=argv[id++];
   	outputAddress=argv[id++];
	// ************************** //

    bool printMCC=false;

    vector<NETWORK> multiplex;
	ReadMultiplexNetwork(multiplex,argc, argv);
    vector <lint> dismantlingSet;


	class MTRand *RandNumb = new MTRand(seed);



	Multiplex_SP   (multiplex[0],
					multiplex[1],
					dismantlingSet,
					RandNumb,
					costType,
					seed,
					outputAddress,
					alpha,
					beta,
					printMCC);

	Multiplex_HDA  (multiplex[0],
					multiplex[1],
					dismantlingSet,
					RandNumb,
					costType,
					seed,
					outputAddress,
					alpha,
					beta,
					printMCC);

	Multiplex_CoreHD   (multiplex[0],
						multiplex[1],
						dismantlingSet,
						RandNumb,
						costType,
						seed,
						outputAddress,
						alpha,
						beta,
						printMCC);

	Multiplex_CI   (multiplex[0],
					multiplex[1],
					dismantlingSet,
					RandNumb,
					costType,
					seed,
					outputAddress,
					alpha,
					beta,
					printMCC);

	Multiplex_EI   (multiplex[0],
					multiplex[1],
					dismantlingSet,
					RandNumb,
					costType,
					seed,
					outputAddress,
					alpha,
					beta,
					printMCC);

	Multiplex_EMD  (multiplex[0],
					multiplex[1],
					dismantlingSet,
					RandNumb,
					costType,
					seed,
					outputAddress,
					alpha,
					beta,
					printMCC);


	for(unsigned int layer=0 ; layer<multiplex.size() ; layer++)
		multiplex[layer].FreeMemory();


	free (RandNumb);
}



/*
void FIG1 (int argc, char* argv[])
{
	// ***** Get user inputs ***** //
	lint N;
	ulint seed;
	double averageDegree, alpha=0.4, beta=0.5;
	string outputAddress, costType;

	int id=2;
	N=atoi(argv[id++]);
	averageDegree=atof(argv[id++]);
   	seed=atoi(argv[id++]);
   	costType=argv[id++];
   	outputAddress=argv[id++];
   	id=ReturnKeyWordValue(argc, argv, "--alpha");
	if(-1 != id)
    	alpha=atof(argv[id]);
    id=ReturnKeyWordValue(argc, argv, "--beta");
	if(-1 != id)
    	beta=atof(argv[id]);
	// ************************** //
    printf("alpha=%lf\n", alpha);
    bool printMCC=false;

    vector<NETWORK> multiplex;
    multiplex.push_back(NETWORK ("POISSON1"));
    multiplex.push_back(NETWORK ("POISSON2"));
   	CreatePoissonNetwork (multiplex[0], N, averageDegree, 1001);
  	CreatePoissonNetwork (multiplex[1], N, averageDegree, 1002);
    vector <lint> dismantlingSet;


	class MTRand *RandNumb = new MTRand(seed);


	Multiplex_SP   (multiplex[0],
					multiplex[1],
					dismantlingSet,
					RandNumb,
					costType,
					seed,
					outputAddress,
					alpha,
					beta,
					printMCC);

	Multiplex_HDA  (multiplex[0],
					multiplex[1],
					dismantlingSet,
					RandNumb,
					costType,
					seed,
					outputAddress,
					alpha,
					beta,
					printMCC);

	Multiplex_CoreHD   (multiplex[0],
						multiplex[1],
						dismantlingSet,
						RandNumb,
						costType,
						seed,
						outputAddress,
						alpha,
						beta,
						printMCC);

	Multiplex_CI   (multiplex[0],
					multiplex[1],
					dismantlingSet,
					RandNumb,
					costType,
					seed,
					outputAddress,
					alpha,
					beta,
					printMCC);

	Multiplex_EI   (multiplex[0],
					multiplex[1],
					dismantlingSet,
					RandNumb,
					costType,
					seed,
					outputAddress,
					alpha,
					beta,
					printMCC);

	Multiplex_EMD  (multiplex[0],
					multiplex[1],
					dismantlingSet,
					RandNumb,
					costType,
					seed,
					outputAddress,
					alpha,
					beta,
					printMCC);


	for(unsigned int layer=0 ; layer<multiplex.size() ; layer++)
		multiplex[layer].FreeMemory();


	free (RandNumb);
}



void FIG2 (int argc, char* argv[])
{
	// ***** Get user inputs ***** //
	lint N;
	ulint seed;
	double gamma, averageDegree, alpha=0.4, beta=0.5;
	string outputAddress, costType;

	int id=2;
	N=atoi(argv[id++]);
	gamma=atof(argv[id++]);
	averageDegree=atof(argv[id++]);
   	seed=atoi(argv[id++]);
   	costType=argv[id++];
   	outputAddress=argv[id++];
   	id=ReturnKeyWordValue(argc, argv, "--alpha");
	if(-1 != id)
    	alpha=atof(argv[id]);
    id=ReturnKeyWordValue(argc, argv, "--beta");
	if(-1 != id)
    	beta=atof(argv[id]);
	// ************************** //

    bool printMCC=false;

    vector<NETWORK> multiplex;
    multiplex.push_back(NETWORK ("STATIC1"));
    multiplex.push_back(NETWORK ("STATIC2"));
   	CreateStaticNetwork (multiplex[0], N, gamma, averageDegree, 101);
  	CreateStaticNetwork (multiplex[1], N, gamma, averageDegree, 102);
    vector <lint> dismantlingSet;


	class MTRand *RandNumb = new MTRand(seed);


	Multiplex_SP   (multiplex[0],
					multiplex[1],
					dismantlingSet,
					RandNumb,
					costType,
					seed,
					outputAddress,
					alpha,
					beta,
					printMCC);

	Multiplex_HDA  (multiplex[0],
					multiplex[1],
					dismantlingSet,
					RandNumb,
					costType,
					seed,
					outputAddress,
					alpha,
					beta,
					printMCC);

	Multiplex_CoreHD   (multiplex[0],
						multiplex[1],
						dismantlingSet,
						RandNumb,
						costType,
						seed,
						outputAddress,
						alpha,
						beta,
						printMCC);

	Multiplex_CI   (multiplex[0],
					multiplex[1],
					dismantlingSet,
					RandNumb,
					costType,
					seed,
					outputAddress,
					alpha,
					beta,
					printMCC);

	Multiplex_EI   (multiplex[0],
					multiplex[1],
					dismantlingSet,
					RandNumb,
					costType,
					seed,
					outputAddress,
					alpha,
					beta,
					printMCC);

	Multiplex_EMD  (multiplex[0],
					multiplex[1],
					dismantlingSet,
					RandNumb,
					costType,
					seed,
					outputAddress,
					alpha,
					beta,
					printMCC);


	for(unsigned int layer=0 ; layer<multiplex.size() ; layer++)
		multiplex[layer].FreeMemory();


	free (RandNumb);
}

*/
