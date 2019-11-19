#include<SFML/Graphics.hpp>
#include<SFML/Audio.hpp>
#include<iostream>
#include<fstream>
#include"Animation.h"
#include"Player.h"
#include"Platform.h"
#include"Menu.h"
#include"Enemy.h"
#include"Score.h"

#define FRAMERATE 40 //DON'T CHANGE THIS VALUE AT ALL!
#define RWWIDTH 1920
#define RWHEIGHT 1080
using namespace std;
using namespace sf;
struct LevelControl
{
	unsigned int tankNO, casterNO, vampireNO;
	unsigned int minionSum;
}lvl[3];

//enum state {MENU=0,PAUSE,HIGHSCORE,HOWTOPLAY,LEVEL1,LEVEL2,LEVEL3};

int main()
{
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
	
	//Platform plt(nullptr, Vector2f(400.0f, 200.0f), Vector2f(500.0f, 200.0f));

	Music backgroundMusic;
	if (!backgroundMusic.openFromFile("Moonlight.wav"))
	{
		cout << "Can't open music" << endl;
	}
	backgroundMusic.setLoop(true);
	backgroundMusic.play();
	backgroundMusic.setVolume(30);

	float deltaTime = 0.0f;
	Clock clk;

	Menu menu(RWWIDTH,RWHEIGHT);
	HowToPlay htp(RWWIDTH,RWHEIGHT);
	Pause pause(rw);
	int gameState = 0,Level=1;


	Texture bulletTex;
	bulletTex.loadFromFile("Bullet.png");
	p.setBulletTexture(&bulletTex);


	Clock iFrame;
	float enemyMeleeDelay = 1.5f;


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


	map<unsigned int, string> highscoreMap;
	ifstream highscoreReader;
	highscoreReader.open("HighscoreFile.txt");
	Text highscoreNameText[5],highscoreScoreText[5];
	for (int i=0;i<5;i++)
	{
		highscoreNameText[i].setFont(font);
		highscoreNameText[i].setFillColor(Color::Yellow);
		highscoreNameText[i].setCharacterSize(50);
		highscoreNameText[i].setPosition(Vector2f((float)RWWIDTH/4,(float)RWHEIGHT/6*(i+1)));

		highscoreScoreText[i].setFont(font);
		highscoreScoreText[i].setFillColor(Color::Yellow);
		highscoreScoreText[i].setCharacterSize(50);
		highscoreScoreText[i].setPosition(Vector2f((float)3*RWWIDTH / 4, (float)RWHEIGHT / 6 * (i + 1)));
	}
	string line;
	do
	{
		highscoreReader >> line;
		string highscoreName = line.substr(0, line.find(','));
		string highscoreScore = line.substr(line.find(',')+1,line.length());
		highscoreMap[stoi(highscoreScore)] = highscoreName;
		cout << highscoreMap[stoi(highscoreScore)] << "\t" <<highscoreScore << endl;
	} while (highscoreReader.good());//Read until EOF

	map<unsigned int, string>::iterator iterMap=highscoreMap.begin();
	for (iterMap = highscoreMap.begin(); iterMap != highscoreMap.end(); iterMap++) //Assign value
	{
		static int i = 4;
		highscoreScoreText[i].setString(to_string(iterMap->first));
		highscoreNameText[i--].setString(iterMap->second);
		if (i==0)
		{
			break;
		}
	}
	highscoreReader.close();
	//The above highscore part must call every time


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
				FloatRect visibleArea(0,0,(float)e.size.width,(float)e.size.height);
				rw.setView(View(visibleArea));
			}
			else if (e.type==Event::KeyReleased && gameState==0)
			{
				switch (e.key.code)
				{
				case Keyboard::W: menu.MoveUp(); break;
				case Keyboard::S: menu.MoveDown(); break;
				case Keyboard::Return:
					switch (menu.getPressedItem())
					{
					case 0: //cout << "Start game" << endl; 
						gameState = 1;//game start
						break;
					case 1:  //cout << "How to play menu" << endl; 
						gameState = -1;//How to play
						break;
					case 2: //cout << "Highscore menu" << endl; 
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
		//Spawning test
		if (e.type == Event::KeyReleased)
		{
			if (e.key.code == Keyboard::O)
			{
				tankVector.push_back(new Tank(&tankTexture, Vector2u(4, 4), 0.1f, 60.0f));
			}
			if (e.key.code == Keyboard::U)
			{
				vampireVector.push_back(new Vampire(&vampireTexture, Vector2u(4, 4), 0.08f, 90.0f));
			}
			if (e.key.code == Keyboard::I)
			{
				casterVector.push_back(new Caster(&casterTexture, Vector2u(4, 4), 0.1f, 80.0f));
			}
		}

		if (gameState == 0)//Menu state
		{
			menu.Draw(rw);
			p.resetStat();
			vampireVector.clear();
			tankVector.clear();
			casterVector.clear();
		}
		else if (gameState==-1)//How to play TODO:DECORATE
		{
			htp.Draw(rw);
			if (e.type == Event::KeyReleased)
			{
				if (e.key.code == Keyboard::Escape)
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
		else if (gameState==-3)//Highscore
		{
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
			rw.draw(bg);


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
					//cout << "Taking damage from TANK" << endl;
					p.takeDamage(tankVector[i]->meleeDamage());
				}
			}
			for (size_t i = 0; i < vampireVector.size(); i++)//Vampire-Player Punch Intersect
			{
				if (vampireVector[i]->GetCollider().CheckIntersect(p.GetCollider()))
				{
					//cout << "Taking damage from VAMPIRE" << endl;
					p.takeDamage(vampireVector[i]->meleeDamage());
				}
			}
			//TODO:Caster-Player Bullet collision


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
			for (size_t i=0;i<tankVector.size();i++)
			{
				if (tankVector[i]->isDead(rw,deltaTime))
				{
					score.updateScore(tankVector[i]->killScore());
					p.increaseKillCount();
					//cout << "TANK " << i << " HAS BEEN KILLED!" << endl;
					if (tankVector.size()>1)
					{
						Tank* tankPointer;
						tankPointer = tankVector.at(i);
						delete tankPointer;
						tankVector.erase(tankVector.begin()+i);
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
				if (vampireVector[i]->isDead(rw,deltaTime))
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
			for (size_t i=0;i<p.bulletVectorSize();i++)
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
				casterVector[i]->Draw(rw);
				casterVector[i]->Update(deltaTime, rw, p);
			}
			p.Update(deltaTime, rw);
			p.Draw(rw);
			score.Draw(rw);

			//IF WIN: Level++
			if (p.getKillCount()>20)
			{
				//cout << "Win" << endl;
				gameState = 99;//Next level
			}
			
			//IF LOSE: GO TO ENTER SCORE SCREEN
			if (p.isDead(rw,deltaTime))
			{
				//change gameState
				//cout << "You died" << endl;
				gameState = 99;//Score menu
			}
		}


		else if (gameState == 99)//End game state: Name+Score (NAME CAN'T EXCEED 10 LETTERS (field limit size)) TODO:DECORATE
		{
			cout << "End game" << endl;
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
					//cout << playerName << endl;
					playerNameText.setString(playerName);
				}
				else if (lastChar != e.text.unicode && e.text.unicode == 8 && playerName.length() > 0)//Delete letter
				{
					lastChar = e.text.unicode;
					playerName.erase(playerName.length() - 1);
					//cout << playerName << endl;
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
				if (e.key.code == Keyboard::Return)
				{
					ofstream highscoreWriter;
					highscoreWriter.open("HighscoreFile.txt",ios::out | ios::app);
					highscoreWriter << playerName << "," << score.GetScore() << '\n';
					highscoreWriter.close();

					highscoreReader.open("HighscoreFile.txt");
					do
					{
						highscoreReader >> line;
						string highscoreName = line.substr(0, line.find(','));
						string highscoreScore = line.substr(line.find(',') + 1, line.length());
						highscoreMap[stoi(highscoreScore)] = highscoreName;
						//cout << highscoreMap[stoi(highscoreScore)] << "\t" << highscoreScore << endl;
					} while (highscoreReader.good());//Read until EOF

					int i = 4;
					for (iterMap = highscoreMap.begin(); iterMap != highscoreMap.end();iterMap++) //Assign value
					{
						highscoreScoreText[i].setString(to_string(iterMap->first));
						highscoreNameText[i--].setString(iterMap->second);
						if (i == 0)
						{
							break;
						}
					}
					highscoreReader.close();
					score.resetStat();
					gameState = 0;
				}
			}
		}
		rw.display();
	}
	return 0;
}