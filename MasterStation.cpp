#include "MasterStation.h"
#include "Events/Event Headers.h"

#include <stdlib.h>  //for rand
#include <time.h>    // for time(0)
#include <cmath>	 // for exp and log 

MasterStation::MasterStation()
{

	CurrentDay = 1;

	//Missions Counters
	N_Missions = 0;
	NMissionsE=0;

	WaitingMissionsP = 0;
	WaitingMissionsE=0;
	
	NExecMiss=0;
	NExecMissE = 0;


	//Rovers
	NRoversE=0;
	NRoversP=0;

	//They get initialized once more in the ReadRovers
	AvRoversE = 0;
	AvRoversP=0;

	NExecRovs=0;

	CheckupRovers = 0;
	CheckupRoversE=0;


	MaintRovers = 0;
	MaintRoversE=0;

	Failed_P_Rover = 0;
	Failed_E_Rover = 0;

	DailyCompletedCount = 0;
	DailyCompletedCountE = 0;

	TotalCompleted = 0;
	////////////////////////////////FAILURE///////////////////////////////////////////////

	ProbabilityOFfailure = 5; // in percent

	//////////////////////////////////////////////////////////////////////////////////////
	
	//Statistics
	Total_Wait = 0;

	Total_InExecution = 0;

	IO_Interface = new UI();
	Output.open("Output.txt");


	//Lists

	EventList = new Queue<Event*>();

	Available_P_Rovers = new PriorityQueue<P_Rover*>(); //According to speed
	Available_E_Rovers = new PriorityQueue<E_Rover*>();

	Waiting_E_Missions = new PriorityQueue<E_Mission*>;

	Waiting_P_Missions = new Queue<P_Mission*>;


	N_Execution_Rovers= new PriorityQueue<Rover*>;   

	N_Execution_Missions= new PriorityQueue<Mission*>;

	Checkup_Rovers = new PriorityQueue<Rover*>;

	Maintainance_Rovers = new PriorityQueue<Rover*>;

	Waiting_P_Missions= new Queue<P_Mission*>;



	srand(time(NULL) + N_Missions); // ensures that each run we get a new random output regarding the failure
	
	//Output
	SilentModeFlag = true;

}


MasterStation::~MasterStation()
{
	delete IO_Interface;
	delete EventList;
	E_Rover* TempE;
	while (!Available_E_Rovers->isEmpty())
	{
		Available_E_Rovers->Dequeue(TempE);
		delete TempE;
	}
	delete Available_E_Rovers;
	P_Rover* TempP;
	while (!Available_P_Rovers->isEmpty())
	{
		Available_P_Rovers->Dequeue(TempP);
		delete TempP;
	}
	delete Available_P_Rovers;
	delete N_Execution_Missions;
	delete N_Execution_Rovers;
	delete Waiting_E_Missions;
	delete Waiting_P_Missions;
	Rover* TempR;
	while (!Checkup_Rovers->isEmpty())
	{
		Checkup_Rovers->Dequeue(TempR);
		delete TempR;
	}
	delete Checkup_Rovers;
	while (!Maintainance_Rovers->isEmpty())
	{
		Maintainance_Rovers->Dequeue(TempR);
		delete TempR;
	}
	delete Maintainance_Rovers;
}

////////Data member Getters / incrementers for events

PriorityQueue<E_Mission*>* MasterStation::ReturnWaitingEmerg()
{
	return Waiting_E_Missions;
}

Queue<P_Mission*>* MasterStation::ReturnWaitingPolar()
{
	return Waiting_P_Missions;
}

void MasterStation::IncrementWaitingPolarCount() {
	WaitingMissionsP++;
}

void MasterStation::IncrementWaitingEmerCount(){
	WaitingMissionsE++;
}

int MasterStation::Get_N_RoversP()
{

	return NRoversP;
}

////////////////////////////////INPUT///////////////////////////////////

void MasterStation::ReadInputFile()
{
	Input.open("input.txt");

	if (Input.is_open())
	{
		ReadRovers();
		ReadCheckupInfo();
		ReadMaintDur();
		ReadEvents();
		Input.close();

	}
	else
	{
		IO_Interface->PrintFilenotFound();
	}
}

void MasterStation::ReadRovers()
{
	int UniqueID; //used to find a unique id for each rover
	UniqueID = 1;

	IO_Interface->ReadRoversNumbers(Input, NRoversP, NRoversE); // waiting rovers of each type

	//Initialization
	AvRoversE = NRoversE;
	AvRoversP = NRoversP;

	int speed;
	
	for (int i = 0; i < NRoversP; i++)
	{
		//create new Polar rover and set its ID
		//add it in the queue
		speed = IO_Interface->ReadRoverSpeeds(Input);
		P_Rover* New_P_Rover = new P_Rover(UniqueID, speed);
		Available_P_Rovers->Enqueue(New_P_Rover, speed);
		UniqueID++;
	}
	for (int i = 0; i < NRoversE; i++)
	{
		//create new Emergency rover and set its ID
		//add it in the queue
		speed = IO_Interface->ReadRoverSpeeds(Input);
		E_Rover* New_E_Rover = new E_Rover(UniqueID, speed);
		Available_E_Rovers->Enqueue(New_E_Rover, speed);
		UniqueID++;
	}
}


void MasterStation::ReadCheckupInfo()
{
	int N, CP, CE;
	IO_Interface->Read_N_CheckupDur(Input, N, CP, CE);
	Rover::SetMissionsB4Checkup(N);
	P_Rover::Set_CheckupD(CP);
	E_Rover::Set_CheckupD(CE);
}

