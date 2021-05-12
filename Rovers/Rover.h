#pragma once


class Rover
{
public:
	Rover(int, int);
	~Rover();
	void SetSpeed(int s);
	int GetSpeed();
	virtual int GetCheckupD() = 0;
	int GetID();
	void static setN(int n);

private:
	int ID;
	int Speed; // in km/hr
	char Type; // M, P, or E
	int static N; //number of missions before checkup
};
