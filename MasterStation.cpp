#include "MasterStation.h"
#include "Events/Event Headers.h"

#include <stdlib.h>  //for rand
#include <time.h>    // for time(0)
#include <cmath>	 // for exp and log 

MasterStation::MasterStation()
{

	CurrentDay = 1;

	N_Missions = 0;
	
	N_Rovers = 0;
	NRoversP = 0; 
	NRoversE=0;
	NMissionsP = 0;
	NMissionsE=0;
	WaitingMissionsP = 0;
	WaitingMissionsE=0;
	AvRoversE = 0;
	AvRoversP=0;

	NExecMiss=0;
	NExecRovs=0;

	CheckupRovers = 0;
	CheckupRoversE=0;
	MaintRovers = 0;
	MaintRoversP=0;

	ProbabilityOFfailure = 5; // in percent

	DailyCompletedCount = 0;
	DailyCompletedCountE = 0;


	IO_Interface = new UI();
	Output.open("Output.txt");
	//Lists

	EventList = new Queue<Event*>();

	Available_P_Rovers = new PriorityQueue<P_Rover*>(); //According to speed
	Available_E_Rovers = new PriorityQueue<E_Rover*>();

	N_Execution_Rovers= new PriorityQueue<Rover*>;   

	N_Execution_Missions= new PriorityQueue<Mission*>;

	Waiting_P_Missions= new Queue<P_Mission*>;

	/*PriorityQueue<Rover*>*Checkup_Rovers; 

	PriorityQueue<Rover*>*Maintainance_Rovers; */

	Waiting_E_Missions = new PriorityQueue<E_Mission*>;

	srand(time(NULL) + N_Missions); // ensures that each run we get a new random output regarding the failure
	
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
		cout << "File not found" << endl;
	}
}

void MasterStation::ReadRovers()
{
	int UniqueID; //used to find a unique id for each rover
	UniqueID = 1;

	IO_Interface->ReadRoversNumbers(Input, NRoversP, NRoversE); // waiting rovers of each type

	N_Rovers = NRoversP+ NRoversE; //Total number of rovers
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
	int N, CM, CP, CE;
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
	char Rover_Typ; //Rover type: M, P, or E
	int ED, ID, TLOC, MDUR, SIG;
	for (int i = 0; i < EventsNum; i++)
	{
		TYP = IO_Interface->ReadEventsTyp(Input);
		if (TYP == 'F')
		{
			N_Missions++;
			IO_Interface->Read_Formulation(Input, Rover_Typ, ED, ID, TLOC, MDUR, SIG);
			//create a new formulatoin event and add it in the EventList
			Formulation_Event* newFormEv = new Formulation_Event(ED, ID, Rover_Typ,   TLOC, MDUR, SIG);
			EventList->Enqueue(newFormEv);
		}
		
	}

}


///////////////////////////OUTPUT//////////////////////////////////////
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

	int* InExecution_M_Ids = NULL, * InExecution_R_Ids = NULL,*F_InExecution_R_Ids=NULL;
	char* N_Exectype = NULL, *Ftype=NULL;
	if (!(N_Execution_Missions->isEmpty()))
	{
		InExecution_M_Ids = new int[NExecMiss];
		InExecution_R_Ids = new int[NExecMiss]; //we assume that they are the same size and handle the failed mission rovers in another array
		F_InExecution_R_Ids = new int[NExecRovs - NExecMiss]; //Rovers' numbers are always higher than missions
		
		N_Exectype = new char[NExecMiss];
		Ftype = new char[NExecRovs - NExecMiss]; //difference is the number of rovers who has failed missions

		TakeInfoFromInExecution(InExecution_M_Ids, InExecution_R_Ids, N_Exectype, F_InExecution_R_Ids, Ftype);
	}



	int* AvEmergency_Ids = NULL;
	if (Available_E_Rovers->isEmpty())
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
	if (!(Checkup_Rovers->isEmpty()))
	{
		InCheckupIds = new int[MaintRovers];
		InCheckuptype = new char[MaintRovers];
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

	//TODO: Modify the function to include failed Rovers in the output
	int FailedRovers = NExecRovs - NExecMiss;

	IO_Interface->PrintInExecution(InExecution_M_Ids, InExecution_R_Ids, NExecMiss, NExecMissE, NExecMissP, N_Exectype, F_InExecution_R_Ids, FailedRovers, FailedPRover, FailedERover, Ftype);

	//Available Rovers
	IO_Interface->PrintStatements(4, AvRoversE + AvRoversP);
	IO_Interface->PrintEmergency(AvEmergency_Ids, AvRoversE);
	IO_Interface->PrintPolar(AvPolar_Ids, AvRoversP);

	//InCheckup Rovers
	int CheckupRoversP = CheckupRovers - CheckupRoversE;

	IO_Interface->PrintStatements(5, CheckupRovers);
	IO_Interface->PrintInCheckup(InCheckupIds, CheckupRovers, CheckupRoversE, CheckupRoversP, InCheckuptype);

	//Maintainance Rovers
	int MaintRoversE = MaintRovers - MaintRoversP;
	IO_Interface->PrintStatements(7, MaintRovers);
	IO_Interface->PrintInMaint(InMaintIds, MaintRovers, MaintRoversE, MaintRoversP, InMaintType);

	//CompletedMissions
	int DailyCompletedCountP = DailyCompletedCount - DailyCompletedCountE;
	
	IO_Interface->PrintStatements(6, DailyCompletedCount);
	IO_Interface->PrintInCheckup(DailyCompMissionsIDs, DailyCompletedCount, DailyCompletedCountE, DailyCompletedCountP, DailyCompMissionsType);



	//TODO: DONT FORGET TO DEALLOCATE THE MEMORY 
	
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

	//DONT FORGET TO RESET THE DAILY COMP COUNT
	DailyCompletedCount = 0;
	DailyCompletedCountE = 0;

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
		
		temp.Enqueue(rtemp);
	}
	while (temp.Dequeue(rtemp))
	{
		Checkup_Rovers->Enqueue(rtemp, (rtemp->GetMaintCompletionDay()* -1));
	}
}



