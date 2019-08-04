#include <main.h>


int main(int argc, char *argz[]){
	int i;
	sum = 0;
	index = 0;
	pthread_t parent;
	parent = getpid();
	for (i=0;i<100;i++){
		//make process 
		id[i] = fork();
		if (0==id[i]){
			run_test(i);
			fprintf(stdout,"[child %lu] done running\n",i);
			exit(0);
		}
	}
	pid_t wid;
	while((wid=wait(&(index)))>0);
	fprintf(stdout,"sum : %d\n",sum);
	fprintf(stdout,"[parent %lu] done running\n",parent);
	return 0;
}
//make thread and run
void run_test(pthread_t pid)
{
	if(pthread_create(&(pid),NULL,threadfunc,NULL))
		fprintf(stdout,"[child %lu] error creating thread\n",pid);
}
void *threadfunc(void *param)
{
	time_t timer;
	time(&timer);
	usleep((int)timer%10);
	pthread_mutex_lock(&mutex);
	sum=sum+1;
	pthread_mutex_unlock(&mutex);
}
