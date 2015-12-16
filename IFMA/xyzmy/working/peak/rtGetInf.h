#ifndef __RTGETINF_H__
#define __RTGETINF_H__

#include <stddef.h>
#include "rtwtypes.h"
#include "rt_nonfinite.h"

extern "C" real_T rtGetInf(void);
extern "C" real32_T rtGetInfF(void);
extern "C" real_T rtGetMinusInf(void);
extern "C" real32_T rtGetMinusInfF(void);
 
#endif
