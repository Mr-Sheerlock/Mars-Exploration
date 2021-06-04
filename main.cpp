#include "MasterStation.h"
#include <iostream>
using namespace std;


int main() {
	
	MasterStation MS;

	//Read input
	MS.ReadInputFile();
	if (MS.CheckConsistency()) {

		while (!MS.CheckLastDay()) {

			MS.ExecuteDay();
		}
	}
	else {
		cout << "There were no Polar or Emergency Rovers, so simulation did not start! :D ";
	}
}
