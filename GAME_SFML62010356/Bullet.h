#pragma once
#include<SFML/Graphics.hpp>
#include"Collider.h"
using namespace sf;
class Bullet
{
public:
	RectangleShape shape;
	Vector2f currVelocity;
	float maxSpeed;

	Bullet(float wid=10.0f,float hei=10.0f);
	~Bullet();
	void setBulletTexture(Texture *tex);
	Collider getCollider() { return Collider(shape); }
};

