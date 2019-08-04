#include "types_p2.h"
#include "utils.h"



void Person::set_gender(int data) { gender = data; }
int Person::get_gender(void)      { return gender; }

void Person::set_order(unsigned long data) { order = data; }
unsigned long Person::get_order(void)      { return order; }

void Person::set_use_order(unsigned long data) { use_order = data; }
unsigned long Person::get_use_order(void)      { return use_order; }

void Person::set_time(long data) { time_to_stay_ms = data; }
void Person::get_time(void) { return get_elasped_time(t_create,t_start); }
int Person::ready_to_leave(void) {
	struct timeval t_curr;
	gettimeofday(&t_curr, NULL);

	if (get_elasped_time(t_start, t_curr) >= time_to_stay_ms) { return 1; }
	else { return 0; }
}
void Person::start(void) {
	gettimeofday(&t_start, NULL);
	printf("(%lu)th person enters the restroom: \n", order);
	printf(" - (%lu) milliseconds after the creation\n", get_elasped_time(t_create, t_start));
}
void Person::complete(void) {
	gettimeofday(&t_end, NULL);
	printf("(%lu)th person comes out of the restroom: \n", order);
	printf(" - (%lu) milliseconds after the creation\n", get_elasped_time(t_create, t_end));
	printf(" - (%lu) milliseconds after using the restroom\n", get_elasped_time(t_start, t_end));
}
Person::Person() {
	gettimeofday(&t_create, NULL);
}
// You need to use this function to print the Restroom's status
void Restroom::out(std::string ss){
	fprintf(stdout,"%s\n",ss);
}
void Restroom::print_status(void) {
	switch (gen){
		MAN:out("MAN");return;
		WOMAN:out("WOMAN");return;
		EMPTY:out("EMPTY");return; 
	}
}
// Call by reference
// This is just an example. You can implement any function you need
void Restroom::add_person(Person& p) {
	pthread_mutex_lock(mutex_s);
	queue.push(Person p);
	if(EMPTY==gen){
		start();
		gen = p.get_gender();
	}
	pthread_mutex_unlock(mutex_s);
}
int Restroom::get_gender(void){
	return gen;
}
//run thread
void Restroom::start(void){
	pthread_create(&rid,NULL,Restroom::threadfunc,NULL);
}
void Restroom::wait(void){
	pthread_cond_wait(cond,mutex);
}
void Restroom::signal(void){
	pthread_cond_signal(cond);
}
void Restroom::join(void){
	pthread_join(rid,NULL);
}
void *Restroom::threadfunc(void *parm){
	//thread
	while(!rqueue.empty()){
		user = rqueue.pop();
		user.start();
		gen = user.get_gender();
		std::string ing = (gen==MAN)?"(Man)":"(Woman)";
		fprintf(stdout,"[%d ms][Input] A person %s goes into the que\n",user.get_time(),ing);
		while (!user.ready_to_leave());
		user.complete();
		fprintf(stdout,"[%d ms][fitting room] A person %s left the que ",user.get_time(),ing);
		fprintf(stdout,"Total: %d %s in fitting room %d\n"rqueue.size(),ing,rid);
	}
	gen = EMPTY;
	//signal main empty room
	signal();
}
//eof
