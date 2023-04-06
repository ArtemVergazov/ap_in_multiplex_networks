#ifndef _HEADER_H_
#define _HEADER_H_



#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <time.h>
#include <list>
//#include <unistd.h>

//#include <windows.h>

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



#include <gsl/gsl_sf_zeta.h>
#include <gsl/gsl_randist.h>
#include <gsl/gsl_sf_gamma.h>
#include <gsl/gsl_cdf.h>



typedef long int lint ;
typedef unsigned long int ulint ;
typedef enum {PRESENT,ABSENT} STATE;


#define ZERO  (lint)0
#define ONE   (lint)1
#define UNKNOWN     (lint) -1
#define FILE_NAME_LENGTH (int) 1000




#endif 
