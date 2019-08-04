#include <sorthelp.h>
void go(std::vector<int> &ids,std::vector<float> &grades,int l,int r){
	pthread_t mythread;
	pthread_create(&mythread,NULL,merge,NULL);
}
void* merge(void*){
	//merge sort 
}
