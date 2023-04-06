#ifndef _NETWORK_MODELS_CPP_
#define _NETWORK_MODELS_CPP_



#include "NetworkModels.h"



/****************************************************************************
						Degree Distributions.
*****************************************************************************/



void SamplePoissonDistribution(NETWORK &network, const lint N, const double meanDegree, const ulint seed)
{

    const gsl_rng_type * T;
    gsl_rng * r;


    gsl_rng_env_setup();


    T = gsl_rng_default;
    r = gsl_rng_alloc (T);
    gsl_rng_set (r,seed);



    lint i, k, sum;
    sum = ZERO;


    for (i=ZERO ; i<N ; i++)
    {
        while(true)
        {
            k = gsl_ran_poisson (r, meanDegree);

            if(k<N-ONE)
            {
                break;
            }
        }

        sum += k;

        network.degree[i] = k;
    }



    while(ZERO != sum%2)
    {

        i = gsl_rng_uniform_int(r, network.numNodes);

        sum -= network.degree[i];

        while(true)
        {
            k = gsl_ran_poisson (r, meanDegree);

            if(k<N-ONE)
            {
                break;
            }
        }

        sum += k;

        network.degree[i] = k;
    }


    gsl_rng_free (r);

}



void SamplePowerLawDistribution(NETWORK &network, const lint N, const double exponent, const lint kmin, const lint kmax, class MTRand *RandNumb)
{

    lint i, stubs, size;
    vector <double> CDF;


    stubs = ZERO;
    double norm = 0.0;


    // Create CDF for degree sampling.
    for (i=kmin ; i<=kmax ; i++)
    {
        norm += pow (i,-exponent) ;
        CDF.push_back(norm) ;
    }


    size = CDF.size();
    for (i=ZERO ; i<size ; i++)
        CDF[i] /= norm ;


    // sample degrees. 
    for (i=0 ; i<N ; i++)
    {
        network.degree[i] = lower_bound(CDF.begin(), CDF.end(), RandNumb->randExc())-CDF.begin()+kmin;
        stubs += network.degree[i];
    }


    // check total number of stubs to be even number.
    if(ZERO != stubs%2)
        network.degree[N-ONE]++;


    // set number of edges.
    network.numLinks = (lint)(stubs/2);
}


/****************************************************************************
						Configuration Model.
*****************************************************************************/


void PermutationStubs (vector <int> &stub, class MTRand *RandNumb)
{
    int i,j,N,temp;
    N = stub.size();

    for (i=0 ; i<N ; i++)
    {
        j = i+(N-i)*RandNumb->randExc() ;

        temp = stub[i] ;

        stub[i] = stub[j] ;

        stub[j] = temp ;
    }

}


void MatchStubs (NETWORK &network, vector <int> &stubs)
{

    int i, j ,stubSize, source, target ;


    int selfEdges = 0 ;
    int multiEdges = 0 ;


    LINK link ;
    link.state = PRESENT;


    stubSize = stubs.size();


    for (i=0 ; i<stubSize ; i=i+2)
    {

        source = stubs[i];
        target = stubs[i+1];


        if (source == target)
        {
            selfEdges ++ ;
            continue ;
        }


        bool state = true ;


        int s = network.node[source].link.size() ;

        for (j=0 ; j<s ; j++)
        {

            if (network.node[source].link[j].target == target)
            {
                multiEdges ++ ;
                state = false ;
                break ;
            }
        }


        if (state == false)
        {
            continue ;
        }


        link.target = target ;
        network.node[source].link.push_back(link) ;


        link . target = source ;
        network.node[target].link.push_back(link) ;

    }

}


void CreateConfigurationModelNetwork (NETWORK &network, class MTRand *RandNumb)
{
    int i, j ;
    vector <int> Stubs ;


    for (i=0 ; i<network.numNodes ; i++)
    {
        for (j=0 ; j<network.degree[i] ; j++)
        {
            Stubs.push_back(i) ;
        }
    }


    PermutationStubs(Stubs, RandNumb);

    MatchStubs(network, Stubs);

    Stubs.clear();

    network.BuildDegreeVector();
}


/****************************************************************************
							  Network Models.
*****************************************************************************/


