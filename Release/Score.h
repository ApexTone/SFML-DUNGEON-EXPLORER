#pragma once
#include<SFML/Graphics.hpp>
#include<iostream>
#include<sstream>
using namespace std;
using namespace sf;
class Score
{
public:
	Score(float xPos,float yPos);
	~Score();

	void Draw(RenderWindow& rw);
	void updateScore(unsigned int num);
	void resetStat();
	int GetScore() { return nowScore; }
private:
	Font font;
	Text text;
	unsigned int nowScore = 0;
	ostringstream scoreFile;

};
