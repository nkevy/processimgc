#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <pthread.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>

pid_t id[100];
int sum,index;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *threadfunc(void* param);
void run_test(pthread_t pid);
int main(int argc,char *argz[]);
#endif
