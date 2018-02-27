/*
 * main.cpp
 *
 *  
 *
 *  Created on: 2018-1-7
 *      Author: Hong
 */

#include <stdio.h>

#define MAIN_SERVER

extern int main_server(int argc, char* argv[]);
extern int main_client(int argc, char* argv[]);

int main(int argc, char* argv[])
{
#ifdef MAIN_SERVER
    printf("server start...\n");
    return main_server(argc, argv);
#else
    printf("client start...\n");
    return main_client(argc, argv);
#endif
}

