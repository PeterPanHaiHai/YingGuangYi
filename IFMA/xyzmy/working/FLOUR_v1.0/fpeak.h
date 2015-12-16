#ifndef __FPEAK_H__
#define __FPEAK_H__
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include "rt_nonfinite.h"

#include "rtwtypes.h"
#include "findpeakcore_types.h"
extern void fpeak(double i0, double i9, const double *v, double delta, double xdelta, double *peakl, double *peakr);
#endif
