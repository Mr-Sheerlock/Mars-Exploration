#include "Rover.h"



Rover::Rover(int id, int s)
{
	ID = id;
	Speed = s;

	//Initialize data members:
	TotalDist = 0;
	TotalMDur = 0;
	HighSigMissNum = 0;
	Health = 90;

	CheckUpCompletionDay = -50;
	NeedsCheckup = 0;
	//AssignedMission = nullptr;
}


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

void Rover::SetMissionsB4Checkup(int n)
{
	if (n < 0) { MissionsB4Checkup = 10; return; }
	MissionsB4Checkup = n;
}

int Rover::GetMissionsB4Checkup()
{
	return MissionsB4Checkup;
}

void  Rover::setMaintDur(int MDur)
{
	
	MaintDur = MDur;
}

int Rover::GetMaintDur()
{
	return MaintDur;
}

int Rover::MissionsB4Checkup=10; //set an initial value
int Rover::MaintDur=10;

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

void Rover::SetAssignedMission(Mission* mission)
{
	AssignedMission = mission;
}

Mission* Rover::GetAssignedMission()
{
	return AssignedMission;
}

//Checkup

void Rover::SetNeedCheck(bool x) {

	NeedsCheckup = x;
}

bool Rover::GetNeedCheck() {

	return NeedsCheckup;
}

void Rover::SetCheckUpCompletionDay(int x)
{
	CheckUpCompletionDay = x;
}

int Rover::GetCheckUpCompletionDay()
{
	return CheckUpCompletionDay;
}

void Rover::SetMissionsDone(int x)
{
	MissionsDone = x;
}

int Rover::GetMissionsDone()
{
	return MissionsDone;
}

//Maintainance:

void Rover::UpdateTotalDist(int D)
{
	TotalDist += D;
}

void Rover::UpdateTotalMDur(int MDur)
{
	TotalMDur += MDur;
}

void Rover::IncrementHighSigMissNum()
{
	HighSigMissNum++;
}

void Rover::SetMaintCompletionDay(int MCD)
{
	MaintCompletionDay = MCD;
}

void Rover::UpdateHealth()
{
	if (TotalMDur > 20)
	{
		Health -= 30;
		TotalMDur = 0;
	}
	if (TotalDist > 1500)
	{
		Health -= 30;
		TotalDist = 0;
	}
	if (HighSigMissNum > 4)
	{
		Health -= 30;
		HighSigMissNum = 0;
	}
}

int Rover::GetTotalDist()
{
	return TotalDist;
}

int Rover::GetTotalMDur()
{
	return TotalMDur;
}

int Rover::GetHighSigMissNum()
{
	return HighSigMissNum;
}

int Rover::GetMaintCompletionDay()
{
	return MaintCompletionDay;
}

int Rover::GetHealth()
{
	return Health;
}

void Rover::ResetMaintValues()
{
	TotalDist = 0;
	TotalMDur = 0;
	HighSigMissNum = 0;
	MaintCompletionDay = 0;
	Health = 100;
}