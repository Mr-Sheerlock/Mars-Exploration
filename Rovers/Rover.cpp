#include "Rover.h"



Rover::Rover(int id, int s)
{
	ID = id;
	Speed = s;
	Health = 100;      // ie .100%

	MissionID = -1;

	NeedsCheckup = 0;
}

int Rover::MissionsB4Checkup=10; //set an initial value
int Rover::MaintDur=10;

Rover::~Rover()
{
}

void Rover::SetSpeed(int s)
{
	if (s <= 0) { Speed = 1; return; }
	Speed = s;
}

int Rover::GetSpeed()
{
	return Speed;
}


int Rover::GetID()
{
	return ID;
}

void Rover::setN(int n)
{
	if (n < 0) { MissionsB4Checkup = 10; return; }
	MissionsB4Checkup = n;
}

void Rover::setMaintDur(int MDur)
{
	//if (MDur < 0) { MainDur = 10; return; }
	MaintDur = MDur;
}

int Rover::GetMaintDur()
{
	return MaintDur;
}


void Rover::SetMissionID(int ID) {

	MissionID = ID;
}

int Rover::GetMissionID() {

	return MissionID;
}

void Rover::SetCompletionDay(int ArrivalD) {

	CompletionDay = ArrivalD;
}

int Rover::GetCompletionlDay() {
	return CompletionDay;
}

void Rover::SetType(char x) {
	Type = x;
}


void Rover::SetArrive2Target(int x) {

	Arrive2Target = x;
}

int Rover::GetArrive2Target() {

	return Arrive2Target;
}