void MasterStation::ReadMaintDur()
{
	int MainDur;
	MainDur = IO_Interface->Read_MaintDur(Input);
	Rover::setMaintDur(MainDur);
}



void MasterStation::ReadEvents()
{
	int EventsNum;

	EventsNum = IO_Interface->Read_EventsNum(Input);

	char TYP;
	char Miss_Typ; //Mission type P, or E
	int ED, ID, TLOC, MDUR, SIG;
	for (int i = 0; i < EventsNum; i++)
	{
		TYP = IO_Interface->ReadEventsTyp(Input);
		if (TYP == 'F')
		{
			N_Missions++;
			IO_Interface->Read_Formulation(Input, Miss_Typ, ED, ID, TLOC, MDUR, SIG);
			//create a new formulatoin event and add it in the EventList
			Formulation_Event* newFormEv = new Formulation_Event(ED, ID, Miss_Typ,   TLOC, MDUR, SIG);
			if (Miss_Typ == 'E') {
				NMissionsE++;
			}
			EventList->Enqueue(newFormEv);
		}
		
	}

}


bool MasterStation::CheckConsistency() {
	
	//if Probability of Failure =100%
	if (ProbabilityOFfailure == 100) {
		IO_Interface->WriteProbabilityWarning(Output);
		return false;
	}

	//We here process the corner case where there are missions but no corresponding rovers

	//If there are no rovers, return false to end the simulation abruptly
	if (NMissionsE!=0 && (NRoversE + NRoversP ==0)) {
		IO_Interface->WriteCriticalWarning(Output);
		return false;
	}

	//If there is polar missions but no polar rovers, we run the simulation normally but we don't formulate them
	if ((N_Missions -NMissionsE != 0) && NRoversP == 0) {

		if (NRoversE!=0) {
			//and call UI that at the end there was no Rovers to finish the Polar missions
			IO_Interface->WritePRoversWarning(Output);
		}
		
	}
	IO_Interface->WriteHeader(Output);
	return true;



}


///////////////////////////OUTPUT//////////////////////////////////////
//1-To Console
void MasterStation::PrintEachDay()
{

	int* WaitingEIds = NULL;
	if (!(Waiting_E_Missions->isEmpty()))//in case of empty list(no waiting emergency missions)
	{
		WaitingEIds = new int[WaitingMissionsE];
		TakeIdsFromWaitingE(WaitingEIds);  //initializes the array
	}

	
	 int* WaitingPIds = NULL;
	if (!(Waiting_P_Missions->IsEmpty()))
	{
		WaitingPIds = new int[WaitingMissionsP];
		TakeIdsFromWaitingP(WaitingPIds); //initializes the array
	}

	int* InExecution_M_Ids = NULL, * InExecution_R_Ids = NULL;
	char* N_Exectype = NULL;
	int* F_InExecution_R_Ids = NULL;
	char* Ftype = NULL;

	if (!(N_Execution_Missions->isEmpty()))
	{
		InExecution_M_Ids = new int[NExecMiss];
		InExecution_R_Ids = new int[NExecMiss]; //we assume that they are the same size and handle the failed mission rovers in another array

		F_InExecution_R_Ids = new int[NExecRovs - NExecMiss]; //Rovers' numbers are always higher	than missions
		Ftype = new char[NExecRovs - NExecMiss]; //difference is the number of rovers who has failed missions

		N_Exectype = new char[NExecMiss];


		TakeInfoFromInExecution(InExecution_M_Ids, InExecution_R_Ids, N_Exectype, F_InExecution_R_Ids, Ftype);
	}
	else {
		//in this case, all the rovers are failed rovers
		F_InExecution_R_Ids = new int[NExecRovs ]; //NexecMiss=0
		Ftype = new char[NExecRovs]; //NexecMiss=0
		TakeFailedFromInExecRov( F_InExecution_R_Ids, Ftype);
	}

	

	int* AvEmergency_Ids = NULL;
	if (!(Available_E_Rovers->isEmpty()))
	{
		AvEmergency_Ids = new int[AvRoversE];
		TakeIdsFromAvailableE(AvEmergency_Ids);
	}


	int* AvPolar_Ids = NULL;
	if (!(Available_P_Rovers->isEmpty()))
	{
		AvPolar_Ids = new int[AvRoversP];
		TakeIdsFromAvailableP(AvPolar_Ids);
	}


	int* InCheckupIds = NULL; char* InCheckuptype = NULL;
	if (!(Checkup_Rovers->isEmpty()))
	{
		InCheckupIds = new int[CheckupRovers];
		InCheckuptype = new char[CheckupRovers];
		TakeInfoFromInCheckup(InCheckupIds, InCheckuptype);
		
	}
	
	int* InMaintIds = NULL; char* InMaintType = NULL;
	if (!(Maintainance_Rovers->isEmpty()))
	{
		InMaintIds = new int[MaintRovers];
		InMaintType = new char[MaintRovers];
		TakeInfoFromInMaint(InMaintIds, InMaintType);
	}
	
	///////////////////////////////////////////////////
	
	IO_Interface->PrintStatements(1, CurrentDay);
	//Waiting Missions
	int TotalWaiting = WaitingMissionsE + WaitingMissionsP;
	IO_Interface->PrintStatements(2, TotalWaiting);
	IO_Interface->PrintEmergency(WaitingEIds, WaitingMissionsE);
	IO_Interface->PrintPolar(WaitingPIds, WaitingMissionsP);
	
	//InExecution Missions/Rovers AND Failed Rovers

	
	IO_Interface->PrintStatements(3, NExecMiss);

	int NExecMissP = NExecMiss - NExecMissE;

	int FailedRovers = NExecRovs - NExecMiss;

	IO_Interface->PrintInExecution(InExecution_M_Ids, InExecution_R_Ids, NExecMiss, NExecMissE, NExecMissP, N_Exectype, F_InExecution_R_Ids, FailedRovers, Failed_P_Rover, Failed_E_Rover, Ftype);

	//Available Rovers
	IO_Interface->PrintStatements(4, AvRoversE + AvRoversP);
	IO_Interface->PrintEmergency(AvEmergency_Ids, AvRoversE);
	IO_Interface->PrintPolar(AvPolar_Ids, AvRoversP);

	//InCheckup Rovers
	int CheckupRoversP = CheckupRovers - CheckupRoversE;

	IO_Interface->PrintStatements(5, CheckupRovers);
	//IO_Interface->PrintInCheckup(InCheckupIds, CheckupRovers, CheckupRoversE, CheckupRoversP, InCheckuptype);
	IO_Interface->PrintEmergencyAndPolar(InCheckupIds, CheckupRovers, CheckupRoversE, CheckupRoversP, InCheckuptype);

	//Maintainance Rovers
	int MaintRoversP = MaintRovers - MaintRoversE;
	IO_Interface->PrintStatements(6, MaintRovers);
	//IO_Interface->PrintInMaint(InMaintIds, MaintRovers, MaintRoversE, MaintRoversP, InMaintType);
	IO_Interface->PrintEmergencyAndPolar(InMaintIds, MaintRovers, MaintRoversE, MaintRoversP, InMaintType);

	//CompletedMissions
	int DailyCompletedCountP = DailyCompletedCount - DailyCompletedCountE;
	
	IO_Interface->PrintStatements(7, DailyCompletedCount);
	//IO_Interface->PrintInCheckup(DailyCompMissionsIDs, DailyCompletedCount, DailyCompletedCountE, DailyCompletedCountP, DailyCompMissionsType);
	IO_Interface->PrintEmergencyAndPolar(DailyCompMissionsIDs, DailyCompletedCount, DailyCompletedCountE, DailyCompletedCountP, DailyCompMissionsType);

	IO_Interface->PrintBreakLine();

	//DONT FORGET TO DEALLOCATE THE MEMORY 
	
	delete[] WaitingEIds;
	delete[] WaitingPIds;
	delete[] InExecution_M_Ids;
	delete[] InExecution_R_Ids;
	delete[] F_InExecution_R_Ids;
	delete[] N_Exectype;
	delete[] Ftype;
	delete[] AvEmergency_Ids;
	delete[] AvPolar_Ids;
	delete[] InCheckupIds;
	delete[] InCheckuptype;
	delete[] InMaintIds;
	delete[] InMaintType;


}

