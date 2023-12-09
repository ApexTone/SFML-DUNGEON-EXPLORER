#pragma once
#include "Collider.h"
#include <SFML/Graphics.hpp>
using namespace sf;
class Bullet {
public:
  RectangleShape shape;
  Vector2f currVelocity;
  float maxSpeed;

  Bullet(float wid = 15.0f, float hei = 15.0f);
  ~Bullet();
  void setBulletTexture(Texture *tex);
  Collider getCollider() { return Collider(shape); }
};
