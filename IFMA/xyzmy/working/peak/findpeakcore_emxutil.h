#ifndef __FINDPEAKCORE_EMXUTIL_H__
#define __FINDPEAKCORE_EMXUTIL_H__
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "rt_nonfinite.h"

#include "rtwtypes.h"
#include "findpeakcore_types.h"
extern "C" void b_emxInit_int32_T(emxArray_int32_T **pEmxArray, int numDimensions);
extern "C" void b_emxInit_real_T(emxArray_real_T **pEmxArray, int numDimensions);
extern "C" void emxEnsureCapacity(emxArray__common *emxArray, int oldNumel, int elementSize);
extern "C" void emxFree_int32_T(emxArray_int32_T **pEmxArray);
extern "C" void emxFree_real_T(emxArray_real_T **pEmxArray);
extern "C" void emxInit_int32_T(emxArray_int32_T **pEmxArray, int numDimensions);
extern "C" void emxInit_real_T(emxArray_real_T **pEmxArray, int numDimensions);
#endif
 