void MasterStation::TakeIdsFromWaitingE( int * Ids)
{
	Queue<E_Mission*>temp;//missions are stored temporarily in queue instead
	E_Mission* item;      //of priority queue to reduce complexity

	int i = 0;
	while (Waiting_E_Missions->Dequeue(item))
	{
		temp.Enqueue(item);
		Ids[i] = item->GetID();
		i++;
	}
	while (temp.Dequeue(item))
	{
		Waiting_E_Missions->Enqueue(item, item->GetPriority());
	}
}

void MasterStation::TakeIdsFromWaitingP(int* Ids)
{
	Queue<P_Mission*>temp;
	P_Mission* item;
	int i = 0;
	while (Waiting_P_Missions->Dequeue(item)) {
		temp.Enqueue(item);
		Ids[i] = item->GetID();
		i++;
	}
	while (temp.Dequeue(item))
	{
		Waiting_P_Missions->Enqueue(item);
	}
}

void MasterStation::TakeInfoFromInExecution( int* M, int* R, char* RM, int* FR, char*FT )
{
	Queue<Rover*> t;
	Mission* tempmission; Rover* temprover;

	int i = 0, j=0;
	while (N_Execution_Rovers->Dequeue(temprover))
	{
			tempmission = temprover->GetAssignedMission();
			
			//if rover is assigned 
			if (tempmission) {
				R[i]=temprover->GetID();
				M[i] = tempmission->GetID();
				RM[i] = tempmission->GetTYP();
				i++;
			}//if rover is getting back from a failed mission
			else {
				FR[j] = temprover->GetID();
				FT[j] = temprover->GetType();
				j++;
			}

			t.Enqueue(temprover);
	}
	while (t.Dequeue(temprover))
	{
		N_Execution_Rovers->Enqueue(temprover, (temprover->GetCompletionDay() * -1));
	}
}

void MasterStation::TakeFailedFromInExecRov(int* FR, char* FT)
{
	Queue<Rover*> t;
	Rover* temprover;

	int j = 0;
	//in this case, all the rovers are failed rovers
	while (N_Execution_Rovers->Dequeue(temprover))
	{
		FR[j] = temprover->GetID();
		FT[j] = temprover->GetType();
		j++;
		t.Enqueue(temprover);
	}
	
	while (t.Dequeue(temprover))
	{
		N_Execution_Rovers->Enqueue(temprover, (temprover->GetCompletionDay() * -1));
	}
}


void MasterStation::TakeIdsFromAvailableE(int* id)
{
	Queue<E_Rover*>E;
	E_Rover* etemp;
	int i = 0;
	while (Available_E_Rovers->Dequeue(etemp))
	{
		id[i]=etemp->GetID();
		E.Enqueue(etemp);
		i++;
	}
	while (E.Dequeue(etemp))
	{
		Available_E_Rovers->Enqueue(etemp, etemp->GetSpeed());
	}
}

