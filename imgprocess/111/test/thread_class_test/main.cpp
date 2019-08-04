#include <main.h>
int main(int argc, char *argv[]){
	if (argc<2){
		fprintf(stderr,"usage: main <int>");
		return -1;
	}
	pid_t pid = getpid();
	int num = atoi(argv[1]);
	obj_thread mythread(num);
	mythread.run();
	fprintf(stderr,"main process complete: %d\n",pid);
	return 0;
}
