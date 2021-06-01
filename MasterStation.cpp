#include "MasterStation.h"
#include "Events/Event Headers.h"

#include <stdlib.h>  //for rand
#include <time.h>    // for time(0)
#include <cmath>	 // for exp and log 

MasterStation::MasterStation()
{

	
	IO_Interface = new UI();
	
	ProbabilityOFfailure = 5; // in percent
	N_Missions = 0;

	Output.open("Output.txt");
	CurrentDay = 1;

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



////////Data member Getters

PriorityQueue<E_Mission*>* MasterStation::ReturnWaitingEmerg()
{
	return Waiting_E_Missions;
}

Queue<P_Mission*>* MasterStation::ReturnWaitingPolar()
{
	return Waiting_P_Missions;
}



/////////////////////////////////////////INPUT////////////////////////////////////////////////

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

	IO_Interface->ReadRoversNumbers(Input, N_Polar, N_Emerg); // waiting rovers of each type

	N_Rovers = N_Polar + N_Emerg; //Total number of rovers
	int speed;
	
	for (int i = 0; i < N_Polar; i++)
	{
		//create new Mountainous rover and set its ID
		//add it in the queue
		speed = IO_Interface->ReadRoverSpeeds(Input);
		P_Rover* New_P_Rover = new P_Rover(UniqueID, speed);
		Available_P_Rovers->Enqueue(New_P_Rover, speed);
		UniqueID++;
	}
	for (int i = 0; i < N_Emerg; i++)
	{
		//create new Mountainous rover and set its ID
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


//////////////////////////////////////OUTPUT////////////////////////////////////////////////
void MasterStation::PrintEachDay()
{
	Queue<int> id;

	int WaitingEmergency = 0; int* WaitingEIds = NULL;
	if (!(Waiting_E_Missions->isEmpty()))//in case of empty list(no waiting emergency missions)
	{
		WaitingEmergency = TakeIdsFromWaitingE(id);
		WaitingEIds = new int[WaitingEmergency];
		PutIdsInArray(id, WaitingEIds, WaitingEmergency);
	}
	int WaitingPolar = 0; int* WaitingPIds = NULL;
	if (!(Waiting_P_Missions->IsEmpty()))
	{
		WaitingPolar = TakeIdsFromWaitingP(id);
		WaitingPIds = new int[WaitingPolar];
		PutIdsInArray(id, WaitingPIds, WaitingPolar);
	}
	int Ecount = 0, Compcount = 0, ECompcount = 0, InExecution = 0;
	int* InExecution_M_Ids = NULL, * InExecution_R_Ids = NULL, * Completed_Ids = NULL;
	char* type = NULL, * CompType = NULL;
	if (!(N_Execution_Missions->isEmpty()))
	{
		Queue<int> MIds, RIds, CompIds;
		Queue<char> M_Rtype, comptype;
		InExecution = TakeInfoFromInExecution(CompIds, MIds, RIds, M_Rtype, comptype, Compcount, ECompcount, Ecount);
		InExecution_M_Ids = new int[InExecution];
		InExecution_R_Ids = new int[InExecution];
		type = new char[InExecution];
		Completed_Ids = new int[Compcount];
		CompType = new char[Compcount];
		PutIdsInArray(MIds, InExecution_M_Ids, InExecution);
		PutIdsInArray(RIds, InExecution_R_Ids, InExecution);
		PutIdsInArray(M_Rtype, type, InExecution);
		PutIdsInArray(CompIds, Completed_Ids, Compcount);
		PutIdsInArray(comptype, CompType, Compcount);
	}
	int AvEmergency = 0; int* AvEmergency_Ids = NULL;
	if (Available_E_Rovers->isEmpty())
	{
		AvEmergency = TakeIdsFromAvailableE(id);
		AvEmergency_Ids = new int[AvEmergency];
		PutIdsInArray(id, AvEmergency_Ids, AvEmergency);
	}
	int AvPolar = 0; int* AvPolar_Ids = NULL;
	if (!(Available_P_Rovers->isEmpty()))
	{
		AvPolar = TakeIdsFromAvailableP(id);
		AvPolar_Ids = new int[AvPolar];
		PutIdsInArray(id, AvPolar_Ids, AvPolar);
	}
	int InCheckup = 0, EInCheckup = 0; int* InCheckupIds = NULL; char* InCheckuptype = NULL;
	if (!(Checkup_Rovers->isEmpty()))
	{
		Queue<char>type;
		InCheckup = TakeInfoFromInCheckup(id, type, EInCheckup);
		InCheckupIds = new int[InCheckup];
		InCheckuptype = new char[InCheckup];
		PutIdsInArray(id, InCheckupIds, InCheckup);
		PutIdsInArray(type, InCheckuptype, InCheckup);
	}
	IO_Interface->PrintStatements(1, CurrentDay);
	IO_Interface->PrintStatements(2, WaitingEmergency + WaitingPolar);
	IO_Interface->PrintEmergency(WaitingEIds, WaitingEmergency);
	IO_Interface->PrintPolar(WaitingPIds, WaitingPolar);
	IO_Interface->PrintStatements(3, InExecution);
	IO_Interface->PrintInExecution(InExecution_M_Ids, InExecution_R_Ids, InExecution, Ecount, InExecution - Ecount, type);
	IO_Interface->PrintStatements(4, AvEmergency + AvPolar);
	IO_Interface->PrintEmergency(AvEmergency_Ids, AvEmergency);
	IO_Interface->PrintPolar(AvPolar_Ids, AvPolar);
	IO_Interface->PrintStatements(5, InCheckup);
	IO_Interface->PrintInCheckup(InCheckupIds, InCheckup, EInCheckup, InCheckup - EInCheckup, InCheckuptype);
	IO_Interface->PrintStatements(6, Compcount);
	IO_Interface->PrintInCheckup(Completed_Ids, Compcount, ECompcount, Compcount - ECompcount, CompType);//note to self ba3d masla7 el error a8iar esmha f el UI
}

int MasterStation::TakeIdsFromWaitingE(Queue<int>& Ids)
{
	Queue<E_Mission*>temp;//missions are stored temporarily in queue instead
	E_Mission* item;      //of priority queue to reduce complexity
	Queue<int> id;
	int size = 0;
	while (Waiting_E_Missions->Dequeue(item))
	{
		id.Enqueue(item->GetID());
		temp.Enqueue(item);
		size++;
	}
	while (temp.Dequeue(item))
	{
		Waiting_E_Missions->Enqueue(item, item->GetPriority());
	}
	return size;
}

int MasterStation::TakeIdsFromWaitingP(Queue<int>& ids)
{
	int size = 1;
	P_Mission* ptemp1, * ptemp2;
	Waiting_P_Missions->Dequeue(ptemp1);
	ids.Enqueue(ptemp1->GetID());
	Waiting_P_Missions->Peek(ptemp2);
	while (ptemp2 != ptemp1)
	{
		Waiting_P_Missions->Dequeue(ptemp2);
		ids.Enqueue(ptemp2->GetID());
		size++;
		Waiting_P_Missions->Enqueue(ptemp2);
		Waiting_P_Missions->Peek(ptemp2);
	}
	return size;
}

int MasterStation::TakeInfoFromInExecution(Queue<int>& C, Queue<int>& M, Queue<int>& R, Queue<char>& RM, Queue<char>& Ctype, int& comp, int& Ecomp, int& E)
{
	int InExecution = 0;
	Queue<Mission*> t;
	Mission* tempmission; Rover* temprover;
	while (N_Execution_Missions->Dequeue(tempmission))
	{
		if (tempmission->GetCompletionDay() == CurrentDay)
		{
			C.Enqueue(tempmission->GetID());
			Ctype.Enqueue(tempmission->GetTYP());
			comp++;
			if (tempmission->GetTYP() == 'E')
				Ecomp++;
		}
		else
		{
			M.Enqueue(tempmission->GetID());
			temprover = tempmission->GetAssignedRover();
			R.Enqueue(temprover->GetID());
			RM.Enqueue(tempmission->GetTYP());
			InExecution++;
			if (tempmission->GetTYP() == 'E')
				E++;
		}
		t.Enqueue(tempmission);
	}
	while (t.Dequeue(tempmission))
	{
		N_Execution_Missions->Enqueue(tempmission, (tempmission->GetCompletionDay() * -1));
	}
	return InExecution;
}

int MasterStation::TakeIdsFromAvailableE(Queue<int>& id)
{
	int size = 0;
	Queue<E_Rover*>E;
	E_Rover* etemp;
	while (Available_E_Rovers->Dequeue(etemp))
	{
		id.Enqueue(etemp->GetID());
		E.Enqueue(etemp);
		size++;
	}
	while (E.Dequeue(etemp))
	{
		Available_E_Rovers->Enqueue(etemp, etemp->GetSpeed());
	}
	return size;
}

int MasterStation::TakeIdsFromAvailableP(Queue<int>& id)
{
	int size = 0;
	Queue<P_Rover*>Q;
	P_Rover* ptemp;
	while (Available_P_Rovers->Dequeue(ptemp))
	{
		id.Enqueue(ptemp->GetID());
		Q.Enqueue(ptemp);
		size++;
	}
	while (Q.Dequeue(ptemp))
	{
		Available_P_Rovers->Enqueue(ptemp, ptemp->GetSpeed());
	}
	return size;
}

int MasterStation::TakeInfoFromInCheckup(Queue<int>& id, Queue<char>& type, int& Ecount)
{
	int size = 0;
	Queue<Rover*>temp;
	Rover* rtemp;
	while (Checkup_Rovers->Dequeue(rtemp))
	{
		id.Enqueue(rtemp->GetID());
		temp.Enqueue(rtemp);
		size++;
		char x = rtemp->GetType();
		type.Dequeue(x);
		if (x == 'E')
			Ecount++;
	}
	while (temp.Dequeue(rtemp))
	{
		Checkup_Rovers->Enqueue(rtemp, (rtemp->GetCheckUpCompletionDay() * -1));
	}
	return size;
}

template<typename T>
void MasterStation::PutIdsInArray(Queue<T>& tempId, T*& ids, int size)
{
	T temp;
	for (int i = 0; i < size; i++)
	{
		tempId.Dequeue(temp);
		ids[i] = temp;
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





//////////////////////////////////////Operation//////////////////////////////////////////



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
	
	//1-Check if there are Any Events O(1)
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


void MasterStation::CheckRoverArrived()
{
	Rover* TempRover;
	CheckupComplete();
	while (N_Execution_Rovers->Peek(TempRover))
	{
		if (TempRover->GetCompletionlDay() == CurrentDay)
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

void MasterStation::MoveFromCheckupToAvailable(Rover* R)
{
	if (R->GetType() == 'P')
	{
		P_Rover* PRover = dynamic_cast<P_Rover*>(R);
		PRover->SetMissionsDone(0);
		Available_P_Rovers->Enqueue(PRover, PRover->GetSpeed());
	}
	else
	{
		E_Rover* ERover = dynamic_cast<E_Rover*>(R);
		ERover->SetMissionsDone(0);
		Available_E_Rovers->Enqueue(ERover, ERover->GetSpeed());
	}
}

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
	}
}

void MasterStation::MoveFromInExecutionToAvailable(Rover* TempRover)
{
	if (TempRover->GetType() == 'P')
	{
		P_Rover* PRover = dynamic_cast<P_Rover*>(TempRover);

		Available_P_Rovers->Enqueue(PRover, PRover->GetSpeed());
	}
	else // if it's not polar, then it's definitely emergency
	{
		E_Rover* ERover = dynamic_cast<E_Rover*>(TempRover);

		Available_E_Rovers->Enqueue(ERover, ERover->GetSpeed());
	}
}

void MasterStation::MoveFromInExecutionToMaintenance(Rover* R)
{
	R->SetMaintCompletionDay(CurrentDay + (R->GetMaintDur()));
	Maintainance_Rovers->Enqueue(R, -(R->GetMaintCompletionDay()));
}




//incomplete in this version
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
			
			if(P){
				Waiting_P_Missions->Enqueue(P);
			}
			else{
				//reset the priority
				// Don't forget to ADD the actual Equation
				int Priority = 10;  
				E->SetPriority(Priority);
				Waiting_E_Missions->Enqueue(E,E->GetPriority());
			}
			
			//don't forget to change the arrival time of rover if it should be changed 
			
			//first get the corresponding rover 
			R = M->GetAssignedRover();
			
			R->SetNeedCheck(true); // it will need checkup upon arrival
			
			int NewCompletionlDay = R->GetCompletionlDay(); //assume it's unchanged

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
		N_Execution_Rovers->Enqueue(R, -R->GetCompletionlDay());
	}

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

MasterStation::~MasterStation()
{
}