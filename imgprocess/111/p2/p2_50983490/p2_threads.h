#ifndef __P2_THREADS_H
#define __P2_THREADS_H

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sys/time.h>
#include <string>
#include <vector>
#include <queue>
#include <unistd.h>
#include "utils.h"
#include "types_p2.h"
#include <algorithm>
#include <pthread.h>

#define TIME 5
#define RANGE 7
#define OFFSET 3

enum state{OPEN,CLOSED};
state flag;
enum gender{EMPTY,MAN,WOMAN};

void *quefunc(void *parm);
int getState(void);
int next(int gender);
int line_check(void);
void setState(int x);

std::queue<Person> line;
std::vector<Restroom> store;
#endif
