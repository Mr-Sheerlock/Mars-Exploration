#ifndef  UI_H
#define UI_H

#include <fstream>
#include <iostream>
#include <Windows.h>


using namespace std;

class UI {


public:
	///////////////////////////////////INPUT FUNCTIONS//////////////////////////////

	void ReadRoversNumbers(ifstream& Input, int& P, int& E);

	int ReadRoverSpeeds(ifstream& Input); //returns the speed of each rover

	void Read_N_CheckupDur(ifstream& Input, int& N, int& CP, int& CE); //number of missions before checkup and the checkup duration of each type

	int Read_MaintDur(ifstream& Input);

	int Read_EventsNum(ifstream& Input);

	char ReadEventsTyp(ifstream& Input);

	void Read_Formulation(ifstream& Input, char& Rover_Typ, int& ED, int& ID, int& TLOC, int& MDUR, int& SIG);


	//From Console
	void ReadUserChoice(int& choice);


	//////////////////////////////////OUTPUT FUNCTIONS//////////////////////////////

	//1-To File:

	//What we print to the output file at the end of each day
	void WriteEachDay(int CD, int ID, int FD, int WD, int ED, ofstream& Output);

	void WriteHeader(ofstream& Output);

	void WriteMissions(int E_Mission, int P_Mission, ofstream& Output);

	void WriteRovers(int E_Rover, int P_Rover, ofstream& Output);

	void WriteStats(float Avg_Wait, float Avg_Exec, ofstream& Output);

	void WritePRoversWarning(ofstream& Output);

	void WriteNoMissionsWarning(ofstream& Output);

	void WriteCriticalWarning(ofstream& Output);

	void WriteProbabilityWarning(ofstream& Output);

	//2-To Console:

	//prints the given parameter according to the format specified for emergency
	void PrintEmergency(int* E, int count);

	//prints the given parameter according to the format specified for polar
	void PrintPolar(int* P, int count);

	//prints the given parameter according to the format specified for Missions/Rovers
	void PrintMissionsAndRovers(int* M, int* R, int count, int* F, int Fcount, char type);

	void PrintBreakLine();

	void PrintInExecution(int* M, int* R, int count, int cE, int cP, char* type, int* F, int FCount, int FP, int FE, char* FType);

	void PrintEmergencyAndPolar(int* R, int count, int cE, int cP, char* type);

	void PrintStatements(int choice, int x);

	void PrintFilenotFound();

	void InteractiveMode();

	void StepByStepMode();

	void SilentMode();







};

#endif 