void MasterStation::FinalOutput()
{
	CurrentDay = 1;
	int userChoice;
	IO_Interface->ReadUserChoice(userChoice);
	if (userChoice == 1 || userChoice == 2)
	{
		while (!(EventList->IsEmpty()) && !(N_Execution_Missions->isEmpty()))
		{
			PrintEachDay();
			if (userChoice == 1)
				IO_Interface->InteractiveMode();
			else IO_Interface->StepByStepMode();
			CurrentDay++;
		}
	}
	else IO_Interface->SilentMode();
}





///////////////////////////Operation//////////////////////////////////////



void MasterStation::ExecuteEvent(Queue<Event*>* EventList)
{
	Event* Executed_Event;
	if (EventList->IsEmpty())
	{
		return;
	}
	Event* Ev;
	while (EventList->Peek(Ev)->getEventDay() == CurrentDay)
	{
		EventList->Dequeue(Executed_Event);
		Executed_Event->Execute(this);
	}
}


void MasterStation::ExecuteDay() {
	
		//Notice that here, we need to scheme the order of operation so as to not miss anything

		//Proposed Order: Events->Rovers Arrival->Rovers Checkup-> Rovers Maintainance->
		//->Missions Assignment
	
	//1-Check if there are Any Events O(1)if (EventList->IsEmpty() && Waiting_E_Missions->isEmpty() && Waiting_P_Missions->IsEmpty() && N_Execution_Missions->isEmpty())
	{
		// Last Day, its position in this function is subject to change.
		int AvgWait = 0;
		int AvgExec = 0;
		CalculateStats(AvgWait, AvgExec, CurrentDay);
		IO_Interface->WriteMissions(NMissionsE, NMissionsP, Output);
		IO_Interface->WriteRovers(NRoversE, NRoversP, Output);
		IO_Interface->WriteStats(AvgWait, AvgExec, Output);
	}
	AssignMission();
	Checkfailed();
	CurrentDay++;
}

//temporary code for assigning the mission, doesn't contain any checks, just assigns them
void MasterStation::AssignMission() {
	int i = CurrentDay;
	P_Mission* P = new P_Mission(i, i, i, i+2, i);

	P->SetExecutionDays(15);
	P_Rover* R = new P_Rover(i, 5);
	
	P->AssignRover(R);
	N_Execution_Missions->Enqueue(P,1);



}



//Checkup and Maintainance
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
					MaintRovers++;
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
					CheckupRovers++;
				}
				else if (Maintenance)
				{
					//Maintenance only
					MoveFromInExecutionToMaintenance(TempRover);
					MaintRovers++;
				}
			}
		}
		else
		{
			return;
		}
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
}

bool MasterStation::CheckMaint(Rover* rover)
{
	if (rover->GetHealth() <= 0)
	{
		return true;
		rover->ResetMaintValues();
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
			}
			else if (Temp->GetType() == 'E')
			{
				Available_E_Rovers->Enqueue((E_Rover*)Temp, Temp->GetSpeed());
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
void MasterStation::MoveFromCheckupToAvailable(Rover* R)
{
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
	}
	CheckupRovers--;
}

void MasterStation::MoveFromInExecutionToCheckup(Rover* TempRover)
{
	if (TempRover->GetType() == 'P')
	{
		P_Rover* PRover = dynamic_cast<P_Rover*>(TempRover); //downcast it in order to access checkup duration, and then move it to the checkup list
		PRover->SetCheckUpCompletionDay(CurrentDay + PRover->GetCheckupD());
		Checkup_Rovers->Enqueue(PRover, -(PRover->GetCheckUpCompletionDay()));
		AvRoversP++;
	}
	else
	{
		E_Rover* ERover = dynamic_cast<E_Rover*>(TempRover); //downcast it in order to access checkup duration, and then move it to the checkup list
		ERover->SetCheckUpCompletionDay(CurrentDay + ERover->GetCheckupD());
		Checkup_Rovers->Enqueue(ERover, -(ERover->GetCheckUpCompletionDay()));
		AvRoversE++;
	}
	NExecRovs--;
	CheckupRovers++;
}

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

void MasterStation::MoveFromInExecutionToMaintenance(Rover* R)
{
	R->SetMaintCompletionDay(CurrentDay + (R->GetMaintDur()));
	Maintainance_Rovers->Enqueue(R, -(R->GetMaintCompletionDay()));
	NExecRovs--;
	MaintRovers++;
}




//Failure
void MasterStation::Checkfailed() {

	if (N_Execution_Missions->isEmpty()) { return; }

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
				// Don't forget to ADD the actual Equation
				int Priority = 10;  
				E->SetPriority(Priority);
				Waiting_E_Missions->Enqueue(E,E->GetPriority());
				WaitingMissionsE--;
			}
			
			//don't forget to change the arrival time of rover if it should be changed 
			
			//first get the corresponding rover 
			R = M->GetAssignedRover();
			
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
void MasterStation::CalculateStats(int& AvgWait, int& AvgExec, int FinalDay) 
{

	AvgWait = Total_Wait / FinalDay;
	AvgExec = Total_InExecution / FinalDay;

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
