#ifndef __FINDPEAKCORE_H__
#define __FINDPEAKCORE_H__
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "rt_nonfinite.h"

#include "rtwtypes.h"
#include "findpeakcore_types.h"
extern "C" void findpeakcore(double npeak0, double resolution, const emxArray_real_T *x, double *npeak, double peaks[600], double newpeaks[600]);
#endif
 
