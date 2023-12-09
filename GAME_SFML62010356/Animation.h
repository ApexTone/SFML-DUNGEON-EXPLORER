#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
using namespace sf;
using namespace std;

class Animation {
public:
  Animation(Texture *texture, Vector2u imageCount, float switchTime);
  ~Animation();
  void Update(int row, float deltaTime, bool faceRight);
  bool playRow(int row, float deltaTime,
               bool faceRight); // Return false if the row end
  IntRect uvRect;               // image usage area

private:
  Vector2u imageCount;   // image in format of row X column
  Vector2u currentImage; // current image position
  Vector2u currentRowImage;

  float totalTime;  // Kinda like millis() in arduino
  float switchTime; // Time per frame
};