void MasterStation::TakeIdsFromAvailableP(int* id)
{
	Queue<P_Rover*>Q;
	P_Rover* ptemp;
	int i = 0;
	while (Available_P_Rovers->Dequeue(ptemp))
	{
		id[i]=ptemp->GetID();
		Q.Enqueue(ptemp);
		i++;
	}
	while (Q.Dequeue(ptemp))
	{
		Available_P_Rovers->Enqueue(ptemp, ptemp->GetSpeed());
	}
}


void MasterStation::TakeInfoFromInCheckup(int* id, char* type)
{
	Queue<Rover*>temp;
	Rover* rtemp;
	int i = 0;
	while (Checkup_Rovers->Dequeue(rtemp))
	{
		char x = rtemp->GetType();
		id[i]=rtemp->GetID();
		type[i]=x;
		i++;
		temp.Enqueue(rtemp);
	}
	while (temp.Dequeue(rtemp))
	{
		Checkup_Rovers->Enqueue(rtemp, (rtemp->GetCheckUpCompletionDay() * -1));
	}
}

void MasterStation::TakeInfoFromInMaint(int* id, char* type)
{
	Queue<Rover*>temp;
	Rover* rtemp;
	int i = 0;
	while (Maintainance_Rovers->Dequeue(rtemp))
	{
		char x = rtemp->GetType();
		id[i]=rtemp->GetID();
		type[i]=x;
		i++;
		temp.Enqueue(rtemp);
	}
	while (temp.Dequeue(rtemp))
	{
		Maintainance_Rovers->Enqueue(rtemp, (rtemp->GetMaintCompletionDay()* -1));
	}
}
void MasterStation::FinalOutput()
{
	if (UserChoice == 1 || UserChoice == 2)
	{
		
		PrintEachDay();
		if (UserChoice == 1)
			IO_Interface->InteractiveMode();
		else IO_Interface->StepByStepMode();
	}
	else
	{
		if (SilentModeFlag) {
			//Prints it once in the beginning of the simulation
			IO_Interface->SilentMode();
			SilentModeFlag = false;
		}
	}
}

void MasterStation::ReadUserChoice()
{
	IO_Interface->ReadUserChoice(UserChoice);
}


///////////////////////////Operations//////////////////////////////////////


void MasterStation::ExecuteDay() {
	
	//Proposed Order: Events->Rovers Arrival->Rovers Checkup-> Rovers Maintainance->
	//->Missions Assignment

		//Very important change handling the corner case where a mission might finish in the sama day 
		DailyCompMissionsIDs= new int [N_Missions- TotalCompleted];
		DailyCompMissionsType = new char[N_Missions- TotalCompleted];
	
	
	ExecuteEvents();
	Checkfailed();
	CheckRoverArrived();
	CheckMissionComplete();
	CheckupComplete();
	Maint_Complete();
	AssignMission();
	FinalOutput();
	CurrentDay++;
	DailyCompletedCount = 0;
	DailyCompletedCountE = 0;
	
	delete[] DailyCompMissionsIDs;
	delete[] DailyCompMissionsType;
	DailyCompMissionsIDs = nullptr;
	DailyCompMissionsType = nullptr;
}



bool MasterStation::CheckLastDay() {
	if (EventList->IsEmpty() && Waiting_E_Missions->isEmpty() && Waiting_P_Missions->IsEmpty() && N_Execution_Missions->isEmpty())
	{
		// Last Day, its position in this function is subject to change.
		float AvgWait = 0;
		float AvgExec = 0;
		CalculateStats(AvgWait, AvgExec);
		IO_Interface->WriteMissions(NMissionsE, N_Missions - NMissionsE, Output);
		IO_Interface->WriteRovers(NRoversE, NRoversP, Output);
		if (CalculateStats(AvgWait, AvgExec))

		{
			IO_Interface->WriteStats(AvgWait, AvgExec, Output);
		}
		else {
			IO_Interface->WriteNoMissionsWarning(Output);
		}
		return true;
		return true;
	}
	return false;
}

//Events
void MasterStation::ExecuteEvents()
{
	Event* Executed_Event;
	if (EventList->IsEmpty())
	{
		return;
	}
	Event* Ev;
	while (EventList->Peek(Ev) && Ev->getEventDay() == CurrentDay)
	{
		EventList->Dequeue(Executed_Event);
		Executed_Event->Execute(this);
		delete Executed_Event;
		Executed_Event = nullptr;
	}
}


