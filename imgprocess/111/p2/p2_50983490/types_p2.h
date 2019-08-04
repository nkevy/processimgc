#ifndef __TYPES_P2_H
#define __TYPES_P2_H

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sys/time.h>
#include <string>
#include <vector>
#include <unistd.h>
#include <p2_threads.h>


class Person
{
	int gender; // 0: male 1: female
	std::string str_gender;
	struct timeval t_create;
	struct timeval t_start;
	struct timeval t_end;
	long time_to_stay_ms;

	unsigned long order;
	unsigned long use_order;

public:
	Person();

	void set_gender(int data);
	int get_gender(void);

	void set_order(unsigned long data);
	unsigned long get_order(void);

	void set_use_order(unsigned long data);
	unsigned long get_use_order(void);

	void set_time(long data);
	void get_time(void);
	int ready_to_leave(void);

	void start(void);
	void complete(void);
};

// You may need to add more class member variables and functions
class Restroom {
	enum gender gen;
	pthread_t rid;
	std::queue<Person> rqueue;
	Person user;
	void out(std::string ss);
	// You need to define the data structure to
    // save the information of people using the restroom
	// You can probebly use Standard Template Library (STL) vector
public:
	Restroom(){
		gen = EMPTY;
	}
	// You need to use this function to print the Restroom's status
	void print_status(void);
	void set_status(int s);
	gender get_status(void);
	// Call by reference
	// This is just an example. You can implement any function you need
	void add_person(Person& p);
	//thread method for rqueue
	void *threadfunc(void *parm);
	void start(void);
	void wait(void);
	void signal(void);
	void join(void);//and clean up
};
#endif
//eof
