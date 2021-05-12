//Abstract Class


class Event {

	int ID;   //ID of Mission
	int EventDay;  //the day the event will get executed

public:

	
	virtual void Execute() = 0;

};