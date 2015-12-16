#include "print.h"
typedef unsigned long DWORD;
typedef unsigned char UCHAR;
typedef unsigned short WCHAR;
int s_port = -1;
print::print()
{
}

void print::miniPrintWriteAscall(int fd,char data)
{

//know the detail of DWORD and UCHAR
        DWORD dwWrote = 0;
        UCHAR szPrint[128] = {0};
        if(fd==-1) printf("error!\n");

        /* Ñ¡Ôñ×Ö·ûŒ¯1 */
        memset(szPrint, 0, sizeof(szPrint));
        szPrint[0] = 0x1b;
        szPrint[1] = 0x36;
        //WriteFile(hPort, szPrint, 2, &dwWrote, NULL);
        write(fd,szPrint,2);
        dwWrote = 0;
        //WriteFile(hPort, &data, sizeof(char), &dwWrote, NULL);
        write(fd,&data,sizeof(char));

        //return (sizeof(char) == dwWrote);

}


void print::printInfo()
{
//AT here print a paper with lots of information

                  char *print;
                  print=(char *)malloc(500*sizeof(char));
                  int length;
                  int i;
                  //just print English
                 // miniPrintWriteAscall(s_port,print);
                                    strcpy(print,"Time:");
                                    char *printTime=" 18:48";
                                    strcat(print,printTime);
                                    strcat(print,"\nDate:");
                                    char *date=" 2012-04-08";
                                    strcat(print,date);
                                    strcat(print,"\nResult:");
                                    char *RESULT=" Positive";
                                    strcat(print,RESULT);
                                    strcat(print,"\nAge:");
                                    char *AGE=" 24";
                                    strcat(print,AGE);
                                    strcat(print,"\nGender");
                                    char *GENDER=" Male";
                                    strcat(print,GENDER);
                                    strcat(print,"\nName:");
                                    char *NAME=" Jacky";
                                    strcat(print,NAME);
                                    strcat(print,"\nID:");
                                    char *ID=" 1234567";
                                    strcat(print,ID);
                                    strcat(print,"\n---------------");
                                    strcat(print,"\nAnalysis Report");
                                    strcat(print,"\n");
                                    strcat(print,"\n");
                                     //At here make some chane
                                    printf("print is %s",print);
                                    length=strlen(print);
                                    for(i=0;i<length;i++)

                                        miniPrintWriteAscall(s_port,print[i]);



}
