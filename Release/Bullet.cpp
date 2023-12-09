#include "Bullet.h"

Bullet::~Bullet() {}

void Bullet::setBulletTexture(Texture *tex) { this->shape.setTexture(tex); }

Bullet::Bullet(float wid, float hei)
    : currVelocity(0.0f, 0.0f), maxSpeed(15.0f) {
  this->shape.setSize(Vector2f(wid, hei));
  // this->shape.setTexture(&tex);
}
