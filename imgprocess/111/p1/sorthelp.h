#ifndef __SORTHELP
#define __SORTHELP

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <pthread.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

void go(std::vector<int> &ids,std::vector<float> &grades,int l,int r);
void* merge(void*);
//std::vector<int> i;
//std::vector<float> g;
//int ll,rr;

#endif
