#ifndef _AP_CPP_
#define _AP_CPP_




#include "AP.h"




void FindArticulationPointsRecursively (NETWORK &network, lint node, lint &time, vector <lint> &parent,
		 								vector <lint> &lowValue, vector <lint> &discoveryTime,
										vector <bool> &visited, vector <bool> &articulationPoints)
{

    lint i, children=ZERO, neighbor;
    visited[node] = true;
    discoveryTime[node] = lowValue[node] = ++time;


    for (i=ZERO ; i<network.degree[node] ; i++)
    {
    	neighbor = network.node[node].link[i].target;

    	if (PRESENT == network.node[node].link[i].state)
    	{
    		if (false == visited[neighbor])
	    	{
	    		children++;

	    		parent[neighbor]=node;

	    		FindArticulationPointsRecursively (network, neighbor, time, parent, lowValue, discoveryTime, visited, articulationPoints);

	    		lowValue[node] = min (lowValue[node] , lowValue[neighbor]);


	    		if (UNKNOWN == parent[node] && children>ONE)
	    			articulationPoints[node] = true;

	    		if (UNKNOWN != parent[node] && lowValue[neighbor] >= discoveryTime[node])
	    			articulationPoints[node] = true;

	    	}
	    	else if (neighbor != parent[node])
	    		lowValue[node] = min (lowValue[node], discoveryTime[neighbor]);

    	}
    }
}



lint NumberOfArticulationPoints (NETWORK &network1, NETWORK &network2)
{

	lint i, time, num=ZERO;
	double gmcc;


	vector <bool> visited1 			  (network1.numNodes, false);
	vector <bool> visited2 			  (network2.numNodes, false);
	vector <bool> articulationPoints1 (network1.numNodes, false);
	vector <bool> articulationPoints2 (network2.numNodes, false);
	vector <lint> parent1 			  (network1.numNodes, UNKNOWN);
	vector <lint> parent2 			  (network2.numNodes, UNKNOWN);
	vector <lint> lowValue1 	   	  (network1.numNodes);
	vector <lint> lowValue2 	   	  (network2.numNodes);
	vector <lint> discoveryTime1 	  (network1.numNodes);
	vector <lint> discoveryTime2 	  (network2.numNodes);


	gmcc = (double)DetectAllMutuallyConnectedComponents(network1, network2)/(double)network1.numNodes;
	printf("\nSize of GMCC before Finding Articulation Points: %lf \n", gmcc);


	for(i=ZERO ; i<network1.numNodes ; i++)
		if(false == visited1[i])
		{
			time = ZERO;
			FindArticulationPointsRecursively (network1, i, time, parent1, lowValue1, discoveryTime1, visited1, articulationPoints1);
		}


	for(i=ZERO ; i<network2.numNodes ; i++)
		if(false == visited2[i])
		{
			time = ZERO;

			FindArticulationPointsRecursively (network2, i, time, parent2, lowValue2, discoveryTime2, visited2, articulationPoints2);
		}


	for(i=ZERO ; i<network1.numNodes ; i++)
		if (articulationPoints1[i] || articulationPoints2[i])
			num++;

	return num;
}



/***********************************************************************************************************/


