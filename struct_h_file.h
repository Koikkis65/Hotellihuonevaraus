#ifndef struct_h_file
#define struct_h_file

#include "includes_and_prototypes_h_file.h"
using namespace std;

struct room {
	int roomnum;
	int reservationnum;
	string name;
	int roomsize;
	int discount;
	int timeofstay;
};

#endif