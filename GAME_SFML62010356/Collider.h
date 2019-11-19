#pragma once
#include<SFML/Graphics.hpp>
using namespace sf;
class Collider
{
public:
	Collider(RectangleShape& body);
	~Collider();

	void Move(float dx, float dy);

	bool CheckCollision(Collider other, float push);
	bool CheckIntersect(Collider other);
	Vector2f GetPosition();
	Vector2f GetHalfSize();
	void drawCollider(RenderWindow& rw);

private:
	RectangleShape& body;
};