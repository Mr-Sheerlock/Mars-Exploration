#pragma once


class Rover
{

	int ID;
	int Speed; // in km/hr
	char Type; // M, P, or E
	
	int static N; //number of missions before checkup

	int Health; //used for maintainance


	/////// Health Eqn

	////////

public:
	Rover(int, int);
	~Rover();
	virtual void SetSpeed(int s);
	virtual int GetSpeed();
	virtual int GetID();
	void static setN(int n);

	//virtual void setType()=0;       //for later 

};
