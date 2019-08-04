#include "p1_threads.h"
using namespace std;

// This file includes functions that actually perform the
// computation. You can create a thread with these function
// or directly call this function
std::vector<int> _intList(std::vector<std::string> &ss);
std::vector<float> _floatList(std::vector<std::string> &ss);

void _threads(std::vector<std::string> &id,std::vector<std::string> &grade,int n_t,std::string fname){
	std::ofstream o_sort;
	std::ofstream o_stat;
	std::string outpath = "output/"+fname;
	std::string sort = outpath+"_sorted.csv";
	std::string stat = outpath+"_stats.csv";
	o_sort.open(sort.c_str());
	o_stat.open(stat.c_str());
	o_sort<<"Rank,Student ID,Grade"<<endl;
	o_stat<<"Average,Median,Std. Dev"<<endl;
	std::vector<int> idz = _intList(id);
	std::vector<float> gradez = _floatList(grade);
	//split list for merge 
	int segment = gradez.size()/n_t;
	int rem = gradez.size()%n_t;
	int it;
	int temp;
	int l,r;
	l=0;
	r=0;
	for (it=0;it<gradez.size();it++){
		temp=(segment-1);
		if (rem>0){
			temp++;
			rem--;
		}
		if (0==it%temp){
			//l to r is desired sement
			r+=(temp);
			//make thread sort this segment
			go(idz,gradez,l,r);
			//send
			l=it;
		}
	}
	float total = 0;
	float half=gradez.size()/2;
	float median;
	for(it=0;it<gradez.size();it++){
		o_sort<<it<<","<<id.at(it)<<","<<gradez.at(it)<<endl;
		total = total+gradez.at(it);
	}
	float avg = total/gradez.size();
	median = gradez.at(half);
	float stdv = avg-median;
	if (stdv<0){stdv=stdv*-1;}
	o_stat<<avg<<","<<median<<","<<stdv<<endl;
	
	o_stat.close();
	o_sort.close();
}

std::vector<int> _intList(std::vector<std::string> &ss){
	std::vector<int> ii;
	int v=0;
	for (std::vector<string>::size_type i=0;i!=ss.size();i++){
		v=atoi(ss[i].c_str());
		ii.push_back(v);
	}
	return ii;
}
std::vector<float> _floatList(std::vector<std::string> &ss){
	std::vector<float> ff;
	float v=0;
	for (std::vector<string>::size_type i=0;i!=ss.size();i++){
		v=atof(ss[i].c_str());
		ff.push_back(v);
	}
	return ff;
}