void CreatePoissonNetwork (NETWORK &network, const lint N, const double meanDegree, const ulint seed)
{

    printf("\n***********************************\n");
    printf("Create Poisson Network!\n");
    cout<<"Number of Nodes : "<<N<<endl;
    cout<<"Mean Degree : "<<meanDegree<<endl;
    cout<<"Creation Seed : "<<seed<<endl;


    class MTRand *RandNumb = new MTRand((ulint)seed);


    network.numNodes = N;
    network.AllocateMemory();


    lint i, j, source, target ,numLinks;
    double p = meanDegree/((double)N-1.0);
    LINK link;
    link.state = PRESENT;
    numLinks = ZERO;


    for (i=0 ; i<N-ONE ; i++)
    {

        source = i ;

        for ( j=i+ONE ; j<N ; j++)
        {

            target = j ;

            if (RandNumb->randExc()<p)
            {
                link.target = target ;
                network.node[source].link.push_back(link) ;

                link.target = source ;
                network.node[target].link.push_back(link) ;

                numLinks ++ ;
            }

        }

    }
    

    network.numLinks = numLinks ;
    network.BuildDegreeVector();


    cout<<"Number of links of Poisson Network : "<<numLinks<<endl;

    free (RandNumb);
}



void CreateConfigurationModelPoissonNetwork (NETWORK &network, const lint N, const double meanDegree, const ulint seed)
{

    cout <<endl<<"##### Create Poisson Network. #####"<<endl<<endl;
    cout<<"Number of Nodes : "<<N<<endl;
    cout<<"Mean Degree : "<<meanDegree<<endl;
    cout<<"Creation Seed : "<<seed<<endl;


    class MTRand *RandNumb = new MTRand((ulint)seed);


    network.numNodes = N;
    network.AllocateMemory();


    SamplePoissonDistribution(network, N, meanDegree, seed);
    cout<<"Degree Sampled from Poisson distribution."<<endl;


    CreateConfigurationModelNetwork (network, RandNumb);
    cout<<endl<<"##### Poisson Network Created. #####"<<endl;


    network.numLinks = accumulate(network.degree.begin(), network.degree.end(), ZERO)/2;
    cout<<"Number of edges of Poisson Network : "<<network.numLinks<<endl;

    
    free (RandNumb);
}



void CreateConfigurationModelScaleFreeNetwork (NETWORK &network, const lint N, const double exponent, const lint kmin, const lint kmax, const ulint seed)
{
    
    cout <<endl<<"##### Create ScaleFree Network. #####"<<endl<<endl;
    cout<<"Number of Nodes : "<<N<<endl;
    cout<<"PowerLaw Exponent : "<<exponent<<endl;
    cout<<"Min Degree : "<<kmin<<endl;
    cout<<"Max Degree : "<<kmax<<endl;
    cout<<"Creation Seed : "<<seed<<endl;


    class MTRand *RandNumb = new MTRand((ulint)seed);


    network.numNodes = N;
    network.AllocateMemory();


    SamplePowerLawDistribution (network, N, exponent, kmin, kmax, RandNumb);
    cout<<"Degree Sampled from PowerLaw distribution."<<endl;


    CreateConfigurationModelNetwork (network, RandNumb) ;
    cout<<endl<<"##### ScaleFree Network Created. #####"<<endl;


    network.numLinks = accumulate(network.degree.begin(), network.degree.end(), ZERO)/2;
    cout<<"Number of edges of ScaleFree Network : "<<network.numLinks<<endl;


    free (RandNumb);
}


void CreateStaticNetwork (NETWORK &network, const lint N, const double gamma, const double c, const ulint seed)
{

    cout <<endl<<"########################"<<endl<<endl;
    cout<<"Create Static Network..."<<endl;
    cout<<"Number of Nodes : "<<N<<endl;
    cout<<"power-law exponent : "<<gamma<<endl;
    cout<<"Mean Degree : "<<c<<endl;
    cout<<"Creation Seed : "<<seed<<endl;


    class MTRand *RandNumb = new MTRand((ulint)seed);


    network.numNodes=N;
    network.AllocateMemory();


    lint i, j, size, source, target ,numEdges;
    double alpha, sum;
    bool condition;
    vector<double> CDF;

    LINK link;
    link.state =PRESENT;

    alpha=1.0/(gamma-1.0);
    sum=0.0;
    numEdges=(c*N/2);


    for(i=0 ; i<N ; i++)
    {
    	sum+=pow(i+1,-alpha);
    	CDF.push_back(sum);
    }

    for(i=0 ; i<N ; i++)
    	CDF[i]/=sum;


    // create links.
    for (i=0 ; i<numEdges ; i++)
    {

    	condition=true;

    	while(condition)
    	{	
    		condition=false;

    		source=lower_bound(CDF.begin(), CDF.end(), RandNumb->randExc())-CDF.begin();
        	target=lower_bound(CDF.begin(), CDF.end(), RandNumb->randExc())-CDF.begin();
    		
        	if(source==target)
        	{
        		condition=true;
        		continue;
        	}

       		size=network.node[source].link.size();
        	
        	for(j=0 ;j<size ; j++)
        		if(target==network.node[source].link[j].target)
        		{
        			condition=true;
        			continue;
        		}
    	}

    	//printf("%ld %ld\n", source, target);

        link.target=target;
        network.node[source].link.push_back(link);

        link.target=source;
        network.node[target].link.push_back(link);
    }


    network.numLinks = numEdges;

    network.BuildDegreeVector();


    cout<<"Number of edges of Static Network : "<<numEdges<<endl;

    cout<<endl<<"Static Network Created."<<endl;


    free (RandNumb);
}


