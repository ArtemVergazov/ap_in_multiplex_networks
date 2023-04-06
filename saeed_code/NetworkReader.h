#ifndef _NETWORK_READER_H_
#define _NETWORK_READER_H_


#include "Header.h"
#include "NetworkClass.h"
#include "Utility.h"


#define LINELENGTH 1000


//void ReadMultiplexNetwork (Network &network1, Network &network2, const PARAMETERS &par);
//void ReadMonoplexNetwork (Network &network, const PARAMETERS &par);

void ReadMultiplexNetwork (vector <NETWORK> &multiplex, int argc, char* argv[]);
void ReadHyperbolicMultiplexNetwork (vector <NETWORK> &multiplex, int argc, char* argv[]);


#endif
