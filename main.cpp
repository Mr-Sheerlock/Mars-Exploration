#include "MasterStation.h"
#include <iostream>
using namespace std;


int main() {
	
	MasterStation MS;

	//Read input
	MS.ReadInputFile();

	while (!MS.CheckLastDay()) {

		MS.ExecuteDay();
	}
	
}
