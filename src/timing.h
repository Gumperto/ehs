#ifndef TIMING
#define TIMING

#include <time.h>
#define CPUTIME(FCALL) (START = (double) clock(), FCALL, ELAPSED = ((double) clock() - START) / CLOCKS_PER_SEC)
#define SEC_TO_NANOSEC 1e9

#endif
