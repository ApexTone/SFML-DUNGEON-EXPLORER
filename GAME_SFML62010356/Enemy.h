#pragma once
#include "Animation.h"
#include "Bullet.h"
#include "Collider.h"
#include "Player.h"
#include "Randomizer.h"
#include <Math.h>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <Vector>
#include <iostream>
using namespace sf;
using namespace std;
class Tank {
public:
  Tank(Texture *texture, Vector2u imageCount, float switchTime, float speed);
  ~Tank();

  Collider GetCollider() { return Collider(hitbox); }
  void Update(float deltaTime, RenderWindow &rw, Player &p);
  void Draw(RenderWindow &rw);
  void resetStat();

  void takeDamage(int damage);
  int meleeDamage() { return 5; }
  unsigned int killScore() { return 300; }
  bool isDead(RenderWindow &rw, float deltaTime);

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
  int health = 150;

  Clock iFrame;
};

class Vampire {
public:
  Vampire(Texture *texture, Vector2u imageCount, float switchTime, float speed);
  ~Vampire();

  Collider GetCollider() { return Collider(hitbox); }
  void Update(float deltaTime, RenderWindow &rw, Player &p);
  void Draw(RenderWindow &rw);
  void resetStat();

  void takeDamage(int damage);
  int meleeDamage() { return 10; }
  unsigned int killScore() { return 500; }
  bool isDead(RenderWindow &rw, float deltaTime);

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

  Clock iFrame;
};

class Caster {
public:
  Caster(Texture *texture, Vector2u imageCount, float switchTime, float speed);
  ~Caster();

  // TODO:EDIT UPDATE FUNCTION
  Collider GetCollider() { return Collider(hitbox); }
  void Update(float deltaTime, RenderWindow &rw, Player &p);
  void Draw(RenderWindow &rw, float deltaTime);
  void resetStat();

  bool bulletCollision(Collider other, int number, float force);
  unsigned int bulletVectorSize() { return bulletVector.size(); }
  void setBulletTexture(Texture *tex) { b.setBulletTexture(tex); }
  void eraseBullet(int i) { bulletVector.erase(bulletVector.begin() + i); }
  void clearBulletVector() { bulletVector.clear(); }
  void setBulletPosition(int i, float a = 999999, float b = 999999) {
    bulletVector[i].shape.setPosition(Vector2f(a, b));
  }
  Vector2f getBulletPosition(int i) {
    return bulletVector[i].shape.getPosition();
  }

  void takeDamage(int damage);
  int bulletDamage() { return 7; }
  unsigned int killScore() { return 700; }
  bool isDead(RenderWindow &rw, float deltaTime);

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
  int fireRateControl;

  Clock iFrame;
};
