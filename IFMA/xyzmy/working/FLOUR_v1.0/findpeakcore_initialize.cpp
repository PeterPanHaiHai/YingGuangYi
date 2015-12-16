#include "rt_nonfinite.h"
#include "findpeakcore.h"
#include "fpeak.h"
#include "findpeakcore_initialize.h"

void findpeakcore_initialize(void)
{
  rt_InitInfAndNaN(8U);
}

