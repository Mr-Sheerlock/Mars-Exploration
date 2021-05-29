#pragma once


class Rover
{

	int ID;
	int MissionID;
	char Type; // M, P, or E

	

	int Speed; // in km/hr
	int CompletionDay; 

	int Arrive2Target; //Days taken to arrive to the target

	bool NeedsCheckup;

	
	int static MissionsB4Checkup; //number of missions before checkup
	
	int static MaintDur; // Maintainance
	
	int Health; //used for maintainance

	//How we did health


public:
	Rover(int, int);
	~Rover();
	virtual void SetSpeed(int s);
	virtual int GetSpeed();
	virtual int GetID();
	void static SetMissionsB4Checkup(int n);
	void static SetMaintDur(int MDur);

	int static GetMaintDur();

	void SetMissionID(int ID);
	int  GetMissionID();

	int GetCompletionlDay();
	void SetCompletionDay(int x);

	void SetType(char x);       
	virtual char GetType()=0;

	void SetArrive2Target(int x);

	int GetArrive2Target();



};
