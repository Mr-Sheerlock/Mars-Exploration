#include "MasterStation.h"
#include <iostream>
using namespace std;


int main() {
	
	MasterStation MS;

	//Read input
	MS.ReadInputFile();
	if (MS.CheckConsistency()) {
		
		//Call read user choice
		MS.ReadUserChoice();

		while (!MS.CheckLastDay()) {

			MS.ExecuteDay();
		}
	}
}


//int main() {
//
//	int* A = new int[5]; A[0] = 5; A[1] = 2; A[2] = 3;
//
//
//	int B[] = { 1,2,3 };
//
//	cout << "lol hd";
//
//
//}




