#ifndef MYPRINT_H
#define MYPRINT_H
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <stdlib.h>
class MyPrint
{
public:
    MyPrint();
private:
    void printInfo();
    int set_opt();
    //int code_convert(char *from_charset,char *to_charset,char *inbuf,unsigned int inlen,char *outbuf,unsigned int outlen);
    //int u2g(char *inbuf,int inlen,char *outbuf,int outlen);
    void miniPrintWriteAscall(int fd,char data);
    //void miniPrintWriteUnicode(int fd,char *pwchello);
    //void printBefore();
};

#endif // MYPRINT_H
