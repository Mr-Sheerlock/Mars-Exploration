//Abstract Class


class Event {

	int ID;   //ID of Mission
	int EventDay;  //the day the event will get executed

public:

	//should we make the return type of execute Mission pointer  ? 
	virtual void Execute() = 0;

};