#ifndef  UI_H
#define UI_H

#include <fstream>
#include <iostream>

using namespace std;

__declspec(selectany) //prevents the redefined problem emerging from the local variable

struct UI {    //struct because delspec works with it 



	///////////////////////////////////INPUT FUNCTIONS//////////////////////////////

	void ReadRovers( ){}

	void ReadRoverSpeeds(){ } //in case of constant speeds

	void Read_N_missions(){ }


	void Read_Checkup_Durations(){}


	//////////////////////////////////OUTPUT FUNCTIONS//////////////////////////////
	

	
	
	void PrinEndDay() {  //the total prints we need to do at the end of eachDay
		//use many functions here: like PrintPolar() PrintMountainious () PrintEmergency
		//as needed

	}


	void PrintOutput() {

	}


}U_I ;   //create a one global instant of the class as per the singleton technique
		 //(we don't need more than that)


#endif 