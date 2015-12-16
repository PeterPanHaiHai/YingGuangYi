#ifndef __FINDPEAKCORE_H__
#define __FINDPEAKCORE_H__
#include <math.h>
#include <stddef.h>
#include <stdlib.h>

#include "rt_nonfinite.h"

#include "rtwtypes.h"
#include "findpeakcore_types.h"

#define NumPoint 600
extern void findpeakcore(double N, const double *x, double *difx, double *heightl, double *heightr, double *newheightl, double *newheightr, double *areal, double *arear, double *newareal, double *newarear,int *base1,int *feng1,int *base2,int *feng2);
#endif
