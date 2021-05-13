#include "Rover.h"

Rover::Rover(int id, int s)
{
	ID = id;
	Speed = s;
	Health = 100;      // ie .100%
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

void Rover::setN(int n)
{
	if (n < 0) { N = 10; return; }
	N = n;
}

int Rover::N=10; //set an initial value