
#include "rt_nonfinite.h"
#include "findpeakcore.h"
#include "fpeak.h"


static double rt_roundd_snf(double u);
static double rt_roundd_snf(double u)
{
  double y;
  if (fabs(u) < 4.503599627370496E+15) {
    if (u >= 0.5) {
      y = floor(u + 0.5);
    } else if (u > -0.5) {
      y = u * 0.0;
    } else {
      y = ceil(u - 0.5);
    }
  } else {
    y = u;
  }

  return y;
}

void findpeakcore(double N, const double *x, double *difx, double
                  *heightl, double *heightr, double *newheightl, double
                  *newheightr, double *areal, double *arear, double *newareal,
                  double *newarear,int *base1,int *feng1,int *base2,int *feng2)
{
  double peakr;
  double peakl;
  int i;
  int i0;
  int j;
  double newbase;
  double width0;
  double pstartmin;
  double basestart;
  double baseend;
  double widthl;
  double widthr;
  double peaki;
  double d9;
  double difxmin;
  int32_T exitg7;
  double pstart;
  double pend;
  int32_T exitg9;
  int32_T exitg8;
  int32_T exitg6;
  boolean_T exitg5;
  int32_T exitg4;
  boolean_T exitg3;
  int b_x;
  int k;
  boolean_T exitg2;
  boolean_T exitg1;
  int NumPoint1=NumPoint-1;
  fpeak(rt_roundd_snf(N / 40.0), rt_roundd_snf(N - N / 40.0), x, 1.0,
        rt_roundd_snf(N / 3.0), &peakl, &peakr);
  for (i = 0; i < (int)((N - 1.0) + -1.0); i++) {
    difx[i + 1] = x[(int)((2.0 + (double)i) + 1.0) - 1] - x[i + 1];
  }

  for (i = 0; i < (int)((N - 1.0) - 6.0); i++) {
    difx[i] = 0.0;
    i0 = (int)(((6.0 + (1.0 + (double)i)) - 1.0) + (1.0 - ((1.0 + (double)i) +
      1.0)));
    for (j = 0; j < i0; j++) {
      difx[i] += difx[(int)(((1.0 + (double)i) + 1.0) + (double)j) - 1];
    }
  }

  i0 = (int)((N - 1.0) + (1.0 - (((N - 1.0) - 3.0) + 1.0)));
  for (i = 0; i < i0; i++) {
    difx[(int)((((N - 1.0) - 3.0) + 1.0) + (double)i) - 1] = difx[(int)((N - 1.0)
      - 6.0) - 1];
  }

  for (i = 0; i < (int)-(1.0 + (-1.0 - ((N - 1.0) - 6.0))); i++) {
    newbase = ((N - 1.0) - 6.0) + -(double)i;
    difx[(int)(newbase + 3.0) - 1] = difx[(int)newbase - 1];
  }

  for (i = 0; i < 3; i++) {
    difx[i] = difx[3];
  }

  width0 = (peakr - peakl) / 3.0;
  pstartmin = 1.0;
  basestart = 0.0;
  baseend = 0.0;
  *heightl = 0.0;
  *heightr = 0.0;
  *areal = 0.0;
  *arear = 0.0;
  widthl = 0.0;
  widthr = 0.0;
  for (i = 0; i < 2; i++) {
    if (1 + i == 1) {
      peaki = peakl;
    } else {
      peaki = peakr;
    }

    newbase = rt_roundd_snf(peaki - width0);
    if (newbase < 5.0) {
      newbase = 5.0;
    }

    d9 = rt_roundd_snf(peaki + width0);
    if (d9 > (N - 1.0) - 5.0) {
      d9 = (N - 1.0) - 5.0;
    }

    difxmin = 0.0;
    i0 = (int)(d9 + (1.0 - newbase));
    for (j = 0; j < i0; j++) {
      difxmin += fabs(difx[(int)(newbase + (double)j) - 1]);
    }

    difxmin = difxmin / ((d9 - newbase) + 1.0) / 6.0;
    newbase = rt_roundd_snf(N / 30.0);
    if (peaki == 0.0) {
      if (1 + i == 1) {
        peakl = 1.0;
      } else {
        peakr = 1.0;
      }
    } else {
      do {
        exitg7 = 0;
        pstart = peaki - newbase;
        pend = peaki + newbase;
        do {
          exitg9 = 0;
          if (difx[(int)pstart - 1] > difxmin) {
            pstart--;
            if (pstart <= pstartmin + 1.0) {
              exitg9 = 1;
            }
          } else {
            exitg9 = 2;
          }
        } while (exitg9 == 0);

        do {
          exitg8 = 0;
          if (difx[(int)pend - 1] < -difxmin) {
            pend++;
            if (pend >= NumPoint1) {
              exitg8 = 1;
            }
          } else {
            exitg8 = 1;
          }
        } while (exitg8 == 0);

        if ((pstart <= pstartmin + 1.0) && (pend >= NumPoint1)) {
          newbase--;
          if (newbase <= 1.0) {
            exitg7 = 1;
          }
        } else {
          exitg7 = 2;
        }
      } while (exitg7 == 0);

      do {
        exitg6 = 0;
        if (x[(int)pstart - 1] > 2.0 * x[(int)pend - 1]) {
          pstart--;
          if (pstart <= pstartmin + 1.0) {
            exitg6 = 1;
          } else {
            exitg5 = FALSE;
            while ((exitg5 == FALSE) && (difx[(int)pstart - 1] > difxmin)) {
              pstart--;
              if (pstart <= pstartmin + 1.0) {
                exitg5 = TRUE;
              }
            }
          }
        } else {
          exitg6 = 2;
        }
      } while (exitg6 == 0);

      do {
        exitg4 = 0;
        if (x[(int)pend - 1] > 2.0 * x[(int)pstart - 1]) {
          pend++;
          if (pend >= NumPoint1) {
            exitg4 = 1;
          } else {
            exitg3 = FALSE;
            while ((exitg3 == FALSE) && (difx[(int)pend - 1] < -difxmin)) {
              pend++;
              if (pend >= NumPoint1) {
                exitg3 = TRUE;
              }
            }
          }
        } else {
          exitg4 = 1;
        }
      } while (exitg4 == 0);

      j = (int)rt_roundd_snf(pstart) - 1;
      b_x = (int)rt_roundd_snf(pend) - 1;
      if (x[j] > x[b_x]) {
        i0 = (int)-(peaki + (-1.0 - pend));
        k = 0;
        exitg2 = FALSE;
        while ((exitg2 == FALSE) && (k <= i0 - 1)) {
          newbase = pend + -(double)k;
          if (x[(int)rt_roundd_snf(newbase) - 1] > x[j]) {
            if ((newbase >= peaki + 1.0) || rtIsNaN(peaki + 1.0)) {
            } else {
              newbase = peaki + 1.0;
            }

            if ((NumPoint <= newbase) || rtIsNaN(newbase)) {
              pend = NumPoint;
            } else {
              pend = newbase;
            }

            exitg2 = TRUE;
          } else {
            k++;
          }
        }
      } else {
        i0 = (int)(peaki + (1.0 - pstart));
        k = 0;
        exitg1 = FALSE;
        while ((exitg1 == FALSE) && (k <= i0 - 1)) {
          newbase = pstart + (double)k;
          if (x[(int)rt_roundd_snf(newbase) - 1] > x[b_x]) {
            if ((newbase <= peaki - 1.0) || rtIsNaN(peaki - 1.0)) {
            } else {
              newbase = peaki - 1.0;
            }

            if ((pstartmin >= newbase) || rtIsNaN(newbase)) {
              pstart = pstartmin;
            } else {
              pstart = newbase;
            }

            exitg1 = TRUE;
          } else {
            k++;
          }
        }
      }

      newbase = rt_roundd_snf(pstart);
      difxmin = 1.0E+10;
      i0 = (int)(rt_roundd_snf(pend) + (1.0 - newbase));
      for (j = 0; j < i0; j++) {
        d9 = newbase + (double)j;
        if (x[(int)d9 - 1] < difxmin) {
          difxmin = x[(int)d9 - 1];
        }
      }

      if (1 + i == 1) {
        *heightl = x[(int)peaki - 1] - difxmin;

          *feng1=(int)peaki;

        basestart = pend;

        *base1=basestart;
      } else {
        *heightr = x[(int)peaki - 1] - difxmin;

          *feng2=(int)peaki;

        baseend = pstart;

        *base2=baseend;
      }

      newbase = 0.0;
      d9 = rt_roundd_snf(pstart);
      i0 = (int)(rt_roundd_snf(pend) + (1.0 - d9));
      for (j = 0; j < i0; j++) {
        newbase = (newbase + x[(int)(d9 + (double)j) - 1]) - difxmin;
      }

      if (1 + i == 1) {
        *areal = newbase;
        widthl = (pend - pstart) + 1.0;
      } else {
        *arear = newbase;
        widthr = (pend - pstart) + 1.0;
      }

      pstartmin = pend;
    }
  }

  newbase = 0.0;
  i0 = (int)(baseend + (1.0 - basestart));
  for (i = 0; i < i0; i++) {
    newbase += x[(int)(basestart + (double)i) - 1];
  }

  newbase /= (baseend - basestart) + 1.0;
  *newheightl = x[(int)peakl - 1] - newbase;
  *newheightr = x[(int)peakr - 1] - newbase;
  *newareal = *areal + ((x[(int)peakl - 1] - *heightl) - newbase) * widthl;
  *newarear = *arear + ((x[(int)peakr - 1] - *heightr) - newbase) * widthr;
}
