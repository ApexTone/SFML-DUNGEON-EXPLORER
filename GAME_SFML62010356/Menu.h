#pragma once
#define MAX_ITEMS 4
#define RWWIDTH 1920
#define RWHEIGHT 1080
#include<SFML/Graphics.hpp>
#include<iostream>
using namespace std;
using namespace sf;
class Menu
{
public:
	Menu(float width, float height);
	~Menu();

	void Draw(RenderWindow &rw);
	void MoveUp();
	void MoveDown();
	int getPressedItem() { return selectedItem; }
private:
	int selectedItem;
	Font font;
	Text menu[MAX_ITEMS];
	Text Start, howtoplay, Exit,Highscore,Title;
	Sprite bg;

	Text devName,KMITL;
};

class HowToPlay
{
public:
	HowToPlay(float width,float height);
	~HowToPlay();

	void Draw(RenderWindow &rw);
private:
	//Sprite bg;
	Font font;
	Text key[7];
};

class Pause
{
public:
	Pause(RenderWindow& rw);
	~Pause();
	void Draw(RenderWindow& rw);
private:
	Font font;
	RectangleShape frame;
	Text asking;
};


