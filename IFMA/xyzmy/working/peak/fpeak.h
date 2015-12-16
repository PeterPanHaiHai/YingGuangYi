#ifndef __FPEAK_H__
#define __FPEAK_H__
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "rt_nonfinite.h"

#include "rtwtypes.h"
#include "findpeakcore_types.h"
extern "C" void fpeak(emxArray_real_T *v, double delta, emxArray_real_T *maxtab);
#endif
 
