#include <sys/wait.h>
#include "p1_process.h"

using namespace std;

int inputNum(void);
void runner(std::string files[],int len,int n_t);
void get_statistics(std::string class_name[], int num_processes, int num_threads) {

	// You need to create multiple processes here (each process
	// handles at least one file.)
	
	
	// Each process should use the sort function which you have defined  		
	// in the p1_threads.cpp for multithread sorting of the data. 
	
	// Your code should be able to get the statistics and generate
	// the required outputs for any number of child processes and threads.
	int i,j,in_num,f_num,f_add;
	pid_t og = getpid();
	pid_t now = og;
	in_num=inputNum();
	f_num = 1;
	f_add = 0;
	if (in_num>num_processes){
		f_num = in_num/num_processes;
		f_add = in_num%num_processes;
	}
	i=0;
	do{
		int temp = f_num;
		//add one to num files in process if remander still exists
		if(f_add>0){
			temp++;
			f_add--;
		}
		//check loop
		if(i+temp>in_num){
			break;
		}
		//load finp with files for used by this process 
		std::string finp[temp];
		for(j=0;j<temp;j++){
			finp[j]=class_name[i+j];
		}
		i=i+j;
		now = fork();
		if (now!=0){
			cout<<"child process is creaded: "<<"(pid: "<<now<<" )"<<endl;
		}
		if (getpid()!=og){
			runner(finp,temp,num_threads);
		}
	}while(og==getpid());
	if (getpid()!=og&&getpid()!=0){
		cout<<"child process is terminated: "<<"(pid: "<<getpid()<<" )"<<endl;
		exit(0);
	}
	if(getpid()==og){
		wait(0);
		wait(0);
		wait(0);
		wait(0);
		wait(0);
	}
}
int inputNum(void){
	DIR *in;
	int i=0;
	in=opendir("input");
	if(in!=NULL){
		while(readdir(in))
			i++;
		(void)closedir(in);
	}
	i=i>2?i-2:0;
	return i;
}
void runner(std::string files[],int len,int n_t){
	int i;
	char header[16];
	std::string line;
	vector<std::string> gr_l;
	vector<std::string> id_l;
	std::ifstream infile;
	for(i=0;i<len;i++){
		std::string path= "input/"+files[i]+".csv";
		infile.open(path.c_str());
		if (!infile.is_open()){
			cout<<"error importing files: r/w error"<<endl;
			exit(1);
		}
		//parse file
		std::string id;
		std::string value;
		if (infile>>header){
			while(infile >> line){
				size_t pos = line.find(",");
				id = line.substr(0,pos);
				value = line.substr(pos+1,line.length());
				gr_l.push_back(value);	
				id_l.push_back(id);
			}
		//send list to thread funct to sort and gen output
		_threads(id_l,gr_l,n_t,files[i]);
		}else{
			cout<<"error reading files: unknown format"<<endl;
			exit(1);
		}
		infile.close();
	}

}