/*
void FindArticulationPointsRecursively2 (NETWORK &network, lint node, lint &time, vector <lint> &parent,
		 								 vector <lint> &lowValue, vector <lint> &discoveryTime,
										 vector <bool> &visited, vector <bool> &articulationPoints)
{

    lint i, children, neighbor;

    children = ZERO;

    visited[node] = true;

    discoveryTime[node] = lowValue[node] = ++time;



    for (i=ZERO ; i<network.degree[node] ; i++)
    {

    	neighbor = network.node[node].link[i].target;

    	if (PRESENT == network.node[neighbor].state && PRESENT == network.node[node].link[i].state)
    	{
    		if (false == visited[neighbor])
	    	{
	    		children++;

	    		parent[neighbor] = node;

	    		FindArticulationPointsRecursively2 (network, neighbor, time, parent, lowValue, discoveryTime, visited, articulationPoints);

	    		lowValue[node] = min (lowValue[node] , lowValue[neighbor]);



	    		if (UNKNOWN == parent[node] && children>ONE)
	    		{
	    			articulationPoints[node] = true;
	    		}

	    		if (UNKNOWN != parent[node] && lowValue[neighbor] >= discoveryTime[node])
	    		{
	    			articulationPoints[node] = true;
	    		}

	    	}
	    	else if (neighbor != parent[node])
	    	{
	    		lowValue[node] = min (lowValue[node], discoveryTime[neighbor]);
	    	}

    	}

    }

}



pair <lint, lint> GreedyArticulationPointsRemoval (NETWORK &network1, NETWORK &network2, vector < vector <lint> > &record)
{

	lint i, time, num, timeStep;

	double gmcc;

	num = ONE;

	timeStep = ZERO;

	vector <lint> stepRecord;



	gmcc = (double)DetectAllMutuallyConnectedComponents(network1, network2)/(double)network1.num_node;
	printf("\nSize of GMCC before Finding Articulation Points: %lf \n", gmcc);

	stepRecord.push_back(timeStep);
	stepRecord.push_back(network1.gccSize);



	while (true)
	{

		num = ZERO;



		vector <bool> visited1 			  (network1.num_node, false);
		vector <bool> visited2 			  (network2.num_node, false);
		vector <bool> articulationPoints1 (network1.num_node, false);
		vector <bool> articulationPoints2 (network2.num_node, false);
		vector <lint> parent1 			  (network1.num_node, UNKNOWN);
		vector <lint> parent2 			  (network2.num_node, UNKNOWN);
		vector <lint> lowValue1 	   	  (network1.num_node);
		vector <lint> lowValue2 	   	  (network2.num_node);
		vector <lint> discoveryTime1 	  (network1.num_node);
		vector <lint> discoveryTime2 	  (network2.num_node);




		for(i=ZERO ; i<network1.num_node ; i++)
		{
			if(false == visited1[i] && PRESENT == network1.node[i].state)
			{
				time = ZERO;

				FindArticulationPointsRecursively2 (network1, i, time, parent1, lowValue1, discoveryTime1, visited1, articulationPoints1);
			}
		}




		for(i=ZERO ; i<network2.num_node ; i++)
		{
			if(false == visited2[i] && PRESENT == network2.node[i].state)
			{
				time = ZERO;

				FindArticulationPointsRecursively2 (network2, i, time, parent2, lowValue2, discoveryTime2, visited2, articulationPoints2);
			}
		}




		for(i=ZERO ; i<network1.num_node ; i++)
		{
			if (articulationPoints1[i] || articulationPoints2[i])
			{
				network1.node[i].state = ABSENT;
				network2.node[i].state = ABSENT;

				num++;
			}
		}




		stepRecord.push_back(num);

		printf("\n t=%ld \t n_ap=%lf \t gmcc=%lf", timeStep, (double)num/(double)network1.num_node,
												   (double)network1.gccSize/(double)network1.num_node);




		if (ZERO == num)
		{
			break;
		}




		timeStep++;

		DetectAllMutuallyConnectedComponents(network1, network2);

		stepRecord.push_back(timeStep);

		stepRecord.push_back(network1.gccSize);

	}


	record.push_back(stepRecord);


	return make_pair(network1.gccSize, timeStep);
}


#ifdef POISSON_NETWORK
void GreedyArticulationPointsRemovalAverageDegree()
{

	// output file .
	char outputFileName[FILE_NAME_LENGTH];
	sprintf(outputFileName, "%s/GAPR.dat", GoutputAddress);

	FILE *file = fopen(outputFileName, "w");




	lint i, t, row, column;

	pair <lint, lint> gmcc_time;

	double gmcc, AveDegree;

	ulint seed, netSeed1, netSeed2;

	vector < vector <lint> > record;

	vector <double> meanDegree;

	AveDegree = GmaxDegree;

	seed = ONE;




	POISSON_PARAMETERS parameters;
	parameters.num_node = GnumberNodes;




	while (AveDegree>=GminDegree)
	{

	    // network creation seeds.
		netSeed1 = seed+seed-ONE;
		netSeed2 = netSeed1+ONE;

		seed++;

		char networkName1[FILE_NAME_LENGTH], networkName2[FILE_NAME_LENGTH];
		sprintf(networkName1, "Network_%ld",netSeed1);
		sprintf(networkName2, "Network_%ld",netSeed2);

		NETWORK network1 = NETWORK (networkName1);
		NETWORK network2 = NETWORK (networkName2);

		network1.directed = false;
		network2.directed = false;

		//NETWORK networkRandomized = NETWORK ("Randomized Network");
	    //NETWORK networkAggregated = NETWORK ("Aggregated Network");

	    parameters.mean_degree = AveDegree;

	    parameters.seed = netSeed1;
	    //CreatePoissonNetwork (network1, parameters);
	    CreateConfigurationModelPoissonNetwork(network1, parameters);

	    parameters.seed = netSeed2;
	    //CreatePoissonNetwork (network2, parameters) ;
	    CreateConfigurationModelPoissonNetwork(network2, parameters);



	    for (i=ZERO ; i<network1.num_node ; i++)
	    {
	    	network1.node[i].state = PRESENT;
			network2.node[i].state = PRESENT;
	    }


        // Find number of articulation points.
        gmcc_time = GreedyArticulationPointsRemoval(network1, network2, record);
        gmcc =  (double)gmcc_time.first/(double)network1.num_node;

        // print results to file.
        fprintf(file, "%lf \t %lf \t %ld\n", AveDegree, gmcc, gmcc_time.second);
        printf("<k> : %lf \t S : %lf \t T : %ld\n", AveDegree, gmcc, gmcc_time.second);



        // Decrease Average Degree.
        meanDegree.push_back(AveDegree);
        AveDegree -= 0.05;



        network1.FreeMemory();
        network2.FreeMemory();

    }


    fclose(file);





    row = record.size();

    for (t=ZERO ; t<=Gtime; t++)
    {

    	char outputFileName[FILE_NAME_LENGTH];
		sprintf(outputFileName, "%s/Time_%ld.dat", GoutputAddress, t);

		file = fopen(outputFileName, "w");



		for (i=ZERO ; i<row ; i++)
		{

			// print average degree.
			fprintf(file, "%lf \t", (double)meanDegree[i]);

			column = record[i].size();

			column /=3;

			if (column>t)
			{
				column = (t*3)+1;

				fprintf(file, "%lf \t %lf \n", (double)record[i][column]/(double)GnumberNodes,
											   (double)record[i][column+1]/(double)GnumberNodes);
			}
			else
			{
				column = record[i].size();

				fprintf(file, "%lf \t %lf \n", (double)record[i][column-2]/(double)GnumberNodes,
											   (double)record[i][column-1]/(double)GnumberNodes);
			}

		}



		fclose(file);
    }







    // print time evolution of the GAPR process.
    for (i=ZERO ; i<row ; i++)

    {

    	char outputFileName[FILE_NAME_LENGTH];
		sprintf(outputFileName, "%s/TimeEvolution_%lf.dat", GoutputAddress, meanDegree[i]);

		file = fopen(outputFileName, "w");



		column = record[i].size();

		//column /=3;

		for (t=ZERO ; t<column; t=t+3)
		{
			fprintf(file, "%ld \t %lf \t %lf \n", record[i][t],
												  (double)record[i][t+1]/(double)GnumberNodes,
											      (double)record[i][t+2]/(double)GnumberNodes);
		}


		fclose(file);
    }



}
#endif




void GreedyArticulationPointsRemovalRealMultiplex(NETWORK &network1, NETWORK &network2)
{

	// output file .
	char outputFileName[FILE_NAME_LENGTH];
	sprintf(outputFileName, "%s/GAPR.dat", GoutputAddress);

	FILE *file = fopen(outputFileName, "w");




	lint i, t, row, column;

	pair <lint, lint> gmcc_time;

	double gmcc;

	vector < vector <lint> > record;




    // Find number of articulation points.
    gmcc_time = GreedyArticulationPointsRemoval(network1, network2, record);
    gmcc =  (double)gmcc_time.first/(double)network1.num_node;

    // print results to file.
    fprintf(file, "%lf \t %ld\n", gmcc, gmcc_time.second);
    printf("S : %lf \t T : %ld\n", gmcc, gmcc_time.second);




    fclose(file);




    row = record.size();

    // print time evolution of the GAPR process.
    for (i=ZERO ; i<row ; i++)

    {

    	char outputFileName[FILE_NAME_LENGTH];
		sprintf(outputFileName, "%s/TimeEvolution.dat", GoutputAddress);

		file = fopen(outputFileName, "w");



		column = record[i].size();

		//column /=3;

		for (t=ZERO ; t<column; t=t+3)
		{
			fprintf(file, "%ld \t %lf \t %lf \n", record[i][t],
												  (double)record[i][t+1]/(double)network1.num_node,
											      (double)record[i][t+2]/(double)network1.num_node);
		}


		fclose(file);
    }



}
*/

