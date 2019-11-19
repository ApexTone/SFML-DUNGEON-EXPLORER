#pragma once
#include<Vector>
#include<SFML/Graphics.hpp>
#include<SFML/Audio.hpp>
#include<iostream>
#include"Collider.h"
#include"Animation.h"
#include"Randomizer.h"
#include"Player.h"
#include"Bullet.h"
#include<Math.h>
using namespace sf;
using namespace std;
class Tank
{
public:
	Tank(Texture* texture, Vector2u imageCount, float switchTime, float speed);
	~Tank();

	Collider GetCollider() { return Collider(hitbox); }
	void Update(float deltaTime, RenderWindow& rw,Player &p);
	void Draw(RenderWindow& rw);
	void resetStat();

	void takeDamage(int damage) { health -= damage; }
	int meleeDamage() { return 5; }
	unsigned int killScore() { return 200; }
	bool isDead(RenderWindow& rw,float deltaTime);
private:
	Randomizer random;
	RectangleShape body,hitbox;
	Animation animation;
	unsigned int row;
	float speed;
	bool faceRight;
	Music walk;
	bool isWalking = false;
	int randomNumber;
	int timeToRandom = 0;


	bool action = false;
	int health = 200;
};

class Vampire
{
public:
	Vampire(Texture* texture, Vector2u imageCount, float switchTime, float speed);
	~Vampire();

	Collider GetCollider() { return Collider(hitbox); }
	void Update(float deltaTime, RenderWindow& rw, Player& p);
	void Draw(RenderWindow& rw);
	void resetStat();

	void takeDamage(int damage) { health -= damage; }
	int meleeDamage() { return 10; }
	unsigned int killScore() { return 300; }
	bool isDead(RenderWindow& rw, float deltaTime);
private:
	Randomizer random;
	RectangleShape body, hitbox;
	Animation animation;
	unsigned int row;
	float speed;
	bool faceRight;
	Music walk;
	bool isWalking = false;
	int randomNumber;
	int timeToRandom = 0;


	bool action = false;
	int health = 60;
};

class Caster
{
public:
	Caster(Texture* texture, Vector2u imageCount, float switchTime, float speed);
	~Caster();

	//TODO:EDIT UPDATE FUNCTION
	Collider GetCollider() { return Collider(hitbox); }
	void Update(float deltaTime, RenderWindow& rw, Player& p);
	void Draw(RenderWindow& rw);
	void resetStat();

	bool bulletCollision(Collider other, int number, float force);
	unsigned int bulletVectorSize() { return bulletVector.size(); }
	void setBulletTexture(Texture* tex) { b.setBulletTexture(tex); }
	void eraseBullet(int i) { bulletVector.erase(bulletVector.begin()+i); }
	void clearBulletVector() { bulletVector.clear(); }
	void setBulletPosition(int i, float a = 999999, float b = 999999) { bulletVector[i].shape.setPosition(Vector2f(a, b)); }


	void takeDamage(int damage) { health -= damage; }
	int bulletDamage() { return 20; }
	unsigned int killScore() { return 400; }
	bool isDead(RenderWindow& rw, float deltaTime);
private:
	Randomizer random;
	RectangleShape body, hitbox;
	Animation animation;
	unsigned int row;
	float speed;
	bool faceRight;
	Music walk;
	bool isWalking = false;
	int randomNumber;
	int timeToRandom = 0;


	bool action = false;
	int health = 50;

	Bullet b;
	vector<Bullet> bulletVector;
};

