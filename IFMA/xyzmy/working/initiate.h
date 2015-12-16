#ifndef INITIATE_H
#define INITIATE_H
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

void createDirectory()
{
    //create directory we need
    //create 'User.txt' it contains username pasword authority

    mkdir("points",S_IRWXU);//create a new directory
    mkdir("formula",S_IRWXU);//create a new directory
    mkdir("user",S_IRWXU);
    mkdir("Result",S_IRWXU);
}
#endif // INITIATE_H



