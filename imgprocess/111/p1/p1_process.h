#ifndef __P1_PROCESS
#define __P1_PROCESS

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

#include "p1_threads.h"
#include "sorthelp.h"

void get_statistics(std::string class_name[], int num_processes, int num_threads);

#endif
