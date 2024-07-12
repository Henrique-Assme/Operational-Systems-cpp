/*
gcc -pthread ... 
*/

#include <stdio.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <string>
#include <pthread.h>

int number = -1;
const char* filename;


int main(int argc, char * argv[])
{
	filename = argv[1];
    return 0;
}
