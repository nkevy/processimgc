#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sys/time.h>
#include <string>
#include <vector>
#include <unistd.h>
#include <pthread.h>
#include "types_p3.h"
#include "p3_threads.h"
#include "utils.h"

pthread_cond_t  cond[4];
pthread_cond_t a_task_is_done;
ThreadCtrlBlk   tcb[4];
std::vector<int> readyQue;

int num_of_alive_tasks=4;
int occupied = 0;


pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t taskDoneMutex = PTHREAD_MUTEX_INITIALIZER;

struct timeval t_global_start;
int global_work = 0;

void fifo_schedule(void);
void edf_schedule(int mult);
void rm_schedule(void);
int get_occupied(void);
ThreadCtrlBlk identify(int x);

int main(int argc, char** argv)
{
	if(argc !=2 || atoi(argv[1]) < 0 || atoi(argv[1]) > 2)
	{
		std::cout << "[ERROR] Expecting 1 argument, but got " << argc-1 << std::endl;
		std::cout<< "[USAGE] p3_exec <0, 1, or 2>" << std::endl;
		return 0;
	}
	int schedule = atoi(argv[1]);



	pthread_t       tid[4];
	int             status = 0;
	int             id = 0;
	long            stamp = 0;
	int             mode = 0;


	
	// This is to set the global start time
	gettimeofday(&t_global_start, NULL);	



	tcb[0].id = 0;
	tcb[0].task_time = 200;
	tcb[0].period = 1000;
	tcb[0].deadline = 1000;

	tcb[1].id = 1;
	tcb[1].task_time = 500;
	tcb[1].period = 2000;
	tcb[1].deadline = 2000;

	tcb[2].id = 2;
	tcb[2].task_time = 1000;
	tcb[2].period = 4000;
	tcb[2].deadline = 4000;

	tcb[3].id = 3;
	tcb[3].task_time = 1000;
	tcb[3].period = 6000;
	tcb[3].deadline = 6000;

	for (int i=0; i<4; i++) {
		pthread_cond_init (&(cond[i]), NULL); 
	}
	pthread_cond_init (&a_task_is_done, NULL); 
	

	global_work = 1;
	printf("[Main] Create worker threads\n");
	for (int i=0; i<4; i++) {
		if(pthread_create(&(tid[i]), NULL, threadfunc, &(tcb[i]))) {
			fprintf(stderr, "Error creating thread\n");		
		}
	}

	// Wait until the thread is in place
	usleep(MSEC(1000));
	
	
	// This is to reset the global time and skip the initial wait
	gettimeofday(&t_global_start, NULL);	
	
	
	int sleep = 0;
	for (int i=0; i<240; i++) {	
		stamp = get_time_stamp();

	/////////////////////////////////////////////////////////
	// Implement different scheduling algorithms
        // Select different schedule based on mode value
		 
		switch(schedule)
		{
			case 0:
				fifo_schedule();
				break;
			case 1:
				edf_schedule(i);
				break;
			case 2:
				rm_schedule();
				break;
		}
		/////////////////////////////////////////////////////////
		// Wait for a fraction of 100ms or until a task is done.

		stamp = get_time_stamp();
		sleep = 100 - (stamp%100);
		if (num_of_alive_tasks>0)
			timed_wait_for_task_complition(sleep);
		else{
			printf("All the tasks missed the deadline");
			break;
		}
	}


	printf("[Main] It's time to finish the thread\n");



	printf("[Main] Locks\n");
	pthread_mutex_lock(&mutex);
	global_work = 0;
	printf("[Main] Unlocks\n");

	
	// make sure all the process are in the ready queue
	usleep(MSEC(3000));
	while (readyQue.size()>0) {
		pthread_cond_signal(&(cond[readyQue[0]]));
		readyQue.erase(readyQue.begin()+0);

	}


	pthread_mutex_unlock(&mutex);


	/* wait for the second thread to finish */
	for (int i=0; i<4; i++) {
		if(pthread_join(tid[i], NULL)) {
			fprintf(stderr, "Error joining thread\n");	
		}
	}
	return 0;
}

