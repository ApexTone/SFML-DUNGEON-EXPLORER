#pragma once
#include "Collider.h"
#include <SFML/Graphics.hpp>
using namespace sf;

class Platform {
public:
  Platform(Texture *texture, Vector2f size, Vector2f position);
  ~Platform();

  void Draw(RenderWindow &rw);
  Collider GetCollider() { return Collider(body); }

private:
  RectangleShape body;
};
