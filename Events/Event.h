#pragma once
//Abstract Class

#include "../MasterStation.h"


class MasterStation;

class Event {

	int ID;   //ID of Mission
	int EventDay;  //the day the event will get executed

public:

	Event(int ed, int id) {
		EventDay= ed; //event day
		ID = id;
	}

	virtual bool Execute(MasterStation* MS) = 0;
	

	int getEventDay() { return EventDay; }
	int getID() { return ID; }

};