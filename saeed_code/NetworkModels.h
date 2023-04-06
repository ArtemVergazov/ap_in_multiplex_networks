#ifndef _NETWORK_MODELS_H_
#define _NETWORK_MODELS_H_


#include "Header.h"
#include "NetworkClass.h"
#include "RNG_MT.h"


void CreatePoissonNetwork (NETWORK &network, const lint N, const double meanDegree, const ulint seed);

void CreateConfigurationModelPoissonNetwork (NETWORK &network, const lint N, const double meanDegree, const ulint seed);

void CreateConfigurationModelScaleFreeNetwork (NETWORK &network, const lint N, const double exponent, const lint kmin, const lint kmax, const ulint seed);

void CreateStaticNetwork (NETWORK &network, const lint N, const double gamma, const double c, const ulint seed);

void CreateStaticChungLuNetwork (NETWORK &network, const lint N, const double gamma, const double c, const ulint seed);

void CreateHyperbolicNetworks (NETWORK &network, const lint N, const double gamma, const double kbar, const double temperature, const ulint seed);

void CreateNetworkWithHyperbolicMetric (NETWORK &network, const lint N, const double gamma, const double kbar, const double alpha, class MTRand *RandNumb);



#endif
