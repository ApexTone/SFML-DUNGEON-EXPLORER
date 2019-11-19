#include "Collider.h"

Collider::Collider(RectangleShape& body) :
	body(body)
{

}

Collider::~Collider()
{
}

void Collider::Move(float dx, float dy)
{
	body.move(dx,dy);
}

bool Collider::CheckCollision(Collider other, float push)//Utilize AABB collision detection
{
	Vector2f otherPosition = other.GetPosition();
	Vector2f otherHalfSize = other.GetHalfSize();
	Vector2f thisPosition = GetPosition();
	Vector2f thisHalfSize = GetHalfSize();

	float deltax = otherPosition.x - thisPosition.x;
	float deltay = otherPosition.y - thisPosition.y;

	float intersectx = abs(deltax) - (otherHalfSize.x + thisHalfSize.x);
	float intersecty = abs(deltay) - (otherHalfSize.y + thisHalfSize.y);
	
	if (intersectx < 0.0f && intersecty < 0.0f)//If collide
	{
		push = std::min(std::max(push,0.0f),1.0f);//prevent out of bound pushing force
		
		if (abs(intersectx) < abs(intersecty))//Choos to move the axis that intersect the most
		{
			if (deltax > 0.0f)
			{
				Move(intersectx * (1.0f - push), 0.0f);//Push back to cancel the collision effect
				other.Move(-intersectx*push,0.0f);//push in other direction to cancel the collision effect
			}
			else
			{
				Move(-intersectx * (1.0f - push), 0.0f);//Push back to cancel the collision effect
				other.Move(intersectx * push, 0.0f);//push in other direction to cancel the collision effect
			}
		}
		else
		{
			if (deltay > 0.0f)
			{
				Move(0.0f,intersecty* (1.0f - push));
				other.Move(0.0f,-intersecty * push);
			}
			else
			{
				Move(0.0f,-intersecty * (1.0f - push));
				other.Move(0.0f,intersecty * push);
			}
		}

		return true;
	}
	return false;
}

bool Collider::CheckIntersect(Collider other)
{
	Vector2f otherPosition = other.GetPosition();
	Vector2f otherHalfSize = other.GetHalfSize();
	Vector2f thisPosition = GetPosition();
	Vector2f thisHalfSize = GetHalfSize();

	float deltax = otherPosition.x - thisPosition.x;
	float deltay = otherPosition.y - thisPosition.y;

	float intersectx = abs(deltax) - (otherHalfSize.x + thisHalfSize.x);
	float intersecty = abs(deltay) - (otherHalfSize.y + thisHalfSize.y);

	if (intersectx < 0.0f && intersecty < 0.0f)//If collide
	{
		return true;
	}
	return false;
}


Vector2f Collider::GetPosition()
{
	return body.getPosition();
}

Vector2f Collider::GetHalfSize()
{
	return body.getSize()/2.0f;
}

void Collider::drawCollider(RenderWindow& rw)
{
	body.setFillColor(Color::Green);
	rw.draw(body);
}