//Mission Assignment
void MasterStation::AssignMission() {
	int i = CurrentDay;
	bool flag = true;//to break from while loop if no more missions can be assigned


	

	//1-Assigning E-Missions first 
	while (!(Waiting_E_Missions->isEmpty()) && flag)
	{
		E_Rover* E; P_Rover* P;
		E_Mission* emission;

		// if available ERover assign it
		if (Available_E_Rovers->Dequeue(E))
		{
			Waiting_E_Missions->Dequeue(emission);
			emission->AssignRover(E);
			MoveFromWaitingToInExecution(emission, E);
			AvRoversE--;
		}
		//Else look for a PRover
		else if (Available_E_Rovers->isEmpty() && Available_P_Rovers->Dequeue(P))
		{
			Waiting_E_Missions->Dequeue(emission);
			emission->AssignRover(P);
			MoveFromWaitingToInExecution(emission, P);
			AvRoversP--;
		}
		//If both are empty start checking the maintainance
		else if (Available_E_Rovers->isEmpty() && Available_P_Rovers->isEmpty())
		{
			Waiting_E_Missions->Peek(emission);
			//if the mission has been waiting for more than 5 days, search for a rover from maintainance
			//check is only done on the first mission to preserve mission assignment criteria (emission of higher priority assigned first) 
			if ((CurrentDay - emission->GetFormulationDay()) > 5)
			{
				Rover* rover;
				GetRoverFromMaintenance(rover, 'E');
				if (rover)//a suitable rover is found
				{
					Waiting_E_Missions->Dequeue(emission);
					emission->AssignRover(rover);
					MoveFromWaitingToInExecution(emission, rover);
				}
				else//no suitable rover found
				{
					//it is impossible to assign any other emission this day, flag is set to false to exit while loop 
					flag = false;
				}
			}
			else
			{
				flag = false;//to break from while loop
			}
		}
	}
	
	//2-Assigning P-Missions

	//if flag was set to falsee from first while loop, no available PRovers
	//flag is reset to true to check mission's chance in getting assigned to a rover from maintainance if mission has waited long enough

	flag = true;
	while (!(Waiting_P_Missions->IsEmpty()) && flag)
	{
		P_Rover* prover;
		P_Mission* pmission;
		//if there is a polar rover
		if (Available_P_Rovers->Dequeue(prover))
		{
			Waiting_P_Missions->Dequeue(pmission);
			pmission->AssignRover(prover);
			MoveFromWaitingToInExecution(pmission, prover);
			AvRoversP--;
		}
		//if there isn't a polar rover, try to see maintainance
		else
		{
			Waiting_P_Missions->Peek(pmission);
			//checks if waiting days is greater than 5
			if ((CurrentDay - pmission->GetFormulationDay()) > 5)
			{
				Rover* rover;
				GetRoverFromMaintenance(rover, 'P');
				if (rover)//a suitable rover was found
				{
					Waiting_P_Missions->Dequeue(pmission);
					prover = dynamic_cast<P_Rover*>(rover);
					pmission->AssignRover(prover);
					MoveFromWaitingToInExecution(pmission, prover);
				}
				else//no suitable rover was found
				{
					flag = false;//to break from while loop
				}
			}
			else//mission didn't wait long enough, it can wait for an available rover
			{
				flag = false; // to break from while loop
			}
		}
	}
}



void MasterStation::CheckMissionComplete()
{
	//If a mission is completed:
	//print Mission Done,...etc
	Mission* TempMission;
	while (N_Execution_Missions->Peek(TempMission))
	{
		if (TempMission->GetCompletionDay() == CurrentDay)//mission is completed
		{
			N_Execution_Missions->Dequeue(TempMission);
			//calculate important sums for the stats
			Total_Wait = Total_Wait + TempMission->GetWaitingDays();
			Total_InExecution = Total_InExecution + TempMission->GetExecutionDays();
			
			TempMission->UnAssignRover();
			
			//Write Mission Done info in the output file
			IO_Interface->WriteEachDay(TempMission->GetCompletionDay(), TempMission->GetID(), TempMission->GetFormulationDay(), TempMission->GetWaitingDays(), TempMission->GetExecutionDays(), Output);
			
			//getting id and type to be used to print on console 
			DailyCompMissionsIDs[DailyCompletedCount]=TempMission->GetID();
			DailyCompMissionsType[DailyCompletedCount]=TempMission->GetTYP();
			
			//incrementing and decrementing suitable counters
			NExecMiss--;
			DailyCompletedCount++;
			TotalCompleted++;
			if (TempMission->GetTYP() == 'E') {
				DailyCompletedCountE++;
				NExecMissE--;
			}
			
		}
			//First one didn't finish, do nothing
		else 
		{
				return;
		}
	}

}


void MasterStation::CheckRoverArrived()
{
	Rover* TempRover;
	CheckupComplete();
	while (N_Execution_Rovers->Peek(TempRover))
	{
		if (TempRover->GetCompletionDay() == CurrentDay)
		{
			N_Execution_Rovers->Dequeue(TempRover);
			Mission* AssignedMission = TempRover->GetAssignedMission();

			if (AssignedMission == nullptr) //In case of failure
			{
				MoveFromInExecutionToCheckup(TempRover);
				if (TempRover->GetType() == 'E') {
					Failed_E_Rover--;
				}
				else {
					Failed_P_Rover--;
				}
			}
			else
			{
				UpdateRover(TempRover, AssignedMission);
				DoesItNeedCheckUp(TempRover);
				bool Maintenance = false;
				bool Checkup = false;
				Maintenance = CheckMaint(TempRover);
				if (TempRover->GetNeedCheck() == 1)
				{
					Checkup = true;
				}
				else
				{
					Checkup = false;
				}

				if (Checkup && Maintenance)
				{
					//Needs both maintenance and checkup
					//Reset Checkup values
					TempRover->SetMissionsDone(0);
					MoveFromInExecutionToMaintenance(TempRover);
				}
				else if (!Checkup && !Maintenance)
				{
					//Doesn't need Maintenance or checkup
					MoveFromInExecutionToAvailable(TempRover);
				}
				else if (Checkup)
				{
					//Checkup only
					MoveFromInExecutionToCheckup(TempRover);
				}
				else if (Maintenance)
				{
					//Maintenance only
					MoveFromInExecutionToMaintenance(TempRover);
				}
			}
		}
		else
		{
			return;
		}
	}
}


