#ifndef FILEIO
#define FILEIO

#include "structs.h"
void addResultToFile(const int algoCode, const int seed, const Hyperparams hyperparams, 
                     const double finalScore, const double timeForRun);

#endif
