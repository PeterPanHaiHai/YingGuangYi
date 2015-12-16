#ifndef __SORTROWS_H__
#define __SORTROWS_H__
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "rt_nonfinite.h"
 
#include "rtwtypes.h"
#include "findpeakcore_types.h"
extern "C" void b_sortrows(emxArray_real_T *y);
extern "C" void sortrows(emxArray_real_T *y);
#endif
