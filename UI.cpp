#include "UI.h"
using namespace std;

///////////////////////////////////////INPUT//////////////////////////////////////////
void UI::ReadRoversNumbers(ifstream& Input, int& P, int& E)
{
	Input >> P >> E;
}

int UI::ReadRoverSpeeds(ifstream& Input)
{
	int speed;
	Input >> speed;
	return speed;
}

void UI::Read_N_CheckupDur(ifstream& Input, int& N, int& CP, int& CE)
{
	Input >> N;
	Input >> CP >> CE;
}

int UI::Read_MaintDur(ifstream& Input)
{
	int MainDur;
	Input >> MainDur;
	return MainDur;
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

//From Console
void UI::ReadUserChoice(int& choice)
{
	cout << "Please select the program mode, 1 for Interactive Mode, 2 for Step-by-step Mode and 3 for Silent Mode:";
	cin >> choice;
}


///////////////////////////////////////OUTPUT//////////////////////////////////////////

///1- To File: 
void UI::WriteEachDay(int CD, int ID, int FD, int WD, int ED, ofstream& Output)
{
	Output << CD << "    " << ID << "    " << FD << "    " << WD << "    " << ED << endl;
}


void UI::WriteHeader(ofstream& Output)
{
	Output.clear();
	Output << "CD" << "   " << "ID" << "   " << "FD" << "   " << "WD" << "   " << "ED" << endl;
}

void UI::WriteMissions(int M_Mission, int E_Mission, int P_Mission, ofstream& Output) {
	Output << "Missions:" << " " << M_Mission + E_Mission + P_Mission << " " << "[M:" << " " << M_Mission << "," << "" << "P:" << " " << P_Mission << "," << "" << "E:" << " " << E_Mission << "]" << endl;


}

void UI::WriteRovers(int M_Rover, int E_Rover, int P_Rover, ofstream& Output) {

	Output << "Rovers:" << " " << M_Rover + E_Rover + P_Rover << "    " << "[M:" << " " << M_Rover << "," << "" << "P:" << " " << P_Rover << "," << "" << "E:" << " " << E_Rover << "]" << endl;


}

void UI::WriteStats( int Avg_Wait, int Avg_Exec, int Auto_P, ofstream& Output)
{
	Output << "Avg Wait = " << Avg_Wait << "," << " " << "Avg Exec = " << Avg_Exec << endl;
	Output << "Auto-promoted: " << Auto_P << "%" << endl;
	Output.close();
}



///2- To Console:


//Prints the data in their respective format

void UI::PrintEmergency(int* E, int count)
{
	if (count == 0)
		return;
	cout << " " << "[";
	for (int i = 0; i < count; i++)
	{
		if (i == count - 1)
		{
			cout << E[i] << "]";
		}
		else
		{
			cout << E[i] << ",";
		}
	}
}

void UI::PrintPolar(int* P, int count)
{
	if (count == 0)
		return;
	cout << " " << "(";
	for (int i = 0; i < count; i++)
	{
		if (i == count - 1)
		{
			cout << P[i] << ")";
		}
		else
		{
			cout << P[i] << ",";
		}
	}
}


//Prints in-EXECUTION M/R
void UI::PrintMissionsAndRovers(int* M, int* R, int count, char type)
{
	if (count == 0)
		return;
	cout << " ";
	if (type == 'E')
		cout << "[";
	else cout << "(";
	for (int i = 0; i < count; i++)
	{
		if (i == count - 1)
		{
			cout << M[i] << "/" << R[i];
			if (type == 'E')
				cout << "]";
			else cout << ")";
		}
		else
		{
			cout << M[i] << "/" << R[i] << ",";
		}
	}
}


void UI::PrintBreakLine()
{
	cout << endl;
	for (int i = 0; i < 40; i++)
	{
		cout << "-";
	}
	cout << endl;
}



//Since the InExecution Missions and Rovers are each in 2 PQ's 
//where each PQ contain All the types (M,P,E)
//then we have to classify them first before doing the operation
//Assume that M[] , R[] are arrays that are filled blindly with all ID's irrespective of the types

void UI::PrintInExecution(int* M, int* R, int count, int cE, int cP, char* type)
{

	int* EM = new int[cE]; int* ER = new int[cE]; int c1 = 0;
	int* PM = new int[cP]; int* PR = new int[cP]; int c3 = 0;
	for (int i = 0; i < count; i++)
	{
		if (type[i] == 'E')
		{
			EM[c1] = M[i];
			ER[c1] = R[i];
			c1++;
		}
		else
		{
			PM[c3] = M[i];
			PR[c3] = R[i];
			c3++;
		}
	}
	PrintMissionsAndRovers(EM, ER, cE, 'E');
	PrintMissionsAndRovers(PM, PR, cP, 'P');
}



//Since the Rovers in Checkup are in a PQ that contains all types of Rovers
//then we have to classify the rovers first before doing the operation
//Assume that R[] is an array that is filled blindly with all ID's irrespective of the types. 

void UI::PrintInCheckup(int* R, int count, int cE, int cP, char* type)
{
	int* ER = new int[cE]; int c1 = 0;
	int* PR = new int[cP]; int c3 = 0;
	for (int i = 0; i < count; i++)
	{
		if (type[i] == 'E')
		{
			ER[c1] = R[i];
			c1++;
		}
		else
		{
			PR[c3] = R[i];
			c3++;
		}
	}
	PrintEmergency(ER, cE);
	PrintPolar(PR, cP);
}


//TODO: 
void UI::PrintInMaint(int* R, int count, int mE, int mP, char* type) {


}

//Since making a function to print each day in the UI would require nearly 15 input
//to make the function in the master station we had to handle the statements printed to illustrate the output
//choice determines which statements should be printed
//x is the number printed in the statement(Current day, number of waiting missions, etc)
void UI::PrintStatements(int choice, int x)
{
	switch (choice)
	{
	case 1:
		cout << "Current Day:" << x << endl;
		break;
	case 2:
		cout << x << " " << "Waiting Missions:";
		break;
	case 3:

		//TODO: Add something about failed missions
		PrintBreakLine();
		cout << x << " " << "In-Execution Missions/Rovers:";
		cout << " ";
		break;
	case 4:
		PrintBreakLine();
		cout << x << " " << "Available Rovers:";
		break;
	case 5:
		PrintBreakLine();
		cout << x << " " << "In-Checkup Rovers:";
		cout << " ";
		break;
	case 6:
		PrintBreakLine();
		cout << x << " " << "Completed Missions:";
		break;
	}
}

void UI::InteractiveMode()
{
	system("pause");
}

void UI::StepByStepMode()
{
	Sleep(1000);
}


//in silent mode you don't have to do the process at all
void UI::SilentMode()
{
	cout << "Silent Mode" << endl;
	cout << "Simulation Starts..." << endl;
	cout << "Simulation ends, Output file created" << endl;
}