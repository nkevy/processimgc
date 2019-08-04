#include "p2_threads.h"

extern pthread_cond_t  cond;
extern pthread_mutex_t mutex;
extern pthread_mutex_t mutex_q;
extern pthread_mutex_t mutex_r;
state flag;


void *quefunc(void *parm)
{
	//on start pause
	pthreads_cond_wait(cond,mutex);
	do{
	while(line_check())
	{
		pthreads_mutex_lock(&mutex_q);
		int next_gender = line.front().get_gender();
		pthreads_mutex_unlock(&mutex_q);
		//while avalable threads clear q
		if (next(next_gender)>=0){
			pthreads_mutex_lock(&mutex_q);
			strore.at(next).add_person(line.pop());
			pthreads_mutex_unlock(&mutex_q);
			//delay?
		}else{
			pthreads_cond_wait(cond,mutex);
		}
	}}while(OPEN==getState());
}
//get number of restroom avalable 
int next(int gender){
	for (int i=0;i<store.size();i++){
		pthread_mutex_lock(mutex_q);
		int temp = store.at(i).get_gender();
		pthead_mutex_unlock(mutex_q);
		if (gender==temp||EMPTY==temp){
			return i;
		}
	}
	return -1;
}
int getState(void){
	int ret;
	pthreads_mutex_lock(&mutex_q);
	ret = flag;
	pthreads_mutex_unlock(&mutex_q);
	return ret;
}
void setState(int x){
	pthreads_mutex_lock(&mutex_q);
	flag = x;
	pthreads_mutex_unlock(&mutex_q);
}
int line_check(void){
	pthread_mutex_lock(&mutex_q);
	int ret = (line.empty)?0:1;
	pthread_mutex_lock(&mutex_q);
	return ret;
}
//eof
