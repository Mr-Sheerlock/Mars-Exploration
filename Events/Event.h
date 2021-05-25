#pragma once


//Abstract Class

#include "../MasterStation.h"


class MasterStation;

class Event {

	int ID;   //ID of Mission
	int EventDay;  //the day the event will get executed

public:

	Event(int ed, int id);

	virtual bool Execute(MasterStation* MS) = 0;
	

	int getEventDay();
	int getID();

};