void CreateStaticChungLuNetwork (NETWORK &network, const lint N, const double gamma, const double c, const ulint seed)
{

    cout <<endl<<"########################"<<endl<<endl;
    cout<<"Create Static Chung-Lu Network..."<<endl;
    cout<<"Number of Nodes : "<<N<<endl;
    cout<<"power-law exponent : "<<gamma<<endl;
    cout<<"Mean Degree : "<<c<<endl;
    cout<<"Creation Seed : "<<seed<<endl;


    class MTRand *RandNumb = new MTRand((ulint)seed);


    network.numNodes=N;
    network.AllocateMemory();


    lint i, j, size, source, target ,numEdges;
    double alpha, i0, sum;
    bool condition;
    vector<double> CDF;

    LINK link;
    link.state =PRESENT;

    alpha=1.0/(gamma-1.0);
    
    if( abs(alpha-0.5)<=1e-6 || alpha<0.5)
        i0=1.0;
    else
        i0=pow( (10.0*sqrt(2)*(1.0-alpha)) , (1.0/alpha) ) * pow(N,1.0-0.5/alpha);

    sum=0.0;
    numEdges=(c*N/2);


    for(i=0 ; i<N ; i++)
    {
        sum+=pow(i+i0,-alpha);
        CDF.push_back(sum);
    }

    for(i=0 ; i<N ; i++)
        CDF[i]/=sum;


    // create links.
    for (i=0 ; i<numEdges ; i++)
    {

        condition=true;

        while(condition)
        {   
            condition=false;

            source=lower_bound(CDF.begin(), CDF.end(), RandNumb->randExc())-CDF.begin();
            target=lower_bound(CDF.begin(), CDF.end(), RandNumb->randExc())-CDF.begin();
            
            if(source==target)
            {
                condition=true;
                continue;
            }

            size=network.node[source].link.size();
            
            for(j=0 ;j<size ; j++)
                if(target==network.node[source].link[j].target)
                {
                    condition=true;
                    continue;
                }
        }

        //printf("%ld %ld\n", source, target);

        link.target=target;
        network.node[source].link.push_back(link);

        link.target=source;
        network.node[target].link.push_back(link);
    }


    network.numLinks = numEdges;

    network.BuildDegreeVector();


    cout<<"Number of edges of Static Network : "<<numEdges<<endl;

    cout<<endl<<"Static Network Created."<<endl;


    free (RandNumb);
}


