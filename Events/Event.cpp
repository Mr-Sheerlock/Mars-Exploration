#include "Event.h"


Event::Event(int ed, int id) {
	EventDay = ed; //event day
	ID = id;
}

int Event:: getEventDay() { return EventDay; }


int Event::getID() { return ID; }