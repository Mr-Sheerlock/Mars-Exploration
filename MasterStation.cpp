#include "MasterStation.h"
#include "Events/Event Headers.h"

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
	Rover::setMissionsB4Checkup(N);
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


/////////////////////////////////////////OUTPUT////////////////////////////////////////////////






////////////////////////////////////////////Operation//////////////////////////////////////////



void MasterStation::ExecuteEvent(Queue<Event*>* EventList)
{
	Event* Executed_Event;
	if (EventList->IsEmpty())
	{
		return;
	}
	while (EventList->Peek()->getEventDay() == CurrentDay)
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

	
}

//temporary code for assigning the mission, doesn't contain any checks, just assigns them
void MasterStation::AssignMission(Mission* M) {
	N_Execution_Missions.Enqueue(M,1);



}


//incomplete in this version
void MasterStation::Checkfailed() {

	PriorityQueue<Mission*>tempM; //should be sorted according to negative of completion day

	PriorityQueue<Rover*> tempR;

	Mission* M;
	Rover* R;

	srand(time(NULL) + N_Missions); 
	while (N_Execution_Missions.Dequeue(M)) {

		//puts in a random seed dependent on number of missions and runtime values
		
		int MissionSpan =M->GetExecutionDays(); //execution days represent the span to which 
		//we need to scale  
		
		double MissionFailP = CalculateProbability(ProbabilityOFfailure, MissionSpan);
		
		//Try to adjust accuracy 

		double randomF = (rand() % (1000000)) / 1000.0; //generates a random float from 0 to 100
		
		//if it failed 
		if (randomF<MissionFailP) {
			
			int FD_o = M->GetFormulationDay();
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
				Waiting_E_Missions->Enqueue(E,E->GetPriority());
			}
			
			//don't forget to change the arrival time of rover if it should be changed 
			
			//first get the corresponding rover 
			R = M->GetAssignedRover();
			
			int NewCompletionlDay = R->GetCompletionlDay(); //assume it's unchanged

			int Arrive2Target= R->GetArrive2Target();
			//1-if it was still going to the location
			if (CurrentDay < FD_o + Arrive2Target) {

				//CompDay= CurrentDay + (CurrentDay-FD)

				NewCompletionlDay = 2*CurrentDay - FD_o ;

				R->SetCompletionDay(NewCompletionlDay);
			} 
			//2-if it was on the location
			else if (CurrentDay > FD_o + Arrive2Target && CurrentDay < FD_o + Arrive2Target + MD_o) {

				//Compday= Current+ Arrive2Target

				NewCompletionlDay = CurrentDay +  Arrive2Target;

				R->SetCompletionDay(NewCompletionlDay);

			}
			
			//3-if it was coming back from the location: then leave the CD as is
			
			//until now, we haven't applied the changes to the PQ of rover, this will be done as an extra check in MissionArrive function

		}
		else {

			tempM.Enqueue(M, 1); //dummy priority

			//no need to change the state of the rover, the mission didn't fail
		}



		while (tempM.Dequeue(M)) {
			N_Execution_Missions.Enqueue(M, -(M->GetCompletionDay()));  //negative of completing day

		}

		// we still need a loop to update all the missions' priority

	}

}


///////////////Auxillary Utilities////////////////

/// 5% = x^duration   (duration is in days)
/// This function calculates the x
double MasterStation:: CalculateProbability(int DesiredFailure, int Duration) {

	int DesiredSuccess = 100 - DesiredFailure;

	DesiredSuccess= DesiredSuccess / 100.0;  //you need to divide in order for the exp to work properly

	double DailySuccess = log(DesiredSuccess) / Duration;

	DailySuccess = exp(DailySuccess) * 100;  // in percent

	double DailyFailure = 100 - DailySuccess;  //in percent

	return DailyFailure;
}

MasterStation::~MasterStation()
{
}