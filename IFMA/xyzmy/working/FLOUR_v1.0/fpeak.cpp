
#include "rt_nonfinite.h"
#include "findpeakcore.h"
#include "fpeak.h"

void fpeak(double i0, double i9, const double *v, double delta, double
           xdelta, double *peakl, double *peakr)
{
  double mn;
  double mx;
  double mxx;
  int lookformax;
  int mxpos;
  int i1;
  int i;
  double b_i;
  double istart;
  int istep;
  double mxx2;
  mn = 1.0E+10;
  mx = -1.0E+10;
  mxx = -1.0E+10;
  lookformax = 1;
  *peakl = 0.0;
  *peakr = 0.0;
  mxpos = 0;
  i1 = (int)(i9 + (1.0 - i0));
  for (i = 0; i < i1; i++) {
    b_i = i0 + (double)i;
    if (v[(int)b_i - 1] > mx) {
      mx = v[(int)b_i - 1];
      mxpos = (int)b_i;
    }

    if (v[(int)b_i - 1] < mn) {
      mn = v[(int)b_i - 1];
    }

    if (lookformax != 0) {
      if (v[(int)b_i - 1] < mx - delta) {
        if (mx > mxx) {
          mxx = mx;
          *peakl = mxpos;
        }

        mn = v[(int)b_i - 1];
        lookformax = 0;
      }
    } else {
      if (v[(int)b_i - 1] > mn + delta) {
        mx = v[(int)b_i - 1];
        mxpos = (int)b_i;
        lookformax = 1;
      }
    }
  }

  if (*peakl - xdelta > i0) {
    istart = *peakl - xdelta;
    istep = -1;
    mxx = i0;
  } else {
    istart = *peakl + xdelta;
    istep = 1;
    mxx = i9;
  }

  mn = 1.0E+10;
  mx = -1.0E+10;
  mxx2 = -1.0E+10;
  lookformax = 1;
  i1 = (int)((mxx + ((double)istep - istart)) / (double)istep);
  for (i = 0; i < i1; i++) {
    b_i = istart + (double)i * (double)istep;
    if (v[(int)b_i - 1] > mx) {
      mx = v[(int)b_i - 1];
      mxpos = (int)b_i;
    }

    if (v[(int)b_i - 1] < mn) {
      mn = v[(int)b_i - 1];
    }

    if (lookformax != 0) {
      if (v[(int)b_i - 1] < mx - delta) {
        if (mx > mxx2) {
          mxx2 = mx;
          *peakr = mxpos;
        }

        mn = v[(int)b_i - 1];
        lookformax = 0;
      }
    } else {
      if (v[(int)b_i - 1] > mn + delta) {
        mx = v[(int)b_i - 1];
        mxpos = (int)b_i;
        lookformax = 1;
      }
    }
  }

  if (*peakl > *peakr) {
    mxpos = (int)*peakl;
    *peakl = *peakr;
    *peakr = mxpos;
  }
}
