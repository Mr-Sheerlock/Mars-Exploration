
#include "MasterStation.h"
#include <iostream>
using namespace std;



//the main for loop
//I think some counter of days or somehting? 

int main() {
	MasterStation MS;

	for (int i = 0; i < 100; i++) {
		MS.ExecuteDay();

	}

	MS.FinalOutput();
	
}

