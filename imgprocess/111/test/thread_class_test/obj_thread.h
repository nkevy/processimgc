#ifndef OBJ_THREAD
#define OBJ_THREAD

#include<pthread.h>
#include<stdio.h>
#include<fstream>
#include<iostream>
#include<string>
typedef void (*functiontype)();
class obj_thread{
	private:
	pthread_t *tid;
	pthread_attr_t attr;
	public:
	void obj_thread(int num);
	void run(void);
	void join(void);
	void exit(void);
	void running(void *param);
	pthread_t getId(void);
}	

#endif