//Searches for the approporiate rover in Maintianance (According to Speed)
void MasterStation::SearchForPolar(P_Rover*& NeededPRover, Queue<Rover*>*& TempQ)
{
	NeededPRover = nullptr;
	Rover* rover;
	while (!Maintainance_Rovers->isEmpty())
	{
		Maintainance_Rovers->Dequeue(rover);
		TempQ->Enqueue(rover);
		if (rover->GetType() == 'P')
		{
			if (rover->GetSpeed() > 1 && (rover->GetMaintCompletionDay() - CurrentDay) != 1)
			{
				if (NeededPRover)
				{
					if (rover->GetSpeed() > NeededPRover->GetSpeed())
					{
						NeededPRover = (P_Rover*)rover;
					}
				}
				else
				{
					NeededPRover = (P_Rover*)rover;
				}
			}
		}
	}
}

//Searches for the approporiate rover in Maintianance (According to Speed)
void MasterStation::SearchForPolar_Emergency(E_Rover*& NeededERover, P_Rover*& NeededPRover, Queue<Rover*>*& TempQ)
{
	Rover* rover;
	NeededERover = nullptr;
	NeededPRover = nullptr;
	while (!Maintainance_Rovers->isEmpty())
	{
		Maintainance_Rovers->Dequeue(rover);
		TempQ->Enqueue(rover);
		if (rover->GetType() == 'P')
		{
			if (rover->GetSpeed() > 1 && (rover->GetMaintCompletionDay() - CurrentDay) != 1)
			{
				if (NeededPRover)
				{
					if (rover->GetSpeed() > NeededPRover->GetSpeed())
					{
						NeededPRover = (P_Rover*)rover;
					}
				}
				else
				{
					NeededPRover = (P_Rover*)rover;
				}
			}
		}
		if (rover->GetType() == 'E')
		{
			if (rover->GetSpeed() > 1 && (rover->GetMaintCompletionDay() - CurrentDay) != 1)
			{
				if (NeededERover)
				{
					if (rover->GetSpeed() > NeededERover->GetSpeed())
					{
						NeededERover = (E_Rover*)rover;
					}
				}
				else
				{
					NeededERover = (E_Rover*)rover;
				}
			}
		}
	}
}




void MasterStation::ReturnToMaint(Queue<Rover*>* TempQ)
{
	Rover* rover;
	while (!TempQ->IsEmpty())
	{
		TempQ->Dequeue(rover);
		Maintainance_Rovers->Enqueue(rover, rover->GetMaintCompletionDay());
	}
}

void MasterStation::ReturnToMaint_FindRover(Rover* RoverToFind, Queue<Rover*>* TempQ)
{
	Rover* rover;
	while (TempQ->IsEmpty())
	{
		TempQ->Dequeue(rover);
		if (rover->GetID() != RoverToFind->GetID())
		{
			Maintainance_Rovers->Enqueue(rover, rover->GetMaintCompletionDay());
		}
	}
}

void MasterStation::CalculateArrive2Target(Rover* rover, int Tloc)
{
	float speed = (float)rover->GetSpeed();
	int time = Tloc / speed;
	int timeindays = ceil(time / 25);
	rover->SetArrive2Target(timeindays);
}


//Checkup and Maintainance


bool MasterStation::GetRoverFromMaintenance(Rover*& RoverNeeded, char Type)
{
	RoverNeeded = NULL;
	if (Maintainance_Rovers->isEmpty())
	{
		return false;
	}
	else
	{
		//Restrictions:
		//Min Speed = 1
		// If only 1 day is left to complete the maintenance
		Queue <Rover*>* TempQ = new Queue<Rover*>;
		P_Rover* PolarRover = nullptr;
		E_Rover* EmergencyRover = nullptr;
		if (Type == 'P')
		{
			//search for polar only
			SearchForPolar(PolarRover, TempQ);
			if (!PolarRover)
			{
				//Return to maintenance
				ReturnToMaint(TempQ);
				return false;
			}
			else
			{
				//Return to maintenance and find rover
				ReturnToMaint_FindRover(PolarRover, TempQ);
				//Reset Maintenance values
				PolarRover->ResetMaintValues();
				//Half the speed
				PolarRover->SetSpeed(PolarRover->GetSpeed() / 2);
				RoverNeeded = PolarRover;
				MaintRovers--;
				return true;
			}
		}
		else
		{
			//search for both polar and emergency
			SearchForPolar_Emergency(EmergencyRover, PolarRover, TempQ);
			if (EmergencyRover)
			{
				//Return to maintenance and find rover
				ReturnToMaint_FindRover(EmergencyRover, TempQ);
				//Reset Maintenance values
				PolarRover->ResetMaintValues();
				//Half the speed
				EmergencyRover->SetSpeed(EmergencyRover->GetSpeed() / 2);
				RoverNeeded = EmergencyRover;
				MaintRovers--;
				MaintRoversE--;
				return true;
			}
			else if (PolarRover)
			{
				ReturnToMaint_FindRover(PolarRover, TempQ);
				//Reset Maintenance values
				PolarRover->ResetMaintValues();
				//Half the speed
				PolarRover->SetSpeed(PolarRover->GetSpeed() / 2);
				RoverNeeded = PolarRover;
				MaintRovers--;
				return true;
			}
			else
			{
				ReturnToMaint(TempQ);
				return false;
			}
		}
		delete TempQ;
	}
}

void MasterStation::UpdateRover(Rover* rover, Mission* mission)
{
	//For checkup:
	rover->SetMissionsDone(rover->GetMissionsDone() + 1);
	//For maintenance:
	rover->UpdateTotalMDur(mission->GetDuration());
	rover->UpdateTotalDist(mission->GetTLOC());
	if (mission->GetSIG() > 7) //check if a mission has a high signifance value
	{
		rover->IncrementHighSigMissNum();
	}
	rover->UpdateHealth();
	rover->SetAssignedMission(NULL);
}

