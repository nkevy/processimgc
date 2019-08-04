#ifndef __P1_THREADS
#define __P1_THREADS

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include "p1_process.h"
#include "sorthelp.h"

void _threads(std::vector<std::string> &id, std::vector<std::string> &grade,int n_t,std::string fname);

#endif
