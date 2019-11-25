#include<SFML/Graphics.hpp>
#include<SFML/Audio.hpp>
#include<iostream>
#include<fstream>
#include<algorithm>
#include"Animation.h"
#include"Player.h"
#include"Platform.h"
#include"Menu.h"
#include"Enemy.h"
#include"Score.h"
//TODO: GIVE BUFF TO PLAYER: Health++,Damage++
//TODO: GIVE BUFF TO ENEMIES: Speed++,Fast Spawn
//Equal 4 buffCombo

int buffComboRandom()
{
	srand(time(NULL));
	return rand() % 4;
}

bool sortinrev(const pair<int, string>& a, const pair<int, string>& b);//descending vector sort
#define FRAMERATE 40 //DON'T CHANGE THIS VALUE AT ALL!
#define RWWIDTH 1920
#define RWHEIGHT 1080
using namespace std;
using namespace sf;
struct LevelControl
{
	const unsigned int tankNO, casterNO, vampireNO;
	unsigned int minionSum;
}lvl[3] = { {25,10,10,0},{20,15,20,0},{30,20,50,0} };
struct Spawned
{
	unsigned int tank, caster, vampire;
}slvl[3] = { {0,0,0},{0,0,0},{0,0,0} };

int main()
{
	for (int i=0;i<3;i++)
	{
		lvl[i].minionSum = lvl[i].tankNO+lvl[i].casterNO+lvl[i].vampireNO;
	}
	
	/*
	//GOD MODE
	lvl[0].minionSum=10;
	lvl[1].minionSum=10;
	lvl[2].minionSum=10;
	*/
	
	bool win = false;
	RenderWindow rw(VideoMode(RWWIDTH, RWHEIGHT),"Dungeon Explorer 62010356", Style::Close | Style::Titlebar);
	View view(Vector2f(0.0f,0.0f), Vector2f(RWWIDTH, RWHEIGHT));
	rw.setFramerateLimit(FRAMERATE);

	Texture playerTex,backgroundTex;
	backgroundTex.loadFromFile("GameMap.png");
	backgroundTex.setRepeated(true);
	Sprite bg;
	bg.setTexture(backgroundTex);
	bg.setTextureRect(IntRect(0,0, backgroundTex.getSize().x ,backgroundTex.getSize().y));
	bg.setScale(Vector2f(RWWIDTH/bg.getLocalBounds().width,(RWHEIGHT-60)/bg.getLocalBounds().height));
	playerTex.loadFromFile("Player.png");
	playerTex.setRepeated(true);//Prevent flicker
	Player p(&playerTex, Vector2u(4, 6), 0.1f, 150.0f);
	RectangleShape playerMenuModel;
	playerMenuModel.setTexture(&playerTex);
	playerMenuModel.setTextureRect(IntRect(0,0,playerTex.getSize().x/4,playerTex.getSize().y/6));
	playerMenuModel.setSize(Vector2f(650,700));
	playerMenuModel.setPosition(-750+RWWIDTH/2,-300+RWHEIGHT/2);
	
	vector<Platform> wallVector;
	wallVector.push_back(Platform(nullptr, Vector2f(RWWIDTH*3, 100),Vector2f(0,RWHEIGHT-20)));
	wallVector.push_back(Platform(nullptr, Vector2f(RWWIDTH * 3, 40), Vector2f(0, 5)));
	wallVector.push_back(Platform(nullptr, Vector2f(40, RWHEIGHT*3), Vector2f(RWWIDTH-20, 0)));
	wallVector.push_back(Platform(nullptr, Vector2f(40, RWHEIGHT * 3), Vector2f(0, 5)));

	Music backgroundMusic;
	if (!backgroundMusic.openFromFile("Moonlight.wav"))
	{
		//cout << "Can't open music" << endl;
	}
	backgroundMusic.setLoop(true);
	backgroundMusic.play();
	backgroundMusic.setVolume(30);

	float deltaTime = 0.0f,totalTime=0.0f;
	Clock clk;

	Menu menu(RWWIDTH,RWHEIGHT);
	HowToPlay htp(RWWIDTH,RWHEIGHT);
	Pause pause(rw);
	int gameState = 0,Level=1;


	Texture bulletTex;
	bulletTex.loadFromFile("Bullet.png");
	p.setBulletTexture(&bulletTex);


	Clock iFrame;
	const float iFrameLength = 1.0f;

	float enemySpeedMult = 1, enemySpawnDivider = 1;
	float spawnTime = 1.5f;

	Texture tankTexture;
	tankTexture.loadFromFile("MinionTank.png");
	tankTexture.setRepeated(true);
	//Tank tank1(&tankTexture, Vector2u(4, 4), 0.1f, 60.0f);
	vector<Tank*> tankVector;
	//tankVector.push_back(new Tank(&tankTexture, Vector2u(4, 4), 0.1f, 60.0f));
	/*//DELETING METHOD
	Tank* P = tankVector.at(0);
	delete P;
	tankVector.erase(tankVector.begin() + 0);
	*/

	Texture vampireTexture;
	vampireTexture.loadFromFile("MinionVampire.png");
	vampireTexture.setRepeated(true);
	vector<Vampire*> vampireVector;
	//vampireVector.push_back(new Vampire(&vampireTexture,Vector2u(4,4),0.1f,80.0f));

	Texture casterTexture;
	casterTexture.loadFromFile("MinionCaster.png");
	casterTexture.setRepeated(true);
	vector<Caster*> casterVector;


	Score score((float)(10*rw.getSize().x/12),(float)(rw.getSize().y/20));
	
	//Name input part(End game)
	static RectangleShape inputField;
	inputField.setSize(Vector2f(300.0f, 70.0f));
	inputField.setOrigin(Vector2f(300.0f / 2, 70.0f / 2));
	inputField.setPosition(RWWIDTH / 2, RWHEIGHT / 2);
	static RectangleShape bar;
	bar.setFillColor(Color::Black);
	bar.setSize(Vector2f(5.0f, 64.0f));
	bar.setOrigin(Vector2f(5.0f / 2, 64.0f / 2));
	bar.setPosition(-140+RWWIDTH/2,RWHEIGHT/2);
	Clock blinkClock;
	bool blinkState = false;
	char lastChar = ' ';
	string playerName="";
	Font font;
	Text playerNameText;
	if (!font.loadFromFile("FiddlersCoveRegular-Mgge.otf"))
	{
		//cout << "Can't load font from file" << endl;
	}
	playerNameText.setFillColor(Color::Black);
	playerNameText.setFont(font);
	playerNameText.setPosition(Vector2f(-142 + RWWIDTH / 2, -35+RWHEIGHT / 2));
	playerNameText.setCharacterSize(60);
	Text winText, loseText,ask4name;
	winText.setFont(font);
	winText.setCharacterSize(72);
	winText.setFillColor(Color::White);
	winText.setString("You've Won!");
	winText.setOrigin(Vector2f(winText.getGlobalBounds().width/2,winText.getGlobalBounds().height/2));
	winText.setPosition(Vector2f(-30+RWWIDTH/2,RWHEIGHT/10));
	loseText.setFont(font);
	loseText.setCharacterSize(72);
	loseText.setFillColor(Color::White);
	loseText.setString("You lose");
	loseText.setOrigin(Vector2f(loseText.getGlobalBounds().width/2, loseText.getGlobalBounds().height/2));
	loseText.setPosition(Vector2f(-25+RWWIDTH / 2, RWHEIGHT / 10));
	ask4name.setFont(font);
	ask4name.setCharacterSize(72);
	ask4name.setString("Your name right here!");
	ask4name.setFillColor(Color::White);
	ask4name.setOrigin(Vector2f(ask4name.getGlobalBounds().width/2,ask4name.getGlobalBounds().height/2));
	ask4name.setPosition(Vector2f(RWWIDTH/2,RWHEIGHT/3));
	Texture lostTex;
	if (!lostTex.loadFromFile("lost.png"))
	{
		//cout << "can't load lost.png" << endl;
	}
	Sprite lostBG;
	lostBG.setTexture(lostTex);
	lostBG.setScale(Vector2f(1, RWHEIGHT / lostBG.getGlobalBounds().height));


	Texture hsBgText;
	RectangleShape backFrame;
	backFrame.setFillColor(Color(130,0,0,180));
	backFrame.setSize(Vector2f(1200,800));
	backFrame.setOrigin(backFrame.getSize().x/2,backFrame.getSize().y/2);
	backFrame.setPosition(60+RWWIDTH/2,30+RWHEIGHT/2);
	if (!hsBgText.loadFromFile("HighscoreBackground.png"))
	{
		//cout << "Can't load highscoreBackground texture"<< endl;
	}
	hsBgText.setRepeated(true);
	Sprite highscoreBackground;
	highscoreBackground.setTextureRect(IntRect(0,0,hsBgText.getSize().x,hsBgText.getSize().y));
	highscoreBackground.setTexture(hsBgText);
	highscoreBackground.setScale(Vector2f(RWWIDTH/ highscoreBackground.getGlobalBounds().width,RWHEIGHT/ highscoreBackground.getGlobalBounds().height));
	ifstream highscoreReader;
	highscoreReader.open("HighscoreFile.txt");
	Text highscoreNameText[5],highscoreScoreText[5],highscoreTitle;
	highscoreTitle.setFont(font);
	highscoreTitle.setCharacterSize(72);
	highscoreTitle.setString("---HIGHSCORE---");
	highscoreTitle.setOrigin(highscoreTitle.getGlobalBounds().width,highscoreTitle.getGlobalBounds().height);
	highscoreTitle.setFillColor(Color(43, 195, 94));
	highscoreTitle.setPosition(250+RWWIDTH/2,50+RWHEIGHT/20);
	for (int i=0;i<5;i++)
	{
		highscoreNameText[i].setFont(font);
		highscoreNameText[i].setFillColor(Color(43,195,94));
		highscoreNameText[i].setCharacterSize(50);
		highscoreNameText[i].setPosition(Vector2f((float)RWWIDTH/4,(float)RWHEIGHT/6*(i+1)));

		highscoreScoreText[i].setFont(font);
		highscoreScoreText[i].setFillColor(Color(43, 195, 94));
		highscoreScoreText[i].setCharacterSize(50);
		highscoreScoreText[i].setPosition(Vector2f((float)3*RWWIDTH / 4, (float)RWHEIGHT / 6 * (i + 1)));
	}
	vector<pair<int, string>> pairScoreName;
	string line;
	do
	{
		highscoreReader >> line;
		string highscoreScore = line.substr(0, line.find(','));
		string highscoreName = line.substr(line.find(',')+1,line.length());
		pairScoreName.push_back(make_pair(atoi(highscoreScore.c_str()),highscoreName));
	} while (highscoreReader.good());//Read until EOF
	sort(pairScoreName.begin(), pairScoreName.end(), sortinrev);
	for (size_t i=0;i<5;i++)
	{
		////cout << pairScoreName[i].first << "     " << pairScoreName[i].second << endl;
		highscoreNameText[i].setString(pairScoreName[i].second);
		highscoreScoreText[i].setString(to_string(pairScoreName[i].first));
	}
	highscoreReader.close();







	Sprite htpBG;
	Texture htpTex;
	if (!htpTex.loadFromFile("ScrollNew.jpg"))
	{
		//cout << "Can't load SCROLL.PNG" << endl;
	}
	htpBG.setTexture(htpTex);
	htpBG.setTextureRect(IntRect(20, 22, htpTex.getSize().x, htpTex.getSize().y));
	htpBG.setScale(Vector2f(1.05*RWWIDTH / htpBG.getGlobalBounds().width, 1.1*RWHEIGHT / htpBG.getGlobalBounds().height));

	Text levelName;
	levelName.setFont(font);
	levelName.setCharacterSize(72);
	levelName.setPosition(-35 + RWWIDTH / 2, -200 + RWHEIGHT / 2);
	levelName.setFillColor(Color(43, 195, 94));
	RectangleShape levelBackFrame;
	levelBackFrame.setFillColor(Color(130, 0, 0, 180));
	levelBackFrame.setSize(Vector2f(1000, 500));
	levelBackFrame.setOrigin(backFrame.getSize().x / 2, backFrame.getSize().y / 2);
	levelBackFrame.setPosition(60 + RWWIDTH / 2,120 + RWHEIGHT / 2);

	Text pressQ;
	pressQ.setFont(font);
	pressQ.setCharacterSize(72);
	pressQ.setFillColor(Color(43, 195, 94));
	pressQ.setPosition(-20+RWWIDTH/2,-100+4*RWHEIGHT/6);
	pressQ.setString("Press Q to continue");
	pressQ.setOrigin(pressQ.getGlobalBounds().width/2,pressQ.getGlobalBounds().height/2);
	//Game Loop
	while (rw.isOpen())
	{
		deltaTime = clk.restart().asSeconds();
		Event e;
		while (rw.pollEvent(e))
		{
			if (e.type == Event::Closed)
			{
				rw.close();
			}
			else if (e.type == Event::Resized)
			{
				//Maintain same image without stretching 
				FloatRect visibleArea(0, 0, (float)e.size.width, (float)e.size.height);
				rw.setView(View(visibleArea));
			}
			else if (e.type == Event::KeyReleased && gameState == 0)
			{
				switch (e.key.code)
				{
				case Keyboard::W: menu.MoveUp(); break;
				case Keyboard::S: menu.MoveDown(); break;
				case Keyboard::Return:
					switch (menu.getPressedItem())
					{
					case 0: cout << "Start game" << endl;
						gameState = 1000;//pre-game start
						break;
					case 1:  cout << "How to play menu" << endl;
						gameState = -1;//How to play
						break;
					case 2: cout << "Highscore menu" << endl;
						gameState = -3;//Highscore
						break;
					case 3: //cout << "Exit game" << endl;
						rw.close();
					}
					break;
				}
			}

		}
		rw.clear(Color(100, 100, 100));

		if (gameState == 0)//Menu state
		{
			for (int i = 0; i < 3; i++)
			{
				slvl[i].caster = 0;
				slvl[i].tank = 0;
				slvl[i].vampire = 0;
			}
			Level = 1;
			enemySpawnDivider = 1.0f;
			enemySpeedMult = 1.0f;
			menu.Draw(rw);
			p.resetStat();
			vampireVector.clear();
			tankVector.clear();
			casterVector.clear();
			pressQ.setPosition(-20 + RWWIDTH / 2, -100 + 4 * RWHEIGHT / 6);
			rw.draw(playerMenuModel);
		}
		else if (gameState == -1)//How to play
		{
			rw.draw(htpBG);
			htp.Draw(rw);
			if (e.type == Event::KeyReleased)
			{
				if (e.key.code == Keyboard::Escape)//Using same key cause "bouncing"?
				{
					gameState = 0;
				}
			}
		}
		else if (gameState == -2)//Pause
		{
			rw.draw(bg);
			pause.Draw(rw);
			if (e.key.code == Keyboard::Y)
			{
				gameState = Level;
			}
			else if (e.key.code == Keyboard::N)
			{
				gameState = 0;
			}
		}
		else if (gameState == -3)//Highscore
		{
			rw.draw(highscoreBackground);
			rw.draw(backFrame);
			rw.draw(highscoreTitle);
			for (int i = 0; i < 5; i++)
			{
				rw.draw(highscoreScoreText[i]);
				rw.draw(highscoreNameText[i]);
			}

			if (e.type == Event::KeyReleased)
			{
				if (e.key.code == Keyboard::Escape)
				{
					gameState = 0;
				}
			}
		}


		else if (gameState == 1000)//Waiting First level
		{
			rw.draw(bg);
			rw.draw(levelBackFrame);
			levelName.setString("Bullet sponge");
			levelName.setOrigin(levelName.getGlobalBounds().width / 2, levelName.getGlobalBounds().height / 2);
			rw.draw(levelName);
			rw.draw(pressQ);
			if (e.type == Event::KeyReleased)
			{
				if (e.key.code == Keyboard::Q)
				{
					gameState = 1;
				}
			}
		}
		else if (gameState == 1)//First Level
		{
			if (e.type == Event::KeyReleased)//Pause button
			{
				if (e.key.code == Keyboard::Escape)
				{
					gameState = -2;
					//cout << "Pause" << endl;
				}
			}

			//auto spawning
			totalTime += deltaTime;
			if (totalTime >= spawnTime)
			{
				int randomNumber = 1 + ((int)rand() % 3);
				if (randomNumber == 1)
				{
					if (slvl[0].tank++ < lvl[0].tankNO)
						tankVector.push_back(new Tank(&tankTexture, Vector2u(4, 4), 0.1f, 60.0f));
					else if (slvl[0].vampire++ < lvl[0].vampireNO)
						vampireVector.push_back(new Vampire(&vampireTexture, Vector2u(4, 4), 0.08f, 90.0f));
					else if (slvl[0].caster++ < lvl[0].casterNO)
						casterVector.push_back(new Caster(&casterTexture, Vector2u(4, 4), 0.1f, 80.0f));
				}
				if (randomNumber == 2)
				{
					if (slvl[0].vampire++ < lvl[0].vampireNO)
						vampireVector.push_back(new Vampire(&vampireTexture, Vector2u(4, 4), 0.08f, 90.0f));
					else if (slvl[0].tank++ < lvl[0].tankNO)
						tankVector.push_back(new Tank(&tankTexture, Vector2u(4, 4), 0.1f, 60.0f));
					else if (slvl[0].caster++ < lvl[0].casterNO)
						casterVector.push_back(new Caster(&casterTexture, Vector2u(4, 4), 0.1f, 80.0f));
				}
				if (randomNumber == 3)
				{
					if (slvl[0].caster++ < lvl[0].casterNO)
						casterVector.push_back(new Caster(&casterTexture, Vector2u(4, 4), 0.1f, 80.0f));
					else if (slvl[0].tank++ < lvl[0].tankNO)
						tankVector.push_back(new Tank(&tankTexture, Vector2u(4, 4), 0.1f, 60.0f));
					else if (slvl[0].vampire++ < lvl[0].vampireNO)
						vampireVector.push_back(new Vampire(&vampireTexture, Vector2u(4, 4), 0.08f, 90.0f));
				}
				totalTime = 0;
			}



			rw.draw(bg);
			//OOB COLLISION
			for (size_t i = 0; i < wallVector.size(); i++)//Player OOB
			{
				wallVector[i].GetCollider().CheckCollision(p.GetCollider(), 1.0f);
			}
			for (size_t i = 0; i < tankVector.size(); i++)//Tank OOB
			{
				for (size_t j = 0; j < wallVector.size(); j++)
				{
					wallVector[j].GetCollider().CheckCollision(tankVector[i]->GetCollider(), 1.0f);
				}
			}
			for (size_t i = 0; i < vampireVector.size(); i++)//Vampire OOB
			{
				for (size_t j = 0; j < wallVector.size(); j++)
				{
					wallVector[j].GetCollider().CheckCollision(vampireVector[i]->GetCollider(), 1.0f);
				}
			}
			for (size_t i = 0; i < casterVector.size(); i++)//Caster OOB
			{
				for (size_t j = 0; j < wallVector.size(); j++)
				{
					wallVector[j].GetCollider().CheckCollision(casterVector[i]->GetCollider(), 1.0f);
				}
			}




			//COLLISION
			for (size_t i = 0; i < p.bulletVectorSize(); i++)//Bullet-Tank collision
			{
				for (size_t j = 0; j < tankVector.size(); j++)
				{
					if (p.bulletCollision(tankVector[j]->GetCollider(), i, 0.0f))
					{
						//cout << "Shot the Tank!" << endl;
						p.setBulletPosition(i);
						tankVector[j]->takeDamage(p.bulletDamage());
					}
				}
			}
			for (size_t i = 0; i < p.bulletVectorSize(); i++)//Bullet-Vampire collision
			{
				for (size_t j = 0; j < vampireVector.size(); j++)
				{
					if (p.bulletCollision(vampireVector[j]->GetCollider(), i, 0.0f))
					{
						//cout << "Shot the Vampire!" << endl;
						p.setBulletPosition(i);
						vampireVector[j]->takeDamage(p.bulletDamage());
					}
				}
			}
			for (size_t i = 0; i < p.bulletVectorSize(); i++)//Bullet-Caster collision
			{
				for (size_t j = 0; j < casterVector.size(); j++)
				{
					if (p.bulletCollision(casterVector[j]->GetCollider(), i, 0.0f))
					{
						//cout << "Shot the Caster!" << endl;
						p.setBulletPosition(i);
						casterVector[j]->takeDamage(p.bulletDamage());
					}
				}
			}

			//TAKING DAMAGE
			for (size_t i = 0; i < tankVector.size(); i++)//Tank-Player Punch Intersect
			{
				if (tankVector[i]->GetCollider().CheckIntersect(p.GetCollider()))
				{
					if (iFrame.getElapsedTime().asSeconds() > iFrameLength)
					{
						//cout << "Taking damage from TANK" << endl;
						p.takeDamage(tankVector[i]->meleeDamage());
						iFrame.restart();
					}
				}
			}
			for (size_t i = 0; i < vampireVector.size(); i++)//Vampire-Player Punch Intersect
			{
				if (vampireVector[i]->GetCollider().CheckIntersect(p.GetCollider()))
				{
					if (iFrame.getElapsedTime().asSeconds() > iFrameLength)
					{
						//cout << "Taking damage from VAMPIRE" << endl;
						p.takeDamage(vampireVector[i]->meleeDamage());
						iFrame.restart();
					}
				}
			}
			for (size_t i = 0; i < casterVector.size(); i++)
			{
				for (size_t b = 0; b < casterVector[i]->bulletVectorSize(); b++)
				{
					if (casterVector[i]->bulletCollision(p.GetCollider(), b, 0.0f))
					{
						casterVector[i]->setBulletPosition(b);
						if (iFrame.getElapsedTime().asSeconds() > iFrameLength)
						{
							//cout << "Taking damage from CASTER" << endl;
							p.takeDamage(casterVector[i]->bulletDamage());
							iFrame.restart();
						}
					}
				}
			}


			//PUNCH INTERSECTION
			for (size_t i = 0; i < tankVector.size(); i++)//Player-Tank Punch Intersect
			{
				if (p.GetPunchCollider().CheckIntersect(tankVector[i]->GetCollider()) && p.punching())
				{
					//cout << "Punch Tank!" << endl;
					tankVector[i]->takeDamage(p.meleeDamage());
				}
			}
			for (size_t i = 0; i < vampireVector.size(); i++)//Player-Vampire Punch Intersect
			{
				if (p.GetPunchCollider().CheckIntersect(vampireVector[i]->GetCollider()) && p.punching())
				{
					//cout << "Punch Vampire!" << endl;
					vampireVector[i]->takeDamage(p.meleeDamage());
				}
			}
			for (size_t i = 0; i < casterVector.size(); i++)//Player-Caster Punch Intersect
			{
				if (p.GetPunchCollider().CheckIntersect(casterVector[i]->GetCollider()) && p.punching())
				{
					//cout << "Punch Caster!" << endl;
					casterVector[i]->takeDamage(p.meleeDamage());
				}
			}

			//Kill Minion
			for (size_t i = 0; i < tankVector.size(); i++)
			{
				if (tankVector[i]->isDead(rw, deltaTime))
				{
					score.updateScore(tankVector[i]->killScore());
					p.increaseKillCount();
					//cout << "TANK " << i << " HAS BEEN KILLED!" << endl;
					if (tankVector.size() > 1)
					{
						Tank* tankPointer;
						tankPointer = tankVector.at(i);
						delete tankPointer;
						tankVector.erase(tankVector.begin() + i);
					}
					else
					{
						//cout << "ALL TANK ARE DEAD!" << endl;
						tankVector.clear();
						break;
					}

				}
			}
			for (size_t i = 0; i < vampireVector.size(); i++)
			{
				if (vampireVector[i]->isDead(rw, deltaTime))
				{
					score.updateScore(vampireVector[i]->killScore());
					p.increaseKillCount();
					//cout << "VAMPIRE " << i << " HAS BEEN KILLED!" << endl;
					if (vampireVector.size() > 1)
					{
						Vampire* vampirePointer;
						vampirePointer = vampireVector.at(i);
						delete vampirePointer;
						vampireVector.erase(vampireVector.begin() + i);
					}
					else
					{
						//cout << "ALL VAMPIRE ARE DEAD!" << endl;
						vampireVector.clear();
						break;
					}

				}
			}
			for (size_t i = 0; i < casterVector.size(); i++)
			{
				if (casterVector[i]->isDead(rw, deltaTime))
				{
					score.updateScore(casterVector[i]->killScore());
					p.increaseKillCount();
					//cout << "CASTER " << i << " HAS BEEN KILLED!" << endl;
					if (casterVector.size() > 1)
					{
						Caster* casterPointer;
						casterPointer = casterVector.at(i);
						delete casterPointer;
						casterVector.erase(casterVector.begin() + i);
					}
					else
					{
						//cout << "ALL CASTER ARE DEAD!" << endl;
						casterVector.clear();
						break;
					}

				}
			}



			//OUT OF BOUND DELETE
			for (size_t i = 0; i < p.bulletVectorSize(); i++)
			{
				if (p.getBulletPosition(i).x > rw.getSize().x || p.getBulletPosition(i).y > rw.getSize().y || p.getBulletPosition(i).x < 0 || p.getBulletPosition(i).y < 0)
				{
					//cout << p.getBulletPosition(i).x << "   " << p.getBulletPosition(i).y << endl;
					if (p.bulletVectorSize() > 1)
					{
						p.eraseBullet(i);
					}
					else
					{
						p.clearBulletVector();
						//cout << "Clear the vector" << endl;
					}
				}
			}
			for (size_t i = 0; i < casterVector.size(); i++)
			{
				for (size_t b = 0; b < casterVector[i]->bulletVectorSize(); b++)
				{
					if (casterVector[i]->getBulletPosition(b).x > rw.getSize().x || casterVector[i]->getBulletPosition(b).y > rw.getSize().y || casterVector[i]->getBulletPosition(b).x < 0 || casterVector[i]->getBulletPosition(b).y < 0)
					{
						if (casterVector[i]->bulletVectorSize() > 1)
						{
							casterVector[i]->eraseBullet(b);
						}
						else
						{
							casterVector[i]->clearBulletVector();
							//cout << "Clear the vector" << endl;
						}
					}
				}
			}


			//Render & Update
			for (size_t i = 0; i < tankVector.size(); i++)
			{
				tankVector[i]->Draw(rw);
				tankVector[i]->Update(deltaTime, rw, p);
			}
			for (size_t i = 0; i < vampireVector.size(); i++)
			{
				vampireVector[i]->Draw(rw);
				vampireVector[i]->Update(deltaTime, rw, p);
			}
			for (size_t i = 0; i < casterVector.size(); i++)
			{
				casterVector[i]->Draw(rw, deltaTime);
				casterVector[i]->Update(deltaTime, rw, p);
			}
			p.Update(deltaTime, rw);
			p.Draw(rw);
			score.Draw(rw);
			/*
			for (size_t i=0;i<wallVector.size();i++)
			{
				wallVector[i].Draw(rw);
			}
			*/
			//cout << p.getKillCount()<< endl;
			//IF WIN: Level++
			if (p.getKillCount() >= lvl[0].minionSum)
			{
				//win = true;
				Level = 2;
				score.updateScore(2000);
				p.manualWalkSoundStop();
				p.resetStat();
				tankVector.clear();
				vampireVector.clear();
				casterVector.clear();
				gameState = 1001;//Next wating level
			}

			//IF LOSE: GO TO ENTER SCORE SCREEN
			if (p.isDead(rw, deltaTime))
			{
				//cout << "You died" << endl;
				win = false;
				gameState = 1004;//Score menu
			}
		}

		else if (gameState == 1001)//Waiting second level
		{
		rw.draw(bg);
		rw.draw(levelBackFrame);
			levelName.setString("Vampire will (never) hurt you");
			levelName.setOrigin(levelName.getGlobalBounds().width / 2, levelName.getGlobalBounds().height / 2);
			rw.draw(levelName);
			rw.draw(pressQ);
			if (e.type == Event::KeyReleased)
			{
				if (e.key.code == Keyboard::Q)
				{
					enemySpawnDivider = 1.0f;
					enemySpeedMult = 1.0f;
					int buffCombo = buffComboRandom();
					//cout <<"Buff Combo: "<< buffCombo << endl;
					switch (buffCombo)
					{
					case 0: p.damageBuff(); enemySpeedMult = 1.2f; break;
					case 1: p.damageBuff(); enemySpawnDivider = 1.5f; break;//spawn rate to 1 sec/spawn
					case 2: p.healthBuff(); enemySpeedMult = 1.2f; break;
					case 3: p.healthBuff(); enemySpawnDivider = 1.5f; break;
					}
					gameState = 2;
				}
			}
		}
		else if (gameState == 2)
		{
			if (e.type == Event::KeyReleased)//Pause button
			{
				if (e.key.code == Keyboard::Escape)
				{
					gameState = -2;
					//cout << "Pause" << endl;
				}
			}

			//auto spawning
			totalTime += deltaTime;
			if (totalTime >= spawnTime/enemySpawnDivider)
			{
				int randomNumber = 1 + ((int)rand() % 3);
				if (randomNumber == 1)
				{
					if (slvl[1].tank++ < lvl[1].tankNO)
						tankVector.push_back(new Tank(&tankTexture, Vector2u(4, 4), 0.1f, 60.0f*enemySpeedMult));
					else if (slvl[1].vampire++ < lvl[1].vampireNO)
						vampireVector.push_back(new Vampire(&vampireTexture, Vector2u(4, 4), 0.08f, 90.0f * enemySpeedMult));
					else if (slvl[1].caster++ < lvl[1].casterNO)
						casterVector.push_back(new Caster(&casterTexture, Vector2u(4, 4), 0.1f, 80.0f * enemySpeedMult));
				}
				if (randomNumber == 2)
				{
					if (slvl[1].vampire++ < lvl[1].vampireNO)
						vampireVector.push_back(new Vampire(&vampireTexture, Vector2u(4, 4), 0.08f, 90.0f * enemySpeedMult));
					else if (slvl[1].tank++ < lvl[1].tankNO)
						tankVector.push_back(new Tank(&tankTexture, Vector2u(4, 4), 0.1f, 60.0f * enemySpeedMult));
					else if (slvl[1].caster++ < lvl[1].casterNO)
						casterVector.push_back(new Caster(&casterTexture, Vector2u(4, 4), 0.1f, 80.0f * enemySpeedMult));
				}
				if (randomNumber == 3)
				{
					if (slvl[1].caster++ < lvl[1].casterNO)
						casterVector.push_back(new Caster(&casterTexture, Vector2u(4, 4), 0.1f, 80.0f * enemySpeedMult));
					else if (slvl[1].tank++ < lvl[1].tankNO)
						tankVector.push_back(new Tank(&tankTexture, Vector2u(4, 4), 0.1f, 60.0f * enemySpeedMult));
					else if (slvl[1].vampire++ < lvl[1].vampireNO)
						vampireVector.push_back(new Vampire(&vampireTexture, Vector2u(4, 4), 0.08f, 90.0f * enemySpeedMult));
				}
				totalTime = 0;
			}



			rw.draw(bg);
			//OOB COLLISION
			for (size_t i = 0; i < wallVector.size(); i++)//Player OOB
			{
				wallVector[i].GetCollider().CheckCollision(p.GetCollider(), 1.0f);
			}
			for (size_t i = 0; i < tankVector.size(); i++)//Tank OOB
			{
				for (size_t j = 0; j < wallVector.size(); j++)
				{
					wallVector[j].GetCollider().CheckCollision(tankVector[i]->GetCollider(), 1.0f);
				}
			}
			for (size_t i = 0; i < vampireVector.size(); i++)//Vampire OOB
			{
				for (size_t j = 0; j < wallVector.size(); j++)
				{
					wallVector[j].GetCollider().CheckCollision(vampireVector[i]->GetCollider(), 1.0f);
				}
			}
			for (size_t i = 0; i < casterVector.size(); i++)//Caster OOB
			{
				for (size_t j = 0; j < wallVector.size(); j++)
				{
					wallVector[j].GetCollider().CheckCollision(casterVector[i]->GetCollider(), 1.0f);
				}
			}




			//COLLISION
			for (size_t i = 0; i < p.bulletVectorSize(); i++)//Bullet-Tank collision
			{
				for (size_t j = 0; j < tankVector.size(); j++)
				{
					if (p.bulletCollision(tankVector[j]->GetCollider(), i, 0.0f))
					{
						//cout << "Shot the Tank!" << endl;
						p.setBulletPosition(i);
						tankVector[j]->takeDamage(p.bulletDamage());
					}
				}
			}
			for (size_t i = 0; i < p.bulletVectorSize(); i++)//Bullet-Vampire collision
			{
				for (size_t j = 0; j < vampireVector.size(); j++)
				{
					if (p.bulletCollision(vampireVector[j]->GetCollider(), i, 0.0f))
					{
						//cout << "Shot the Vampire!" << endl;
						p.setBulletPosition(i);
						vampireVector[j]->takeDamage(p.bulletDamage());
					}
				}
			}
			for (size_t i = 0; i < p.bulletVectorSize(); i++)//Bullet-Caster collision
			{
				for (size_t j = 0; j < casterVector.size(); j++)
				{
					if (p.bulletCollision(casterVector[j]->GetCollider(), i, 0.0f))
					{
						//cout << "Shot the Caster!" << endl;
						p.setBulletPosition(i);
						casterVector[j]->takeDamage(p.bulletDamage());
					}
				}
			}

			//TAKING DAMAGE
			for (size_t i = 0; i < tankVector.size(); i++)//Tank-Player Punch Intersect
			{
				if (tankVector[i]->GetCollider().CheckIntersect(p.GetCollider()))
				{
					if (iFrame.getElapsedTime().asSeconds() > iFrameLength)
					{
						//cout << "Taking damage from TANK" << endl;
						p.takeDamage(tankVector[i]->meleeDamage());
						iFrame.restart();
					}
				}
			}
			for (size_t i = 0; i < vampireVector.size(); i++)//Vampire-Player Punch Intersect
			{
				if (vampireVector[i]->GetCollider().CheckIntersect(p.GetCollider()))
				{
					if (iFrame.getElapsedTime().asSeconds() > iFrameLength)
					{
						//cout << "Taking damage from VAMPIRE" << endl;
						p.takeDamage(vampireVector[i]->meleeDamage());
						iFrame.restart();
					}
				}
			}
			for (size_t i = 0; i < casterVector.size(); i++)
			{
				for (size_t b = 0; b < casterVector[i]->bulletVectorSize(); b++)
				{
					if (casterVector[i]->bulletCollision(p.GetCollider(), b, 0.0f))
					{
						casterVector[i]->setBulletPosition(b);
						if (iFrame.getElapsedTime().asSeconds() > iFrameLength)
						{
							//cout << "Taking damage from CASTER" << endl;
							p.takeDamage(casterVector[i]->bulletDamage());
							iFrame.restart();
						}
					}
				}
			}


			//PUNCH INTERSECTION
			for (size_t i = 0; i < tankVector.size(); i++)//Player-Tank Punch Intersect
			{
				if (p.GetPunchCollider().CheckIntersect(tankVector[i]->GetCollider()) && p.punching())
				{
					//cout << "Punch Tank!" << endl;
					tankVector[i]->takeDamage(p.meleeDamage());
				}
			}
			for (size_t i = 0; i < vampireVector.size(); i++)//Player-Vampire Punch Intersect
			{
				if (p.GetPunchCollider().CheckIntersect(vampireVector[i]->GetCollider()) && p.punching())
				{
					//cout << "Punch Vampire!" << endl;
					vampireVector[i]->takeDamage(p.meleeDamage());
				}
			}
			for (size_t i = 0; i < casterVector.size(); i++)//Player-Caster Punch Intersect
			{
				if (p.GetPunchCollider().CheckIntersect(casterVector[i]->GetCollider()) && p.punching())
				{
					//cout << "Punch Caster!" << endl;
					casterVector[i]->takeDamage(p.meleeDamage());
				}
			}

			//Kill Minion
			for (size_t i = 0; i < tankVector.size(); i++)
			{
				if (tankVector[i]->isDead(rw, deltaTime))
				{
					score.updateScore(tankVector[i]->killScore());
					p.increaseKillCount();
					//cout << "TANK " << i << " HAS BEEN KILLED!" << endl;
					if (tankVector.size() > 1)
					{
						Tank* tankPointer;
						tankPointer = tankVector.at(i);
						delete tankPointer;
						tankVector.erase(tankVector.begin() + i);
					}
					else
					{
						//cout << "ALL TANK ARE DEAD!" << endl;
						tankVector.clear();
						break;
					}

				}
			}
			for (size_t i = 0; i < vampireVector.size(); i++)
			{
				if (vampireVector[i]->isDead(rw, deltaTime))
				{
					score.updateScore(vampireVector[i]->killScore());
					p.increaseKillCount();
					//cout << "VAMPIRE " << i << " HAS BEEN KILLED!" << endl;
					if (vampireVector.size() > 1)
					{
						Vampire* vampirePointer;
						vampirePointer = vampireVector.at(i);
						delete vampirePointer;
						vampireVector.erase(vampireVector.begin() + i);
					}
					else
					{
						//cout << "ALL VAMPIRE ARE DEAD!" << endl;
						vampireVector.clear();
						break;
					}

				}
			}
			for (size_t i = 0; i < casterVector.size(); i++)
			{
				if (casterVector[i]->isDead(rw, deltaTime))
				{
					score.updateScore(casterVector[i]->killScore());
					p.increaseKillCount();
					//cout << "CASTER " << i << " HAS BEEN KILLED!" << endl;
					if (casterVector.size() > 1)
					{
						Caster* casterPointer;
						casterPointer = casterVector.at(i);
						delete casterPointer;
						casterVector.erase(casterVector.begin() + i);
					}
					else
					{
						//cout << "ALL CASTER ARE DEAD!" << endl;
						casterVector.clear();
						break;
					}

				}
			}



			//OUT OF BOUND DELETE
			for (size_t i = 0; i < p.bulletVectorSize(); i++)
			{
				if (p.getBulletPosition(i).x > rw.getSize().x || p.getBulletPosition(i).y > rw.getSize().y || p.getBulletPosition(i).x < 0 || p.getBulletPosition(i).y < 0)
				{
					//cout << p.getBulletPosition(i).x << "   " << p.getBulletPosition(i).y << endl;
					if (p.bulletVectorSize() > 1)
					{
						p.eraseBullet(i);
					}
					else
					{
						p.clearBulletVector();
						//cout << "Clear the vector" << endl;
					}
				}
			}
			for (size_t i = 0; i < casterVector.size(); i++)
			{
				for (size_t b = 0; b < casterVector[i]->bulletVectorSize(); b++)
				{
					if (casterVector[i]->getBulletPosition(b).x > rw.getSize().x || casterVector[i]->getBulletPosition(b).y > rw.getSize().y || casterVector[i]->getBulletPosition(b).x < 0 || casterVector[i]->getBulletPosition(b).y < 0)
					{
						if (casterVector[i]->bulletVectorSize() > 1)
						{
							casterVector[i]->eraseBullet(b);
						}
						else
						{
							casterVector[i]->clearBulletVector();
							//cout << "Clear the vector" << endl;
						}
					}
				}
			}


			//Render & Update
			for (size_t i = 0; i < tankVector.size(); i++)
			{
				tankVector[i]->Draw(rw);
				tankVector[i]->Update(deltaTime, rw, p);
			}
			for (size_t i = 0; i < vampireVector.size(); i++)
			{
				vampireVector[i]->Draw(rw);
				vampireVector[i]->Update(deltaTime, rw, p);
			}
			for (size_t i = 0; i < casterVector.size(); i++)
			{
				casterVector[i]->Draw(rw, deltaTime);
				casterVector[i]->Update(deltaTime, rw, p);
			}
			p.Update(deltaTime, rw);
			p.Draw(rw);
			score.Draw(rw);

			//cout << p.getKillCount() << endl;
			//IF WIN: Level++
			if (p.getKillCount() >= lvl[1].minionSum)
			{
				//win = true;
				Level = 3;
				score.updateScore(2000);
				p.manualWalkSoundStop();
				p.resetStat();
				tankVector.clear();
				vampireVector.clear();
				casterVector.clear();
				gameState = 1002;//Next wating level
			}

			//IF LOSE: GO TO ENTER SCORE SCREEN
			if (p.isDead(rw, deltaTime))
			{
				//cout << "You died" << endl;
				win = false;
				gameState = 1004;//Score menu
			}

		}

		else if (gameState == 1002)//Wait third level
		{
		rw.draw(bg);
		rw.draw(levelBackFrame);
			levelName.setString("100 Evil Souls");
			levelName.setOrigin(levelName.getGlobalBounds().width / 2, levelName.getGlobalBounds().height / 2);
			rw.draw(levelName);
			rw.draw(pressQ);
			if (e.type == Event::KeyReleased)
			{
				if (e.key.code == Keyboard::Q)
				{
					enemySpawnDivider = 1.0f;
					enemySpeedMult = 1.0f;
					int buffCombo = buffComboRandom();
					//cout << "Buff Combo: " << buffCombo << endl;
					switch (buffCombo)
					{
					case 0: p.damageBuff(); enemySpeedMult = 1.2f; break;
					case 1: p.damageBuff(); enemySpawnDivider = 1.5f; break;//spawn rate to 1 sec/spawn
					case 2: p.healthBuff(); enemySpeedMult = 1.2f; break;
					case 3: p.healthBuff(); enemySpawnDivider = 1.5f; break;
					}
					gameState = 3;
				}
			}
		}
		else if (gameState == 3)
		{
		if (e.type == Event::KeyReleased)//Pause button
		{
			if (e.key.code == Keyboard::Escape)
			{
				gameState = -2;
				//cout << "Pause" << endl;
			}
		}

		//auto spawning
		totalTime += deltaTime;
		if (totalTime >= spawnTime/enemySpawnDivider)
		{
			int randomNumber = 1 + ((int)rand() % 3);
			if (randomNumber == 1)
			{
				if (slvl[2].tank++ < lvl[2].tankNO)
					tankVector.push_back(new Tank(&tankTexture, Vector2u(4, 4), 0.1f, 60.0f * enemySpeedMult));
				else if (slvl[2].vampire++ < lvl[2].vampireNO)
					vampireVector.push_back(new Vampire(&vampireTexture, Vector2u(4, 4), 0.08f, 90.0f * enemySpeedMult));
				else if (slvl[2].caster++ < lvl[2].casterNO)
					casterVector.push_back(new Caster(&casterTexture, Vector2u(4, 4), 0.1f, 80.0f * enemySpeedMult));
			}
			if (randomNumber == 2)
			{
				if (slvl[2].vampire++ < lvl[2].vampireNO)
					vampireVector.push_back(new Vampire(&vampireTexture, Vector2u(4, 4), 0.08f, 90.0f * enemySpeedMult));
				else if (slvl[2].tank++ < lvl[2].tankNO)
					tankVector.push_back(new Tank(&tankTexture, Vector2u(4, 4), 0.1f, 60.0f * enemySpeedMult));
				else if (slvl[2].caster++ < lvl[2].casterNO)
					casterVector.push_back(new Caster(&casterTexture, Vector2u(4, 4), 0.1f, 80.0f * enemySpeedMult));
			}
			if (randomNumber == 3)
			{
				if (slvl[2].caster++ < lvl[2].casterNO)
					casterVector.push_back(new Caster(&casterTexture, Vector2u(4, 4), 0.1f, 80.0f * enemySpeedMult));
				else if (slvl[2].tank++ < lvl[2].tankNO)
					tankVector.push_back(new Tank(&tankTexture, Vector2u(4, 4), 0.1f, 60.0f * enemySpeedMult));
				else if (slvl[2].vampire++ < lvl[2].vampireNO)
					vampireVector.push_back(new Vampire(&vampireTexture, Vector2u(4, 4), 0.08f, 90.0f * enemySpeedMult));
			}
			totalTime = 0;
		}



		rw.draw(bg);
		//OOB COLLISION
		for (size_t i = 0; i < wallVector.size(); i++)//Player OOB
		{
			wallVector[i].GetCollider().CheckCollision(p.GetCollider(), 1.0f);
		}
		for (size_t i = 0; i < tankVector.size(); i++)//Tank OOB
		{
			for (size_t j = 0; j < wallVector.size(); j++)
			{
				wallVector[j].GetCollider().CheckCollision(tankVector[i]->GetCollider(), 1.0f);
			}
		}
		for (size_t i = 0; i < vampireVector.size(); i++)//Vampire OOB
		{
			for (size_t j = 0; j < wallVector.size(); j++)
			{
				wallVector[j].GetCollider().CheckCollision(vampireVector[i]->GetCollider(), 1.0f);
			}
		}
		for (size_t i = 0; i < casterVector.size(); i++)//Caster OOB
		{
			for (size_t j = 0; j < wallVector.size(); j++)
			{
				wallVector[j].GetCollider().CheckCollision(casterVector[i]->GetCollider(), 1.0f);
			}
		}




		//COLLISION
		for (size_t i = 0; i < p.bulletVectorSize(); i++)//Bullet-Tank collision
		{
			for (size_t j = 0; j < tankVector.size(); j++)
			{
				if (p.bulletCollision(tankVector[j]->GetCollider(), i, 0.0f))
				{
					//cout << "Shot the Tank!" << endl;
					p.setBulletPosition(i);
					tankVector[j]->takeDamage(p.bulletDamage());
				}
			}
		}
		for (size_t i = 0; i < p.bulletVectorSize(); i++)//Bullet-Vampire collision
		{
			for (size_t j = 0; j < vampireVector.size(); j++)
			{
				if (p.bulletCollision(vampireVector[j]->GetCollider(), i, 0.0f))
				{
					//cout << "Shot the Vampire!" << endl;
					p.setBulletPosition(i);
					vampireVector[j]->takeDamage(p.bulletDamage());
				}
			}
		}
		for (size_t i = 0; i < p.bulletVectorSize(); i++)//Bullet-Caster collision
		{
			for (size_t j = 0; j < casterVector.size(); j++)
			{
				if (p.bulletCollision(casterVector[j]->GetCollider(), i, 0.0f))
				{
					//cout << "Shot the Caster!" << endl;
					p.setBulletPosition(i);
					casterVector[j]->takeDamage(p.bulletDamage());
				}
			}
		}

		//TAKING DAMAGE
		for (size_t i = 0; i < tankVector.size(); i++)//Tank-Player Punch Intersect
		{
			if (tankVector[i]->GetCollider().CheckIntersect(p.GetCollider()))
			{
				if (iFrame.getElapsedTime().asSeconds() > iFrameLength)
				{
					//cout << "Taking damage from TANK" << endl;
					p.takeDamage(tankVector[i]->meleeDamage());
					iFrame.restart();
				}
			}
		}
		for (size_t i = 0; i < vampireVector.size(); i++)//Vampire-Player Punch Intersect
		{
			if (vampireVector[i]->GetCollider().CheckIntersect(p.GetCollider()))
			{
				if (iFrame.getElapsedTime().asSeconds() > iFrameLength)
				{
					//cout << "Taking damage from VAMPIRE" << endl;
					p.takeDamage(vampireVector[i]->meleeDamage());
					iFrame.restart();
				}
			}
		}
		for (size_t i = 0; i < casterVector.size(); i++)
		{
			for (size_t b = 0; b < casterVector[i]->bulletVectorSize(); b++)
			{
				if (casterVector[i]->bulletCollision(p.GetCollider(), b, 0.0f))
				{
					casterVector[i]->setBulletPosition(b);
					if (iFrame.getElapsedTime().asSeconds() > iFrameLength)
					{
						//cout << "Taking damage from CASTER" << endl;
						p.takeDamage(casterVector[i]->bulletDamage());
						iFrame.restart();
					}
				}
			}
		}


		//PUNCH INTERSECTION
		for (size_t i = 0; i < tankVector.size(); i++)//Player-Tank Punch Intersect
		{
			if (p.GetPunchCollider().CheckIntersect(tankVector[i]->GetCollider()) && p.punching())
			{
				//cout << "Punch Tank!" << endl;
				tankVector[i]->takeDamage(p.meleeDamage());
			}
		}
		for (size_t i = 0; i < vampireVector.size(); i++)//Player-Vampire Punch Intersect
		{
			if (p.GetPunchCollider().CheckIntersect(vampireVector[i]->GetCollider()) && p.punching())
			{
				//cout << "Punch Vampire!" << endl;
				vampireVector[i]->takeDamage(p.meleeDamage());
			}
		}
		for (size_t i = 0; i < casterVector.size(); i++)//Player-Caster Punch Intersect
		{
			if (p.GetPunchCollider().CheckIntersect(casterVector[i]->GetCollider()) && p.punching())
			{
				//cout << "Punch Caster!" << endl;
				casterVector[i]->takeDamage(p.meleeDamage());
			}
		}

		//Kill Minion
		for (size_t i = 0; i < tankVector.size(); i++)
		{
			if (tankVector[i]->isDead(rw, deltaTime))
			{
				score.updateScore(tankVector[i]->killScore());
				p.increaseKillCount();
				//cout << "TANK " << i << " HAS BEEN KILLED!" << endl;
				if (tankVector.size() > 1)
				{
					Tank* tankPointer;
					tankPointer = tankVector.at(i);
					delete tankPointer;
					tankVector.erase(tankVector.begin() + i);
				}
				else
				{
					//cout << "ALL TANK ARE DEAD!" << endl;
					tankVector.clear();
					break;
				}

			}
		}
		for (size_t i = 0; i < vampireVector.size(); i++)
		{
			if (vampireVector[i]->isDead(rw, deltaTime))
			{
				score.updateScore(vampireVector[i]->killScore());
				p.increaseKillCount();
				//cout << "VAMPIRE " << i << " HAS BEEN KILLED!" << endl;
				if (vampireVector.size() > 1)
				{
					Vampire* vampirePointer;
					vampirePointer = vampireVector.at(i);
					delete vampirePointer;
					vampireVector.erase(vampireVector.begin() + i);
				}
				else
				{
					//cout << "ALL VAMPIRE ARE DEAD!" << endl;
					vampireVector.clear();
					break;
				}

			}
		}
		for (size_t i = 0; i < casterVector.size(); i++)
		{
			if (casterVector[i]->isDead(rw, deltaTime))
			{
				score.updateScore(casterVector[i]->killScore());
				p.increaseKillCount();
				//cout << "CASTER " << i << " HAS BEEN KILLED!" << endl;
				if (casterVector.size() > 1)
				{
					Caster* casterPointer;
					casterPointer = casterVector.at(i);
					delete casterPointer;
					casterVector.erase(casterVector.begin() + i);
				}
				else
				{
					//cout << "ALL CASTER ARE DEAD!" << endl;
					casterVector.clear();
					break;
				}

			}
		}



		//OUT OF BOUND DELETE
		for (size_t i = 0; i < p.bulletVectorSize(); i++)
		{
			if (p.getBulletPosition(i).x > rw.getSize().x || p.getBulletPosition(i).y > rw.getSize().y || p.getBulletPosition(i).x < 0 || p.getBulletPosition(i).y < 0)
			{
				//cout << p.getBulletPosition(i).x << "   " << p.getBulletPosition(i).y << endl;
				if (p.bulletVectorSize() > 1)
				{
					p.eraseBullet(i);
				}
				else
				{
					p.clearBulletVector();
					//cout << "Clear the vector" << endl;
				}
			}
		}
		for (size_t i = 0; i < casterVector.size(); i++)
		{
			for (size_t b = 0; b < casterVector[i]->bulletVectorSize(); b++)
			{
				if (casterVector[i]->getBulletPosition(b).x > rw.getSize().x || casterVector[i]->getBulletPosition(b).y > rw.getSize().y || casterVector[i]->getBulletPosition(b).x < 0 || casterVector[i]->getBulletPosition(b).y < 0)
				{
					if (casterVector[i]->bulletVectorSize() > 1)
					{
						casterVector[i]->eraseBullet(b);
					}
					else
					{
						casterVector[i]->clearBulletVector();
						//cout << "Clear the vector" << endl;
					}
				}
			}
		}


		//Render & Update
		for (size_t i = 0; i < tankVector.size(); i++)
		{
			tankVector[i]->Draw(rw);
			tankVector[i]->Update(deltaTime, rw, p);
		}
		for (size_t i = 0; i < vampireVector.size(); i++)
		{
			vampireVector[i]->Draw(rw);
			vampireVector[i]->Update(deltaTime, rw, p);
		}
		for (size_t i = 0; i < casterVector.size(); i++)
		{
			casterVector[i]->Draw(rw, deltaTime);
			casterVector[i]->Update(deltaTime, rw, p);
		}
		p.Update(deltaTime, rw);
		p.Draw(rw);
		score.Draw(rw);

		//cout << p.getKillCount() << endl;
		//IF WIN: Level++
		if (p.getKillCount() >= lvl[2].minionSum)
		{
			Level=1;
			score.updateScore(2000);
			p.manualWalkSoundStop();
			p.resetStat();
			tankVector.clear();
			vampireVector.clear();
			casterVector.clear();
			gameState = 1003;//Pre win
		}

		//IF LOSE: GO TO ENTER SCORE SCREEN
		if (p.isDead(rw, deltaTime))
		{
			//cout << "You died" << endl;
			win = false;
			gameState = 1004;//Score menu
		}
		}


		else if (gameState==1003)//Pre win end game
		{
		rw.draw(highscoreBackground);
		levelName.setString("    You Win!");
		rw.draw(levelBackFrame);
			//cout << "win state"<< endl;
			rw.draw(levelName);
			rw.draw(pressQ);
			if (e.type==Event::KeyReleased)
			{
				if (e.key.code==Keyboard::Q)
				{
					win = true;
					gameState = 99;
				}
			}
		}
		else if (gameState == 1004)//Pre lose end game
		{
		rw.draw(lostBG);
		levelName.setString("    You Lose!");
		rw.draw(levelBackFrame);
		//cout << "win state"<< endl;
		rw.draw(levelName);
		rw.draw(pressQ);
		if (e.type == Event::KeyReleased)
		{
			if (e.key.code == Keyboard::Q)
			{
				win = false;
				gameState = 99;
			}
		}
 }
		else if (gameState == 99)//End game state: Name+Score (NAME CAN'T EXCEED 10 LETTERS (field limit size))
		{
		for (int i=0;i<3;i++)
		{
			slvl[i].caster = 0;
			slvl[i].tank = 0;
			slvl[i].vampire = 0;
		}
		if (win)
		{
			rw.draw(highscoreBackground);
			rw.draw(winText);
		}
		else
		{
			rw.draw(lostBG);
			rw.draw(loseText);
		}
		rw.draw(ask4name);
			rw.draw(inputField);
			if (blinkClock.getElapsedTime().asSeconds()>=0.5f)
			{
				blinkClock.restart();
				blinkState = !blinkState;
			}
			if (blinkState)
			{
				bar.setPosition(playerNameText.getGlobalBounds().width+5-140 + RWWIDTH / 2, RWHEIGHT / 2);
				rw.draw(bar);
			}


			if (e.type==Event::TextEntered)//Enter letter
			{
				if (lastChar != e.text.unicode && playerName.length()<10 &&((e.text.unicode>='A' && e.text.unicode<='Z') || (e.text.unicode>='a' && e.text.unicode<='z') || (e.text.unicode>='0' && e.text.unicode<='9')))
				{
					lastChar = e.text.unicode;
					playerName += lastChar;
					playerNameText.setString(playerName);
				}
				else if (lastChar != e.text.unicode && e.text.unicode == 8 && playerName.length() > 0)//Delete letter
				{
					lastChar = e.text.unicode;
					playerName.erase(playerName.length() - 1);
					playerNameText.setString(playerName);
				}
			}
			if (e.type==Event::KeyReleased && lastChar!=' ')
			{
				lastChar = ' ';
			}
			rw.draw(playerNameText);
			score.Draw(rw);
			if (e.type==Event::KeyReleased)
			{
				if (e.key.code == Keyboard::Return && playerName.length()>0)
				{
					pairScoreName.push_back(make_pair(score.GetScore(),playerName));
					sort(pairScoreName.begin(),pairScoreName.end(),sortinrev);
					/*
					//cout << "--------------sorted vector-------" << endl;
					for (size_t i = 0; i < 5; i++)//only write top 5 score
					{
						//cout << pairScoreName[i].first << "," << pairScoreName[i].second << "\n";
					}
					*/
					ofstream highscoreWriter;
					highscoreWriter.open("HighscoreFile.txt");
					for (size_t i = 0; i < 5; i++)//only write top 5 score
					{
						highscoreWriter << pairScoreName[i].first << "," << pairScoreName[i].second << "\n";
					}
					highscoreWriter.close();

					highscoreReader.open("HighscoreFile.txt");
					pairScoreName.clear();
					do
					{
						highscoreReader >> line;
						string highscoreScore = line.substr(0, line.find(','));
						string highscoreName = line.substr(line.find(',') + 1, line.length());
						pairScoreName.push_back(make_pair(atoi(highscoreScore.c_str()), highscoreName));
					} while (highscoreReader.good());//Read until EOF
					highscoreReader.close();
					for (size_t i = 0; i < 5; i++)
					{
						//cout << pairScoreName[i].first << "     " << pairScoreName[i].second << endl;
						highscoreNameText[i].setString(pairScoreName[i].second);
						highscoreScoreText[i].setString(to_string(pairScoreName[i].first));
					}
					playerName = "";
					playerNameText.setString("");
					score.resetStat();
					gameState = 0;
				}
			}
		}

		rw.display();
	}
	return 0;
}


bool sortinrev(const pair<int, string>& a, const pair<int, string>& b) //Sort in descending order consider first element
{
	return (a.first > b.first);
}