bool MasterStation::CheckMaint(Rover* rover)
{
	if (rover->GetHealth() <= 0)
	{
		rover->ResetMaintValues();
		return true;
	}
	return false;
}

void MasterStation::Maint_Complete()
{
	if (!Maintainance_Rovers->isEmpty())
	{
		Rover* Temp;
		Maintainance_Rovers->Peek(Temp);
		while (Temp->GetMaintCompletionDay() == CurrentDay)
		{
			Maintainance_Rovers->Dequeue(Temp);
			MaintRovers--;
			if (Temp->GetType() == 'P')
			{
				Available_P_Rovers->Enqueue((P_Rover*)Temp, Temp->GetSpeed());
				AvRoversP++;
			}
			else if (Temp->GetType() == 'E')
			{
				Available_E_Rovers->Enqueue((E_Rover*)Temp, Temp->GetSpeed());
				AvRoversE++;
			}
			if (Maintainance_Rovers->isEmpty())
			{
				return;
			}
			Maintainance_Rovers->Peek(Temp);
		}
	}
}

void MasterStation::CheckupComplete()
{
	Rover* R;
	while (Checkup_Rovers->Peek(R))
	{
		if (R->GetCheckUpCompletionDay() == CurrentDay)
		{
			Checkup_Rovers->Dequeue(R);
			MoveFromCheckupToAvailable(R);
			
		}
		else
		{
			return;
		}

	}
}

void MasterStation::DoesItNeedCheckUp(Rover* R)
{
	if (R->GetMissionsDone() == R->GetMissionsB4Checkup())
	{
		R->SetNeedCheck(1);
	}
}


//Movers

//Moves both a mission and rover to InExecution
void MasterStation::MoveFromWaitingToInExecution(Mission* mission, Rover* rover)
{
	rover->SetAssignedMission(mission);
	CalculateArrive2Target(rover, mission->GetTLOC());
	//CD=ED+WD+FD
	int CD = (2 * rover->GetArrive2Target() + mission->GetDuration()) + CurrentDay ;
	
	//if the mission finishes the sameDay
	if (CD == CurrentDay) {

			//calculate important sums for the stats
			Total_Wait = Total_Wait + mission->GetWaitingDays();

			//Write Mission Done info in the output file
			IO_Interface->WriteEachDay(CurrentDay, mission->GetID(), mission->GetFormulationDay(), CurrentDay-mission->GetFormulationDay(), 0, Output);

			//getting id and type to be used to print on console 
			DailyCompMissionsIDs[DailyCompletedCount] = mission->GetID();
			DailyCompMissionsType[DailyCompletedCount] = mission->GetTYP();

			//incrementing and decrementing suitable counters
			DailyCompletedCount++;
			TotalCompleted++;
			if (mission->GetTYP() == 'E') {
				DailyCompletedCountE++;
				WaitingMissionsE--;
			}
			else {
				WaitingMissionsP--;

			}
			return;
	}
	
	mission->SetCompletionDay(CD);

	rover->SetCompletionDay(CD);

	mission->SetExecutionDays(2 * rover->GetArrive2Target() + mission->GetDuration());

	mission->SetWaitingDays(CurrentDay - mission->GetFormulationDay());

	N_Execution_Missions->Enqueue(mission, -1 * CD);
	N_Execution_Rovers->Enqueue(rover, -1 * CD);
	NExecRovs++; NExecMiss++;
	
	if (mission->GetTYP() == 'E')
	{
		NExecMissE++;
		WaitingMissionsE--;
	}
	else {
		WaitingMissionsP--;

	}
}

//Moves rover to av
void MasterStation::MoveFromCheckupToAvailable(Rover* R)
{
	//Resets the bool value
	R->SetNeedCheck(false);

	if (R->GetType() == 'P')
	{
		P_Rover* PRover = dynamic_cast<P_Rover*>(R);
		PRover->SetMissionsDone(0);
		Available_P_Rovers->Enqueue(PRover, PRover->GetSpeed());
		AvRoversP++;
	}
	else
	{
		E_Rover* ERover = dynamic_cast<E_Rover*>(R);
		ERover->SetMissionsDone(0);
		Available_E_Rovers->Enqueue(ERover, ERover->GetSpeed());
		AvRoversE++;
		CheckupRoversE--;
	}
	CheckupRovers--;
}

//Moves rover to chck
void MasterStation::MoveFromInExecutionToCheckup(Rover* TempRover)
{
	if (TempRover->GetType() == 'P')
	{
		P_Rover* PRover = dynamic_cast<P_Rover*>(TempRover); //downcast it in order to access checkup duration, and then move it to the checkup list
		PRover->SetCheckUpCompletionDay(CurrentDay + PRover->GetCheckupD());
		Checkup_Rovers->Enqueue(PRover, -(PRover->GetCheckUpCompletionDay()));
	}
	else
	{
		E_Rover* ERover = dynamic_cast<E_Rover*>(TempRover); //downcast it in order to access checkup duration, and then move it to the checkup list
		ERover->SetCheckUpCompletionDay(CurrentDay + ERover->GetCheckupD());
		Checkup_Rovers->Enqueue(ERover, -(ERover->GetCheckUpCompletionDay()));
		CheckupRoversE++;
	}
	NExecRovs--;
	CheckupRovers++;
}

