#include "Score.h"

Score::Score(float xPos, float yPos)
{
	if (!font.loadFromFile("Serithai-Regular.ttf"))
	{
		//Error handling
	}
	text.setFont(font);
	text.setFillColor(Color::Green);
	text.setPosition(xPos,yPos);
	scoreFile << "Score: " << nowScore;
	text.setString(scoreFile.str());
}

Score::~Score()
{
}

void Score::Draw(RenderWindow& rw)
{
	rw.draw(text);
	//cout << nowScore<< endl;
}

void Score::updateScore(unsigned int num)
{
	nowScore += num;

	scoreFile.str("");
	scoreFile << "Score: " << nowScore;
	text.setString(scoreFile.str());
}

void Score::resetStat()
{
	nowScore = 0;
}