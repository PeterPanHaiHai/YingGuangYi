//#include <avr/io.h>

#include "rt_nonfinite.h"
#include "findpeakcore.h"
#include "fpeak.h"
#include "qtest.h"

//#include "stdio.h"
#define N 1200




double  Height_Diff,Height_Same,Area_Diff,Area_Same;
int base1,feng1,base2,feng2;
//int heightl,heightr,newheightl,newheightr;
double areal,arear;
double qTest(double xi[3000])
{
    int  j;
    double xi1[1200];
    double difxi[N-1];
    double heightl,heightr,newheightl,newheightr,newareal,newarear;
    //float  x[N];
    for(j=0;j<1200;j++)
    {
       xi1[j]=xi [j];//xi1[j]=xi[j]+(float)0.5;
    }

    findpeakcore(N, xi1, difxi, &heightl,&heightr,&newheightl,&newheightr,&areal,&arear,&newareal,&newarear,&base1,&feng1,&base2,&feng2);
    Height_Diff= heightl/heightr;
    Height_Same= newheightl/newheightr;
    Area_Diff=areal/arear;
    Area_Same=newareal/newarear;
    return Height_Diff;
}

