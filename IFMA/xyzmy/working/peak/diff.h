#ifndef __DIFF_H__
#define __DIFF_H__
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "rt_nonfinite.h"

#include "rtwtypes.h"
#include "findpeakcore_types.h"
extern void b_diff(const emxArray_real_T *x, emxArray_real_T *y);
extern void diff(const emxArray_real_T *x, emxArray_real_T *y);
#endif
 