/***********************************************************************************************************/



/*
void FindArticulationPointsRecursively3 (NETWORK &network, lint node, lint &time, vector <lint> &parent,
		 								 vector <lint> &lowValue, vector <lint> &discoveryTime,
										 vector <bool> &visited, vector <bool> &articulationPoints)
{

    lint i, children, neighbor;

    children = ZERO;

    visited[node] = true;

    discoveryTime[node] = lowValue[node] = ++time;



    for (i=ZERO ; i<network.degree[node] ; i++)
    {

    	neighbor = network.node[node].link[i].target;


    	if (network.gccRoot == network.componentID[neighbor])
    	{

    		if (false == visited[neighbor])
	    	{
	    		children++;

	    		parent[neighbor] = node;

	    		FindArticulationPointsRecursively3 (network, neighbor, time, parent, lowValue, discoveryTime, visited, articulationPoints);

	    		lowValue[node] = min (lowValue[node] , lowValue[neighbor]);



	    		if (UNKNOWN == parent[node] && children>ONE)
	    		{
	    			articulationPoints[node] = true;
	    		}

	    		if (UNKNOWN != parent[node] && lowValue[neighbor] >= discoveryTime[node])
	    		{
	    			articulationPoints[node] = true;
	    		}

	    	}
	    	else if (neighbor != parent[node])
	    	{
	    		lowValue[node] = min (lowValue[node], discoveryTime[neighbor]);
	    	}

    	}

    }


}



void DepthFirstSearch (NETWORK &network, vector <bool> &visited, lint node, lint &clusterSize)
{

    lint i, neighbor;

    visited[node] = true;



    for (i=ZERO ; i<network.degree[node] ; i++)
    {

    	neighbor = network.node[node].link[i].target;


    	if (network.gccRoot == network.componentID[neighbor])
    	{

    		if (!visited[neighbor])
	    	{
	    		DepthFirstSearch (network, visited, neighbor, clusterSize);
	    	}

    	}

    }

    clusterSize++;
}



lint ClaculateScore (NETWORK &network, vector <bool> &visited, lint node)
{

	lint i, neighbor, clusterSize, maxSize;

	maxSize = ZERO;



	for(i=ZERO ; i<network.num_node ; i++)
	{
		visited[i] = false;
	}



	network.componentID[node] = UNKNOWN;



	for(i=ZERO ; i<network.degree[node] ; i++)
	{

		neighbor = network.node[node].link[i].target;

		if(!visited[neighbor] && network.gccRoot == network.componentID[neighbor])
		{

			clusterSize = ZERO;

			DepthFirstSearch (network, visited, neighbor, clusterSize);

			maxSize = max (clusterSize, maxSize);
		}

	}



	network.componentID[node] = network.gccRoot;



	return maxSize;
}




pair <double, double> ArticulationPointsTargetedAttack(NETWORK &network, class MTRand *RandNumb, FILE *file, FILE *SNfile)
{

	lint i, time, score, minScore, minID, removed;

	bool hasAP = true;

	double gcc;

	removed = ZERO;



	// Before removing.
    DetectAllIsolatedComponents (network);

    gcc = (double)network.gccSize/(double)network.num_node;

    fprintf(file, "%lf \t %lf \n", 0.0, gcc);
    printf("%lf \t %lf \n", 0.0 , gcc);



	while (hasAP && gcc>Gthreshold)
	{

		hasAP = false;



		vector <bool> visited 			 (network.num_node, false);
		vector <bool> articulationPoints (network.num_node, false);
		vector <lint> parent 			 (network.num_node, UNKNOWN);
		vector <lint> lowValue 			 (network.num_node);
		vector <lint> discoveryTime 	 (network.num_node);



		for(i=ZERO ; i<network.num_node ; i++)
		{
			if(false == visited[i] && network.gccRoot == network.componentID[i])
			{
				time = ZERO;

				FindArticulationPointsRecursively3 (network, i, time, parent, lowValue, discoveryTime, visited, articulationPoints);

				break;
			}
		}



		minScore = network.num_node;
		minID = UNKNOWN;



		for(i=ZERO ; i<network.num_node ; i++)
		{
			if(articulationPoints[i])
			{

				hasAP = true;

				score = ClaculateScore (network, visited, i);

				if (score<minScore)
				{
					minScore = score;
					minID = i;
				}

			}
		}



		if(hasAP)
		{

			network.node[minID].state = ABSENT;

			DetectAllIsolatedComponents (network);

			removed++;



		    gcc = (double)network.gccSize/(double)network.num_node;

		    fprintf(file, "%lf \t %lf \n", (double)(removed)/(double)network.num_node, gcc);
		    printf("%lf \t %lf \n", (double)(removed)/(double)network.num_node, gcc);
		}

	}



    return (make_pair( ((double)removed/(double)network.num_node), gcc));

}
*/





#endif