//Moves rover to av
void MasterStation::MoveFromInExecutionToAvailable(Rover* TempRover)
{
	if (TempRover->GetType() == 'P')
	{
		P_Rover* PRover = dynamic_cast<P_Rover*>(TempRover);

		Available_P_Rovers->Enqueue(PRover, PRover->GetSpeed());
		AvRoversP++;
	}
	else // if it's not polar, then it's definitely emergency
	{
		E_Rover* ERover = dynamic_cast<E_Rover*>(TempRover);

		Available_E_Rovers->Enqueue(ERover, ERover->GetSpeed());
		AvRoversE++;
	}
	NExecRovs--;
}

//Moves rover to maint
void MasterStation::MoveFromInExecutionToMaintenance(Rover* R)
{
	R->SetMaintCompletionDay(CurrentDay + (R->GetMaintDur()));
	Maintainance_Rovers->Enqueue(R, -(R->GetMaintCompletionDay()));
	NExecRovs--;
	MaintRovers++;
	if (R->GetType() == 'E')
	{
		MaintRoversE++;
	}
}



//Getters

//Failure
void MasterStation::Checkfailed() {

	if (N_Execution_Missions->isEmpty()) { return; }
	if (ProbabilityOFfailure == 0) {
		return; //no need to calculate or check
	}

	PriorityQueue<Mission*>tempM; //should be sorted according to negative of completion day


	Mission* M;
	Rover* R;

	while (N_Execution_Missions->Dequeue(M)) {

		//puts in a random seed dependent on number of missions and runtime values
		
		int MissionSpan =M->GetExecutionDays(); //execution days represent the span to which 
		//we need to scale to  
		
		double MissionFailP = CalculateProbability(ProbabilityOFfailure, MissionSpan);
		


		double randomF = ((rand() % (10000)) / 100.0); //generates a random float from 0 to 100 With Percision up to TWO decimal places
		
		//So we need to add extra 5 decimal places for accuracy 
		int a = 0;
		for (int i = 0; i < 5; i++) {
			a = rand() % 10;
			randomF = randomF + a * pow(0.1, 3 + i);
		}


		//if it failed 
		if (randomF<MissionFailP) {
			
			int SD_o = M->GetStartingDay();
			int MD_o = M->GetDuration();
			//restart FormulationDay
			M->SetFormulationDay(CurrentDay);
			//restart Waiting Days 
			M->SetWaitingDays(0);

			

			//classify
			P_Mission* P = dynamic_cast<P_Mission*>(M);
			
			E_Mission* E = dynamic_cast<E_Mission*>(M);
			
			NExecMiss--;
			if(P){
				Waiting_P_Missions->Enqueue(P);
				WaitingMissionsP++;
			}
			else{
				//reset the priority
				E->SetPriority();
				Waiting_E_Missions->Enqueue(E,E->GetPriority());
				WaitingMissionsE++;
				NExecMissE--;
			}
			
			//don't forget to change the arrival time of rover if it should be changed 
			
			//first get the corresponding rover 
			R = M->GetAssignedRover();
			
			if (R->GetType() == 'E') {
				Failed_E_Rover++;
			}
			else {
				Failed_P_Rover++;
			}

			R->SetAssignedMission(nullptr);
			R->SetNeedCheck(true); // it will need checkup upon arrival
			
			int NewCompletionlDay = R->GetCompletionDay(); //assume it's unchanged

			int Arrive2Target= R->GetArrive2Target();
			//1-if it was still going to the location
			if (CurrentDay < SD_o + Arrive2Target) {

				//CompDay= CurrentDay + (CurrentDay-StartingDay)

				NewCompletionlDay = 2*CurrentDay - SD_o ;

				R->SetCompletionDay(NewCompletionlDay);
			} 
			//2-if it was on the location
			else if (CurrentDay > SD_o + Arrive2Target && CurrentDay < SD_o + Arrive2Target + MD_o) {

				//Compday= Current+ Arrive2Target

				NewCompletionlDay = CurrentDay +  Arrive2Target;

				R->SetCompletionDay(NewCompletionlDay);

			}
			
			//3-if it was coming back from the location: then leave the CD as is
			
			//until now, we haven't applied the changes to the PQ of rover, this will be done in a separate while loop

		}
		//DID NOT FAIL
		else {

			tempM.Enqueue(M, 1); //dummy priority

			//no need to change the state of the rover, the mission didn't fail
		}

	}

	while (tempM.Dequeue(M)) {
		N_Execution_Missions->Enqueue(M, -(M->GetCompletionDay()));  //negative of completion day
	}
	// we still need a loop to update all the Rovers' priority
	PriorityQueue<Rover*> tempR;
	while (N_Execution_Rovers->Dequeue(R)) {
		tempR.Enqueue(R, 1);  //dummy priority
	}

	while (tempR.Dequeue(R)) {
		N_Execution_Rovers->Enqueue(R, -R->GetCompletionDay());
	}

}



//Statistics
//should be called at final day.
bool MasterStation::CalculateStats(float& AvgWait, float& AvgExec) 
{

	if (N_Missions == 0)
	{
		return false;
	}
	AvgWait = Total_Wait / N_Missions;
	AvgExec = Total_InExecution / N_Missions;
	return true;

}


///////////////Auxillary Utilities////////////////

/// 5% = x^duration   (duration is in days)
/// This function calculates the x
double MasterStation:: CalculateProbability(int DesiredFailure, int Duration) {

	int DesiredSuccess = 100 - DesiredFailure;

	double DailySuccess = DesiredSuccess / 100.0;  //you need to divide in order for the exp to work properly

	DailySuccess = log(DailySuccess) / Duration;

	DailySuccess = exp(DailySuccess) * 100;  // in percent

	double DailyFailure = 100 - DailySuccess;  //in percent

	return DailyFailure;
}
