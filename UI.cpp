#include "UI.h"
using namespace std;


void UI::ReadRoversNumbers(ifstream& Input, int& M, int& P, int& E)
{
	Input >> M >> P >> E;
}

int UI::ReadRoverSpeeds(ifstream& Input)
{
	int speed;
	Input >> speed;
	return speed;
}

void UI::Read_N_CheckupDur(ifstream& Input, int& N, int& CM, int& CP, int& CE)
{
	Input >> N;
	Input >> CM >> CP >> CE;
}

int UI::Read_AutoP(ifstream& Input)
{
	int AutoP;
	Input >> AutoP;
	return AutoP;
}

int UI::Read_EventsNum(ifstream& Input)
{
	int n;
	Input >> n;
	return n;
}

char UI::ReadEventsTyp(ifstream& Input)
{
	char Typ;
	Input >> Typ;
	return Typ;
}

void UI::Read_Formulation(ifstream& Input, char& Rover_Typ, int& ED, int& ID, int& TLOC, int& MDUR, int& SIG)
{
	Input >> Rover_Typ >> ED >> ID >> TLOC >> MDUR >> SIG;
}

void UI::Read_Promotion(ifstream& Input, int& ED, int& ID)
{
	Input >> ED >> ID;
}

void UI::Read_Cancellation(ifstream& Input, int& ED, int& ID)
{
	Input >> ED >> ID;
}
