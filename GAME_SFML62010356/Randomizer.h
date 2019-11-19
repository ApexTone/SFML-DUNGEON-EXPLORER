#pragma once
#include<iostream>
#include<time.h>
class Randomizer//Randomizer don't work!
{
public:
	Randomizer();
	~Randomizer();
	int generateRandom(int min=0,int max=100);
private:
	int randomInteger;
};

