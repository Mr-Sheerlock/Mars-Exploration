#pragma once

class Mission;


class Rover
{

	int ID;
	int Speed; // in km/hr
	char Type; // M, P, or E

	

	int CompletionDay; 
	int Arrive2Target; //Days taken to arrive to the target

	//For Checkup
	bool NeedsCheckup;
	int CheckUpCompletionDay;
	int MissionsDone;
	int static MissionsB4Checkup; //number of missions before checkup
	

	//for Maintainance
	int static MaintDur; // Maintainance
	int TotalDist; //Total distance covered by the rover
	int TotalMDur; //Total Mission duration
	int HighSigMissNum; //Total number of missions executed thay have high significance 
	int MaintCompletionDay;
	
	int Health; 

	//How we did health


	Mission* AssignedMission;

public:
	Rover(int, int);
	~Rover();

	virtual void SetSpeed(int s);
	virtual int GetSpeed();

	virtual int GetID();

	void static SetMissionsB4Checkup(int n);
	int static GetMissionsB4Checkup();
	
	void static setMaintDur(int MDur);

	int static GetMaintDur();


	int GetCompletionlDay();
	void SetCompletionDay(int x);

	void SetArrive2Target(int x);
	int GetArrive2Target();


	void SetType(char x);       
	virtual char GetType()=0;

	////////////Checkup

	void SetMissionsDone(int x);
	int GetMissionsDone();

	void SetCheckUpCompletionDay(int x);
	int GetCheckUpCompletionDay();

	void SetNeedCheck(bool x);
	bool GetNeedCheck();

	///////////Maintainance
	void UpdateTotalDist(int D);
	void UpdateTotalMDur(int MDur);
	void IncrementHighSigMissNum();
	void SetMaintCompletionDay(int MCD);
	void UpdateHealth();

	int GetTotalDist();
	int GetTotalMDur();
	int GetHighSigMissNum();
	int GetMaintCompletionDay();
	int GetHealth();

	void ResetMaintValues(); //Reset the values of TotalDist, TotalMDur, HighSigMissNum


	void SetAssignedMission(Mission* mission);
	Mission* GetAssignedMission();
};
