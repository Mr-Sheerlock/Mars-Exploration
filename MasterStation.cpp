#include "MasterStation.h"

MasterStation::MasterStation()
{
	IO_Interface = new UI();
	N_Missions = 0;

	Output.open("Output.txt");

	//Lists

	EventList = new Queue<Event*>();

	Av_P_Rovers = new PriorityQueue<P_Rover*>(); //According to speed
	Av_E_Rovers = new PriorityQueue<E_Rover*>();


}


/////////////////////////////////////////INPUT////////////////////////////////////////////////

void MasterStation::ReadInputFile()
{
	Input.open("input.txt");

	if (Input.is_open())
	{
		ReadRovers();
		ReadCheckupInfo();
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

	IO_Interface->ReadRoversNumbers(Input, N_Mount, N_Polar, N_Emerg); // waiting rovers of each type

	N_Rovers = N_Mount + N_Polar + N_Emerg; //Total number of rovers
	int speed;
	
	for (int i = 0; i < N_Polar; i++)
	{
		//create new Mountainous rover and set its ID
		//add it in the queue
		speed = IO_Interface->ReadRoverSpeeds(Input);
		P_Rover* New_P_Rover = new P_Rover(UniqueID, speed);
		Av_P_Rovers->Enqueue(New_P_Rover, speed);
		UniqueID++;
	}
	for (int i = 0; i < N_Emerg; i++)
	{
		//create new Mountainous rover and set its ID
		//add it in the queue
		speed = IO_Interface->ReadRoverSpeeds(Input);
		E_Rover* New_E_Rover = new E_Rover(UniqueID, speed);
		Av_E_Rovers->Enqueue(New_E_Rover, speed);
		UniqueID++;
	}
}

void MasterStation::ReadCheckupInfo()
{
	int N, CM, CP, CE;
	IO_Interface->Read_N_CheckupDur(Input, N, CM, CP, CE);
	Rover::setN(N);
	P_Rover::Set_CheckupD(CP);
	E_Rover::Set_CheckupD(CE);
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

void MasterStation::ExecuteDay() {
	
		//Notice that here, we need to scheme the order of operation so as to not miss anything

		//Proposed Order: Events->Rovers Arrival->Rovers Checkup-> Rovers Maintainance->
		//->Missions Assignment
	
	//1-Check if there are Any Events O(1)

	
}



////
//void MasterStation::Exe




MasterStation::~MasterStation()
{
}