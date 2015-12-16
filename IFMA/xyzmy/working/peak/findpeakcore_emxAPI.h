#ifndef __FINDPEAKCORE_EMXAPI_H__
#define __FINDPEAKCORE_EMXAPI_H__
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "rt_nonfinite.h"

#include "rtwtypes.h"
#include "findpeakcore_types.h"
extern "C" emxArray_real_T *emxCreateND_real_T(int numDimensions, int *size);
extern "C" emxArray_real_T *emxCreateWrapperND_real_T(double *data, int numDimensions, int *size);
extern "C" emxArray_real_T *emxCreateWrapper_real_T(double *data, int rows, int cols);
extern "C" emxArray_real_T *emxCreate_real_T(int rows, int cols);
extern "C" void emxDestroyArray_real_T(emxArray_real_T *emxArray);
#endif
 
