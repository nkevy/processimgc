#include "types_p2.h"
#include "utils.h"
#include "p2_threads.h"

//feilds
pthread_cond_t  cond  = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_q = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_s = PTHREAD_MUTEX_INITIALIZER;

struct timeval t_global_start;

//methods
void enque(Person &p);
int next(void);

//main
int main(int argc, char** argv)
{
	if(argc<2||atoi(argv[1])<1||atoi(argv[2])<1){
		fprintf(stdout,"[ERROR] p2_exec expecting ints > 0 as args\n");
		fprintf(stdout,"[USAGE] p2_exec <int> <int>\n");
		exit(-1);
	}
	// This is to set the global start time
	gettimeofday(&t_global_start, NULL);
	int 		i=0;
	int 		j=0;
	//make 2x number of people 
	const int SIZE = atoi(argv[1]);
	const int NT = atoi(argv[2]);
	Person pple[2*SIZE];
	//make y+1 number of threads
	Restroom room[NT];
	for(i=0;i<NT;i++){
		store.push_back(room[NT]);
	}
	pthread_t qtid;
	//run quethread
	pthread_create(&qtid,NULL,quefunc,NULL);
	//set genders
	int gen=MAN;
	for(i=0;i<2*SIZE;i++){
		int g = rand()%2;
		int t = (rand()%RANGE) + (OFFSET);
		pple[i].set_order(i);
		pple[i].set_gender(g);
		pple[i].set_time(t);
		srand(i*time(NULL));
	}
	//pop and run with random stall
	for(i=0;i<2*SIZE;i++){
		//set delay time and get gender
		int s = rand()%TIME;
		int gen = pple[i].get_gender();
		//get time and delay
		usleep(MSEC(s));
		//que the person here
		enque(pple[i]);
		//if 1st person start the wait q thread
		if (0==i){
			pthread_cond_signal(cond,mutex);
		}//seed random
		srand(i*time(NULL));
	}
	//stop
	setState(CLOSED);
	//join
	pthread_join(qid,NULL);
	for(i=0;i<NT;i++){
		roomz.join();
	}
	return 0;
	// Example code for sleep and class usage
	Person p1;
	pthread_t tid;
	usleep(MSEC(200));
	p1.start();


	usleep(MSEC(150));
	p1.complete();
	///////////////////////////////////////////
	if(pthread_create(&tid, NULL, threadfunc, NULL)) {
		fprintf(stderr, "Error creating thread\n");		
	}
	usleep(MSEC(10));

	for (int i=0; i<5; i++) {
		printf("Wake up thread after (%d) seconds\n", (5-i));
		usleep(MSEC(1000));
	}

	printf("Wake up thread\n");
	int status;
	status = pthread_cond_signal(&cond);
	

	/* wait for the second thread to finish */
	if(pthread_join(tid, NULL)) {
		fprintf(stderr, "Error joining thread\n");	
	}

	return 0;
}
//method
void enque(Person p){
	pthreads_mutex_lock(&mutex_q);
	int index;
	index=next(line.front().get_gender());
	pthreads_mutex_unlock(&mutex_q);
	if (index>=0){
		//add - person gender matches fitting room, put p in that queue
		pthread_mutex_lock(mutex_s);
		store.at(index).push(p);
		pthread_mutex_unlock(mutex_s);
		p.start();
	}else{
		//add - gender does not match need to wait in line  
		pthreads_mutex_lock(&mutex_q);
		line.push(p);
		pthreads_mutex_unlock(&mutex_q);
	}
}
//eof
