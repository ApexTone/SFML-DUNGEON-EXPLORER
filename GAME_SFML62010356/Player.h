#pragma once
#include<SFML/Graphics.hpp>
#include<SFML/Audio.hpp>
#include"Animation.h"
#include"Collider.h"
#include<vector>
#include"Bullet.h"
using namespace sf;
using namespace std;
class Player
{
public:
	Player(Texture* texture, Vector2u imageCount, float switchTime, float speed);
	~Player();

	Collider GetCollider();
	Collider GetPunchCollider();
	Vector2f GetPosition();

	void resetStat();//Call every level and at Menu
	void Update(float deltaTime, RenderWindow& rw);
	void Draw(RenderWindow& rw);//draw just like normal draw() but specific for this class

	bool bulletCollision(Collider other, int number, float force);
	unsigned int bulletVectorSize() { return bullets.size(); }
	void setBulletTexture(Texture* tex);
	void eraseBullet(int i);
	void clearBulletVector() { bullets.clear(); }
	void setBulletPosition(int i, float a = 999999, float b = 999999);

	void takeDamage(int damage) { health -= damage; }
	int bulletDamage() { return bulletDam; }
	int meleeDamage() { return meleeDam; }
	bool punching() { return isPunching; }

	void increaseKillCount() { killCount++; }
	unsigned int getKillCount() { return killCount; }

	Vector2f getBulletPosition(int i);
	Vector2f getSkinPosition() { return body.getPosition(); }

	bool isDead(RenderWindow& rw,float deltaTime);
	void manualWalkSoundStop() { walk.pause(); }

	void healthBuff() { healthBar.setFillColor(Color::Blue); health = 200; maxHealth = 200; }
	void damageBuff() { bulletDam = 40; meleeDam = 50; body.setFillColor(Color::Red); }
private:
	RectangleShape hitbox;//Collision part
	RectangleShape punchHitbox;

	RectangleShape body;//Apply skin
	Animation animation;
	unsigned int row;//row always >=0
	float speed;//tell how fast the object move
	bool faceRight;//Check if facing right in order to render the correct side of image

	int fireRateControl=0,meleeRateControl=0;
	
	Music walk;
	bool isWalking=false;
	SoundBuffer gunShotBuffer,punchBuffer;
	Sound gunShot,punchSound;

	Bullet b;
	vector<Bullet> bullets;
	bool mouseState = false;
	bool action = false;
	bool isPunching = false;
	int bulletDam = 20,meleeDam=25;


	int health=100;
	RectangleShape healthBar;
	float iFrameTime = 1.5f,currentTime=0;
	unsigned int killCount = 0;

	float maxHealth = 100;
};
