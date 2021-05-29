#include "MasterStation.h"
#include <iostream>
using namespace std;




////I think some counter of days or somehting? 
int main() {
	
	MasterStation MS;

	//Read input
	//MS.ReadInputFile();


	
	for (int i = 0; i < 100; i++) {
		MS.ExecuteDay();

	}

	//MS.FinalOutput();
}