//threads will add themselves to readyqueue
void fifo_schedule(void)
{
	//check ready queue for first element
	//use signals to start threads 
	//check if thread completed 
	//send 1st thread 
	int old;
	pthread_mutex_lock(&mutex);
	//pthread_cond_signal(&cond[old++]);
	pthread_cond_signal(&cond[readyQue[0]]);
	old = readyQue[0];
	readyQue.erase(readyQue.begin()+0);
	readyQue.push_back(old);
	pthread_mutex_unlock(&mutex);
	//wait for thread?
	//pthread_join(tid[it]);
	pthread_mutex_lock(&taskDoneMutex);
	while(get_occupied()){
		pthread_cond_wait(&a_task_is_done,&taskDoneMutex);
	}
	pthread_mutex_unlock(&taskDoneMutex);
}


void edf_schedule(int mult)
{
	//iterate over readyque to find earliest deadline tcb->deadline
	//or...just hard code order as tcb in main
	//use signal to send thread 
	//find earlies deadline
	long delay,delta,delay_new;
	int i,j;
	int size,index = 0;
	std::vector<ThreadCtrlBlk> sort;
	//initialize var
	pthread_mutex_lock(&mutex);
	size = readyQue.size();
	pthread_mutex_unlock(&mutex);
	delay_new = get_time_stamp();
	ThreadCtrlBlk next;
	next.id = -99;//err id
	for(i=0;i<size;i++){
		pthread_mutex_lock(&mutex);
		next = identify(readyQue[i]);
		pthread_mutex_unlock(&mutex);
		if (-99!=next.id)
			if (0==sort.size()){
				sort.push_back(next);
			}else{
				long time = get_time_stamp();
				long start = time - sort[0].deadline * (mult+1);
				start = (start<0)?(-1*start):start; //abs 
				long add = time - next.deadline * (mult+1);
				add = (add<0)?(-1*add):add; //abs
				if(start>add){
					sort.push_back(next);
				}else{
					sort.insert(sort.begin(),1,next);
				}
			}
		next.id = -99;
	}
	//send thread
	//note: starvation occurs using this schedualing method
	for(i=0;i<sort.size();i++){
		pthread_mutex_lock(&mutex);
		pthread_cond_signal(&cond[readyQue[sort[0].id]]);
		delay = sort[i].task_time;
		pthread_mutex_unlock(&mutex);
		break;
		if (num_of_alive_tasks>0){
			timed_wait_for_task_complition(delay);
		}
		break;
	}
}


void rm_schedule(void)
{
	//get the id, compare to tcb to get 1/period
	//compare and hold greatest value index
	int i,size,index=0;
	float rate,rate_new;
	std::vector<ThreadCtrlBlk> sort;
	pthread_mutex_lock(&mutex);
	size = readyQue.size();
	pthread_mutex_unlock(&mutex);
	rate_new = 1;
	for (i=0;i<size;i++){
		if (0==sort.size()){
			sort.push_back(tcb[readyQue[i]]);
		}else{
			pthread_mutex_lock(&mutex);
			rate_new = 1/(tcb[readyQue[i]].period);
			pthread_mutex_unlock(&mutex);
			rate = 1/(sort[0].period);
			if (rate>rate_new){
				sort.push_back(tcb[readyQue[i]]);
			}else{
				sort.insert(sort.begin(),1,tcb[readyQue[i]]);
			}
		}
	}
	//send
	for(i=0;i<sort.size();i++){
		//fprintf(stdout,"here : %d\n",sort[i]);
		pthread_mutex_lock(&mutex);
		pthread_cond_signal(&cond[sort[i].id]);
		pthread_mutex_unlock(&mutex);
		//wait
		pthread_mutex_lock(&taskDoneMutex);
		while(get_occupied()){
			pthread_cond_wait(&a_task_is_done,&taskDoneMutex);
		}
		pthread_mutex_unlock(&taskDoneMutex);
	}
}
int get_occupied(void)
{
	int ret;
	pthread_mutex_lock(&mutex);
	ret = occupied;
	pthread_mutex_unlock(&mutex);
	return ret;
}
ThreadCtrlBlk identify(int x){
	for (int i=0;i<4;i++)
		if(tcb[i].id==x)
			return tcb[i];
	ThreadCtrlBlk temp;
	temp.id=-99;
	return temp;
}
