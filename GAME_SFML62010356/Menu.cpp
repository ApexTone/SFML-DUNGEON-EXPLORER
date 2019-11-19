#include "Menu.h"

Menu::Menu(float width, float height)
{
	selectedItem = 0;
	

	if (!font.loadFromFile("FiddlersCoveRegular-Mgge.otf"))
	{
		//Handle error
		cout << "Can't load FiddlersCoveRegular font" << endl;
	}
	Title.setFont(font);
	Title.setCharacterSize(100);
	Title.setFillColor(Color::Yellow);
	Title.setString("Dungeon Explorer!");
	Title.setPosition(Vector2f((width / 8), (height / 15)));

	const int xShift = 150;
	Start.setFont(font);
	Start.setCharacterSize(48);
	Start.setFillColor(Color::Red);
	Start.setString("Start");
	Start.setPosition(Vector2f(xShift+width / 2, (height / 5)));
	
	howtoplay.setFont(font);
	howtoplay.setFillColor(Color::White);
	howtoplay.setCharacterSize(48);
	howtoplay.setString("How To Play");
	howtoplay.setPosition(Vector2f(xShift + width / 2, (height / 5)*2));


	Highscore.setFont(font);
	Highscore.setFillColor(Color::White);
	Highscore.setCharacterSize(48);
	Highscore.setString("Highscore");
	Highscore.setPosition(Vector2f(xShift + width / 2, (height / 5) * 3));

	Exit.setFont(font);
	Exit.setFillColor(Color::White);
	Exit.setCharacterSize(48);
	Exit.setString("Exit");
	Exit.setPosition(Vector2f(xShift + width / 2, (height / 5)*4));

	devName.setFont(font);
	devName.setFillColor(Color::Yellow);
	devName.setCharacterSize(32);
	devName.setString("Tanapol Wong-asa 62010356");
	devName.setPosition(Vector2f(1550,950));
}

Menu::~Menu()
{
}

void Menu::Draw(RenderWindow& rw)
{
	Texture t;
	if (!t.loadFromFile("MainMenuBackground.png"))
	{
		cout << "Cant load MainMenuBackground" << endl;
	}
	bg.setTexture(t);
	bg.setScale(Vector2f(RWWIDTH / bg.getLocalBounds().width, (RWHEIGHT - 60) / bg.getLocalBounds().height));
	rw.draw(bg);
	rw.draw(Start);
	rw.draw(howtoplay);
	rw.draw(Highscore);
	rw.draw(Exit);
	rw.draw(Title);
	rw.draw(devName);
}

void Menu::MoveUp()
{
	/*if (selectedItem-1>=0)
	{
		menu[selectedItem].setFillColor(Color(255, 255, 255));
		selectedItem--;
		menu[selectedItem].setFillColor(Color::Red);
	}*/
	if (selectedItem - 1 >= 0)
	{
		selectedItem--;
		if (selectedItem==0)
		{
			Start.setFillColor(Color::Red);
			howtoplay.setFillColor(Color::White);
			Highscore.setFillColor(Color::White);
			Exit.setFillColor(Color::White);
		}
		else if (selectedItem==1)
		{
			Start.setFillColor(Color::White);
			howtoplay.setFillColor(Color::Red);
			Highscore.setFillColor(Color::White);
			Exit.setFillColor(Color::White);
		}
		else if (selectedItem==2)
		{
			Start.setFillColor(Color::White);
			howtoplay.setFillColor(Color::White);
			Highscore.setFillColor(Color::Red);
			Exit.setFillColor(Color::White);
		}
		else if (selectedItem == 3)
		{
			Start.setFillColor(Color::White);
			howtoplay.setFillColor(Color::White);
			Highscore.setFillColor(Color::White);
			Exit.setFillColor(Color::Red);
		}
	}
}

void Menu::MoveDown()
{
	/*if (selectedItem + 1 <= MAX_ITEMS)
	{
		menu[selectedItem].setFillColor(Color(255,255,255));
		selectedItem++;
		menu[selectedItem].setFillColor(Color::Red);
	}*/
	if (selectedItem + 1 < MAX_ITEMS)
	{
		selectedItem++;
		if (selectedItem == 0)
		{
			Start.setFillColor(Color::Red);
			howtoplay.setFillColor(Color::White);
			Highscore.setFillColor(Color::White);
			Exit.setFillColor(Color::White);
		}
		else if (selectedItem == 1)
		{
			Start.setFillColor(Color::White);
			howtoplay.setFillColor(Color::Red);
			Highscore.setFillColor(Color::White);
			Exit.setFillColor(Color::White);
		}
		else if (selectedItem == 2)
		{
			Start.setFillColor(Color::White);
			howtoplay.setFillColor(Color::White);
			Highscore.setFillColor(Color::Red);
			Exit.setFillColor(Color::White);
		}
		else if (selectedItem == 3)
		{
			Start.setFillColor(Color::White);
			howtoplay.setFillColor(Color::White);
			Highscore.setFillColor(Color::White);
			Exit.setFillColor(Color::Red);
		}
	}
}

HowToPlay::HowToPlay(float width, float height)
{
	if (!font.loadFromFile("FiddlersCoveRegular-Mgge.otf"))
	{
		//Handle error
		cout << "Can't load FiddlersCoveRegular font" << endl;
	}
	for (int i=0;i<7;i++)
	{
		key[i].setFont(font);
		key[i].setFillColor(Color::White);
		key[i].setPosition(Vector2f(width / 3, (height / 8) * (i + 1)));
	}
	key[0].setString("W : Up");
	key[1].setString("A : Left");
	key[2].setString("S : Down");
	key[3].setString("D : Right");
	key[4].setString("Left Mouse Click : Melee");
	key[5].setString("Right Mouse Click : Shoot bullet");
	key[6].setString("ESC : Quit to menu/Pause current game");
	
}

HowToPlay::~HowToPlay()
{
}

void HowToPlay::Draw(RenderWindow& rw)
{
	for (int i=0;i<7;i++)
	{
		rw.draw(key[i]);
	}
}

Pause::Pause(RenderWindow& rw)
{
	const float xSize = 700, ySize = 100;
	frame.setFillColor(Color(100,100,100));
	frame.setSize(Vector2f(xSize,ySize));
	frame.setPosition((float)(rw.getSize().x / 2)-20, (float)(rw.getSize().y / 2)-20);
	frame.setOrigin(xSize/2,ySize/2);
	if (!font.loadFromFile("Serithai-Regular.ttf"))
	{
		//Handle error
	}
	asking.setFont(font);
	asking.setPosition((float)(rw.getSize().x/2)-310,(float)(rw.getSize().y/2)-40);
	asking.setFillColor(Color::White);
	asking.setString("Do you want to continue playing? Y: Yes, N: No");
}

Pause::~Pause()
{
}

void Pause::Draw(RenderWindow& rw)
{
	rw.draw(frame);
	rw.draw(asking);
}