void CreateHyperbolicNetworks (NETWORK &network, const lint N, const double gamma, const double kbar, const double temperature, const ulint seed)
{
     
    cout <<endl<<"##### Create Hyperbolic Network. #####"<<endl;
    cout<<"Creation Seed : "<<seed<<endl;
    cout<<"Number of Nodes : "<<N<<endl;
    cout<<"Mean Degree : "<<kbar<<endl;
    cout<<"exponent : "<<gamma<<endl;
    cout<<"Temperature : "<<temperature<<endl;


    lint i, j, numLinks;
    double kmin, radius, mu, r, R, clustering, p, delta_theta;
    vector <double> kappa (N);
    vector <double> theta (N);
    vector <double> rCoordinates (N);
    const double PI = 3.1415926535897932;


    class MTRand *RandNumb = new MTRand((ulint)seed);


    // create network in S1 model.
    kmin = kbar*(gamma-2.0)/(gamma-1.0);
    radius = (double)N/(2.0*PI);
    mu = sin(temperature*PI)/(2.0*kbar*temperature*PI);
    printf("kmin=%lf \t radius=%lf \t mu=%lf\n",kmin, radius, mu);


    // sample kappa and theta.
    for(i=ZERO ; i<N ; i++)
    {
        kappa[i] = kmin*pow(1.0-RandNumb->randExc(), 1.0/(1.0-gamma));
        theta[i] =  2.0*PI*RandNumb->randExc();
    }


    // allocate memory for network.
    network.numNodes = N;
    network.AllocateMemory();
   

    numLinks = ZERO;
    LINK link;
    link.state = PRESENT;



    for(i=ZERO ; i<N-ONE ; i++)
    {
        for(j=i+ONE ; j<N ; j++)
        {
            delta_theta = abs(PI-abs(PI-abs(theta[i]-theta[j])));

            p = 1.0/(1.0 + pow(radius*delta_theta/(mu*kappa[i]*kappa[j]) , 1.0/temperature));

            if( RandNumb->randExc()<p )
            {
                link.target=i;
                network.node[j].link.push_back(link);

                link.target=j;
                network.node[i].link.push_back(link);

                numLinks++;
            }
        }
    }



    network.numLinks = numLinks;
    network.BuildDegreeVector();



    // change variabkes to H2 model.
    clustering = kbar*sin(temperature*PI)/(2.0*temperature);

    clustering = clustering * ( pow( (gamma-2.0)/(gamma-1.0) , 2.0) );

    R = 2.0*log(N/clustering);

    for(i=ZERO ; i<N ; i++)
    {
        r = R-2*log(kappa[i]/kmin);

        network.r[i] = (r>0.0)?r:0.0;   

        network.theta[i] = theta[i];
    }


    cout<<"Number of edges of Hyperbolic Network : "<<numLinks<<endl;
    cout<<endl<<"##### Hyperbolic Network Created. #####"<<endl;

    
    free (RandNumb);
}


void CreateNetworkWithHyperbolicMetric (NETWORK &network, const lint N, const double gamma, const double kbar, const double alpha, class MTRand *RandNumb)
{
     
    cout <<endl<<"##### Create Network With Hyperbolic Metric... #####"<<endl;
    cout<<"Number of Nodes : "<<N<<endl;
    cout<<"Mean Degree : "<<kbar<<endl;
    cout<<"exponent : "<<gamma<<endl;
    cout<<"Clustering Strength (Alpha) : "<<alpha<<endl;


    lint i, j, numLinks;
    double kmin, radius, mu, p, delta_theta;
    vector <double> kappa (N);
    vector <double> theta (N);
    const double PI = 3.1415926535897932;


    // create network in S1 model.
    kmin = kbar*(gamma-2.0)/(gamma-1.0);
    radius = (double)N/(2.0*PI);
    mu = (alpha-1.0)/(2.0*kbar);
    printf("kmin=%lf \t radius=%lf \t mu=%lf\n",kmin, radius, mu);


    // sample kappa and theta.
    for(i=ZERO ; i<N ; i++)
    {
        kappa[i] = kmin*pow(1.0-RandNumb->randExc(), 1.0/(1.0-gamma));
        theta[i] =  2.0*PI*RandNumb->randExc();
    }


    // allocate memory for network.
    network.numNodes = N;
    network.AllocateMemory();
   

    numLinks = ZERO;
    LINK link;
    link.state = PRESENT;


    for(i=ZERO ; i<N-ONE ; i++)
    {
        for(j=i+ONE ; j<N ; j++)
        {
            delta_theta = abs(PI-abs(PI-abs(theta[i]-theta[j])));

            p = pow(1.0+(radius*delta_theta)/(mu*kappa[i]*kappa[j]) , -alpha);

            if( RandNumb->randExc()<p )
            {
                link.target=i;
                network.node[j].link.push_back(link);

                link.target=j;
                network.node[i].link.push_back(link);

                numLinks++;
            }
        }
    }


    network.numLinks = numLinks;
    network.BuildDegreeVector();


    for(i=ZERO ; i<N ; i++)
    {
        network.node[i].state=PRESENT;
        network.r[i] = kappa[i];   
        network.theta[i] = theta[i];
    }


    cout<<"Number of edges of Network with Hyperbolic Metric : "<<numLinks<<endl;
    cout<<endl<<"##### Network With Hyperbolic Metric Created. #####"<<endl;

}



#endif 
