#include <obj_thread.h>
//construct thread
void obj_thread::obj_thread() : obj_thread(){
	pthread_create(&tid,attr,running,NULL);
}
void obj_thread::obj_thread(functiontype func) : obj_thread(func){
	
}
//start thread
void obj_thread::run(){
	//do something with number?
	int counter = 0;
	if (0==number){
		fprintf(stderr,"obj_thread exit: no method passed to constructor\n");
		return;
	}else if(-1==nunmber){
		//single internal thread
		pthread_create(&tid,&attr,running,NULL);
		//pthread_join(&tid,NULL);
	}else if(0>2){
		fprintf(stderr,"obj_thread exit: negative number passed to constructor\n");
	}else{
	}
}
//thread runs this method 
void obj_thread::*running(void *param){
	//local copy
	for(int i=10;i>0;i--){
		fprintf(stdout,"...\n");
	}
	fprintf(stdout,"thread created: test thread run complete\n");
}
//getter for thread id
pthread_t obj_thread::getId(){
	return tid;
}
//method to wait for child threads
void join(void){
	if(number<2){
		pthread_join(tid,NULL);
	}
}
