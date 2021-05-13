#ifndef  UI_H
#define UI_H

#include <fstream>
#include <iostream>

using namespace std;

__declspec(selectany) //prevents the redefined problem emerging from the local variable

struct UI {    //struct because delspec works with it 



	///////////////////////////////////INPUT FUNCTIONS//////////////////////////////

	void ReadRoversNumbers(ifstream& Input, int& M, int& P, int& E);

	int ReadRoverSpeeds(ifstream& Input); //returns the speed of each rover

	void Read_N_CheckupDur(ifstream& Input, int& N, int& CM, int& CP, int& CE); //number of missions before checkup and the checkup duration of each type

	int Read_AutoP(ifstream& Input);

	int Read_EventsNum(ifstream& Input);

	char ReadEventsTyp(ifstream& Input);

	void Read_Formulation(ifstream& Input, char& Rover_Typ, int& ED, int& ID, int& TLOC, int& MDUR, int& SIG);

	void Read_Promotion(ifstream& Input, int& ED, int& ID);

	void Read_Cancellation(ifstream& Input, int& ED, int& ID);


	//////////////////////////////////OUTPUT FUNCTIONS//////////////////////////////
	

	
	//What we print to the output file at the end of each day
	//Note: change the input names if you want :))
	void PrintEachDay(int CD, int ID, int FD, int WD, int ED ) { 


	}


	void PrintOutput() {

	}


}U_I ;   //create a one global instant of the class as per the singleton technique
		 //(we don't need more than that)


//Some extra definition(s)

enum OUTPUT_MODE {

	INTERACTIVE,
	STEP,
	SILENT

};

#endif 