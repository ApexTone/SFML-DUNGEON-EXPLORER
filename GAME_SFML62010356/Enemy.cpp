#include "Enemy.h"

Tank::Tank(Texture *texture, Vector2u imageCount, float switchTime, float speed)
    : animation(texture, imageCount, switchTime) {
  srand((unsigned int)time(NULL));
  this->speed = speed;
  row = 0;
  faceRight = true;
  randomNumber = 0;
  const float xSize = 120, ySize = 120;

  body.setSize(
      Vector2f(xSize, ySize)); // set size of the minion (VARY FOR EACH MINION)
  body.setPosition((float)(rand() % 1800), (float)(rand() % 900));
  body.setOrigin(Vector2f(xSize / 2, ySize / 2));
  body.setTexture(texture);

  /*
  if (!walk.openFromFile("walk.wav"))//Set Tank Sound fx
  {
          cout << "Can't open walk file" << endl;
  }
  walk.setLoop(true);
  */

  // Fitting the collider size
  hitbox.setSize(Vector2f(body.getSize().x - 20, body.getSize().y - 15));
  hitbox.setPosition(body.getPosition());
  hitbox.setOrigin(hitbox.getSize().x / 2, hitbox.getSize().y / 2);
  hitbox.setFillColor(Color::Blue);
}

Tank::~Tank() {}

void Tank::Update(float deltaTime, RenderWindow &rw, Player &p) {
  float dist = sqrt(pow(body.getPosition().x - p.getSkinPosition().x, 2) +
                    pow(body.getPosition().y - p.getSkinPosition().y, 2));

  Vector2f movement(0.0f, 0.0f);

  // Prevent out of bound
  if (hitbox.getPosition().x - hitbox.getSize().x / 2 < 0) {
    // cout << "OOB Left" << endl;
    hitbox.setPosition(
        Vector2f(hitbox.getSize().x / 2, hitbox.getPosition().y));
  }
  if (hitbox.getPosition().x + hitbox.getSize().x / 2 > rw.getSize().x) {
    // cout << "OOB Right" << endl;
    hitbox.setPosition(
        Vector2f(25 + hitbox.getPosition().x - hitbox.getSize().x / 2,
                 hitbox.getPosition().y));
  }
  if (hitbox.getPosition().y - hitbox.getSize().y / 2 < 0) {
    // cout << "OOB Top" << endl;
    hitbox.setPosition(
        Vector2f(hitbox.getPosition().x, hitbox.getSize().y / 2));
  }
  if (hitbox.getPosition().y + hitbox.getSize().y > rw.getSize().y) {
    // cout << "OOB Bottom" << endl;
    hitbox.setPosition(
        Vector2f(hitbox.getPosition().x,
                 40 + hitbox.getPosition().y - hitbox.getSize().y / 2));
  }

  if (++timeToRandom > 20) {
    randomNumber = 0 + (rand() % 6); // Randomizer don't work!
    timeToRandom = 0;
  }

  if (dist >= 900) // Out of tracking range
  {
    if (randomNumber == 0) {
      movement.x -= speed * deltaTime;
    } else if (randomNumber == 1) {
      movement.x += speed * deltaTime;
    } else if (randomNumber == 2) {
      movement.y -= speed * deltaTime;
    } else if (randomNumber == 3) {
      movement.y += speed * deltaTime;
    } else {
      // No movement
    }
  } else // Track player within range
  {
    float distX = fabs(body.getPosition().x - p.getSkinPosition().x),
          distY = fabs(body.getPosition().y - p.getSkinPosition().y);
    if (distX >= 3) {
      if (body.getPosition().x < p.getSkinPosition().x) {
        movement.x += speed * deltaTime;
      }
      if (body.getPosition().x > p.getSkinPosition().x) {
        movement.x -= speed * deltaTime;
      }
    }
    if (distY >= 3) {
      if (body.getPosition().y > p.getSkinPosition().y) {
        movement.y -= speed * deltaTime;
      }
      if (body.getPosition().y < p.getSkinPosition().y) {
        movement.y += speed * deltaTime;
      }
    }

    if (GetCollider().CheckIntersect(p.GetCollider()) &&
        abs(body.getPosition().y - p.GetPosition().y) < 20) {
      row = 3; // attack frame
      movement = Vector2f(0, 0);
      action = true;
    } else {
      action = false;
    }
  }

  if (movement.x == 0.0f && movement.y == 0.0f && !action) // When idle
  {
    row = 0;
    /*if (isWalking)
    {
            walk.pause();
            isWalking = false;
    }*/
  } else // When moving
  {
    if (!action) {
      row = 1;
    }
    /*if (!isWalking)
    {
            walk.play();
            walk.setVolume(70);
            isWalking = true;
    }*/

    if (movement.x > 0.0f) {
      faceRight = true;
    } else if (movement.x < 0.0f) {
      faceRight = false;
    }
  }

  if (health > 0) {
    animation.Update(row, deltaTime, faceRight); // Update the animation
    body.setTextureRect(animation.uvRect);
    hitbox.move(movement); // move hitbox and apply body texture to it
    body.setPosition(hitbox.getPosition());
  }
}

void Tank::Draw(RenderWindow &rw) {
  // rw.draw(hitbox);
  rw.draw(body);
}

void Tank::resetStat() {
  row = 0;
  faceRight = true;
  randomNumber = 0;
  body.setPosition((float)(rand() % 1800), (float)(rand() % 900));
  hitbox.setPosition(body.getPosition());
  health = 200;
}

void Tank::takeDamage(int damage) {
  const float iFrameLength = 0.1f;
  if (iFrame.getElapsedTime().asSeconds() >= iFrameLength) {
    health -= damage;
    iFrame.restart();
  }
}

bool Tank::isDead(RenderWindow &rw,
                  float deltaTime) // MUST USE DELTA TIME AND ONLY PLAY ONCE
{
  if (health <= 0) {
    const int row = 2;
    while (animation.playRow(row, deltaTime, faceRight)) {
      body.setTextureRect(animation.uvRect);
      rw.draw(body);
    }
    hitbox.setPosition(999999.0f, 999999.0f);
    return true;
  }
  return false;
}

Vampire::Vampire(Texture *texture, Vector2u imageCount, float switchTime,
                 float speed)
    : animation(texture, imageCount, switchTime) {
  srand((unsigned int)time(NULL));
  this->speed = speed;
  row = 0;
  faceRight = true;
  randomNumber = 0;
  const float xSize = 100, ySize = 100;

  body.setSize(
      Vector2f(xSize, ySize)); // set size of the minion (VARY FOR EACH MINION)
  body.setPosition((float)(rand() % 1800), (float)(rand() % 900));
  body.setOrigin(Vector2f(xSize / 2, ySize / 2));
  body.setTexture(texture);

  /*
  if (!walk.openFromFile("walk.wav"))//Set Tank Sound fx
  {
          cout << "Can't open walk file" << endl;
  }
  walk.setLoop(true);
  */

  // Fitting the collider size
  hitbox.setSize(Vector2f(body.getSize().x - 20, body.getSize().y));
  hitbox.setPosition(body.getPosition());
  hitbox.setOrigin(hitbox.getSize().x / 2, hitbox.getSize().y / 2);
  hitbox.setFillColor(Color::Blue);
}

Vampire::~Vampire() {}

void Vampire::Update(float deltaTime, RenderWindow &rw, Player &p) {
  float dist = sqrt(pow(body.getPosition().x - p.getSkinPosition().x, 2) +
                    pow(body.getPosition().y - p.getSkinPosition().y, 2));

  Vector2f movement(0.0f, 0.0f);

  // Prevent out of bound
  if (hitbox.getPosition().x - hitbox.getSize().x / 2 < 0) {
    // cout << "OOB Left" << endl;
    hitbox.setPosition(
        Vector2f(hitbox.getSize().x / 2, hitbox.getPosition().y));
  }
  if (hitbox.getPosition().x + hitbox.getSize().x / 2 > rw.getSize().x) {
    // cout << "OOB Right" << endl;
    hitbox.setPosition(
        Vector2f(25 + hitbox.getPosition().x - hitbox.getSize().x / 2,
                 hitbox.getPosition().y));
  }
  if (hitbox.getPosition().y - hitbox.getSize().y / 2 < 0) {
    // cout << "OOB Top" << endl;
    hitbox.setPosition(
        Vector2f(hitbox.getPosition().x, hitbox.getSize().y / 2));
  }
  if (hitbox.getPosition().y + hitbox.getSize().y > rw.getSize().y) {
    // cout << "OOB Bottom" << endl;
    hitbox.setPosition(
        Vector2f(hitbox.getPosition().x,
                 40 + hitbox.getPosition().y - hitbox.getSize().y / 2));
  }

  if (++timeToRandom > 20) {
    randomNumber = 0 + (rand() % 6); // Randomizer don't work!
    timeToRandom = 0;
  }

  if (dist >= 700) // Out of tracking range
  {
    if (randomNumber == 0) {
      movement.x -= speed * deltaTime;
    } else if (randomNumber == 1) {
      movement.x += speed * deltaTime;
    } else if (randomNumber == 2) {
      movement.y -= speed * deltaTime;
    } else if (randomNumber == 3) {
      movement.y += speed * deltaTime;
    } else {
      // No movement
    }
  } else // Track player within range
  {
    float distX = fabs(body.getPosition().x - p.getSkinPosition().x),
          distY = fabs(body.getPosition().y - p.getSkinPosition().y);
    if (distX >= 3) {
      if (body.getPosition().x < p.getSkinPosition().x) {
        movement.x += speed * deltaTime;
      }
      if (body.getPosition().x > p.getSkinPosition().x) {
        movement.x -= speed * deltaTime;
      }
    }
    if (distY >= 3) {
      if (body.getPosition().y > p.getSkinPosition().y) {
        movement.y -= speed * deltaTime;
      }
      if (body.getPosition().y < p.getSkinPosition().y) {
        movement.y += speed * deltaTime;
      }
    }

    if (GetCollider().CheckIntersect(p.GetCollider()) &&
        abs(body.getPosition().y - p.GetPosition().y) < 20) {
      row = 3; // attack frame
      movement = Vector2f(0, 0);
      action = true;
    } else {
      action = false;
    }
  }

  if (movement.x == 0.0f && movement.y == 0.0f && !action) // When idle
  {
    row = 0;
    /*if (isWalking)
    {
            walk.pause();
            isWalking = false;
    }*/
  } else // When moving
  {
    if (!action) {
      row = 1;
    }
    /*if (!isWalking)
    {
            walk.play();
            walk.setVolume(70);
            isWalking = true;
    }*/

    if (movement.x > 0.0f) {
      faceRight = true;
    } else if (movement.x < 0.0f) {
      faceRight = false;
    }
  }

  if (health > 0) {
    animation.Update(row, deltaTime, faceRight); // Update the animation
    body.setTextureRect(animation.uvRect);
    hitbox.move(movement); // move hitbox and apply body texture to it
    body.setPosition(hitbox.getPosition());
  }
}

void Vampire::Draw(RenderWindow &rw) {
  // rw.draw(hitbox);
  rw.draw(body);
}

void Vampire::resetStat() {
  row = 0;
  faceRight = true;
  randomNumber = 0;
  body.setPosition((float)(rand() % 1800), (float)(rand() % 900));
  hitbox.setPosition(body.getPosition());
  health = 60;
}

void Vampire::takeDamage(int damage) {
  const float iFrameLength = 0.1f;
  if (iFrame.getElapsedTime().asSeconds() >= iFrameLength) {
    health -= damage;
    iFrame.restart();
  }
}

bool Vampire::isDead(RenderWindow &rw, float deltaTime) {
  if (health <= 0) {
    const int row = 2;
    while (animation.playRow(row, deltaTime, faceRight)) {
      body.setTextureRect(animation.uvRect);
      rw.draw(body);
    }
    hitbox.setPosition(999999.0f, 999999.0f);
    return true;
  }
  return false;
}

Caster::Caster(Texture *texture, Vector2u imageCount, float switchTime,
               float speed)
    : animation(texture, imageCount, switchTime) {
  fireRateControl = 0;
  srand((unsigned int)time(NULL));
  this->speed = speed;
  row = 0;
  faceRight = true;
  randomNumber = 0;
  const float xSize = 120, ySize = 120;

  body.setSize(
      Vector2f(xSize, ySize)); // set size of the minion (VARY FOR EACH MINION)
  body.setPosition((float)(rand() % 1800),
                   (float)(rand() % 900)); // TODO: RANDOM STARTING POSITION
  body.setOrigin(Vector2f(xSize / 2, ySize / 2));
  body.setTexture(texture);

  /*
  if (!walk.openFromFile("walk.wav"))//Set Tank Sound fx
  {
          cout << "Can't open walk file" << endl;
  }
  walk.setLoop(true);
  */

  // Fitting the collider size
  hitbox.setSize(Vector2f(body.getSize().x - 20, body.getSize().y - 15));
  hitbox.setPosition(body.getPosition());
  hitbox.setOrigin(hitbox.getSize().x / 2, hitbox.getSize().y / 2);
  hitbox.setFillColor(Color::Blue);
}

Caster::~Caster() {}

void Caster::Update(float deltaTime, RenderWindow &rw, Player &p) {
  fireRateControl++;
  float dist = sqrt(pow(body.getPosition().x - p.getSkinPosition().x, 2) +
                    pow(body.getPosition().y - p.getSkinPosition().y, 2));

  Vector2f movement(0.0f, 0.0f);

  // Prevent out of bound
  if (hitbox.getPosition().x - hitbox.getSize().x / 2 < 0) {
    // cout << "OOB Left" << endl;
    hitbox.setPosition(
        Vector2f(hitbox.getSize().x / 2, hitbox.getPosition().y));
  }
  if (hitbox.getPosition().x + hitbox.getSize().x / 2 > rw.getSize().x) {
    // cout << "OOB Right" << endl;
    hitbox.setPosition(
        Vector2f(25 + hitbox.getPosition().x - hitbox.getSize().x / 2,
                 hitbox.getPosition().y));
  }
  if (hitbox.getPosition().y - hitbox.getSize().y / 2 < 0) {
    // cout << "OOB Top" << endl;
    hitbox.setPosition(
        Vector2f(hitbox.getPosition().x, hitbox.getSize().y / 2));
  }
  if (hitbox.getPosition().y + hitbox.getSize().y > rw.getSize().y) {
    // cout << "OOB Bottom" << endl;
    hitbox.setPosition(
        Vector2f(hitbox.getPosition().x,
                 40 + hitbox.getPosition().y - hitbox.getSize().y / 2));
  }

  if (++timeToRandom > 30) {
    randomNumber = 0 + (rand() % 6); // Randomizer don't work!
    timeToRandom = 0;
  }

  if (dist >= 1000) // Out of tracking range
  {
    if (randomNumber == 0) {
      movement.x -= speed * deltaTime;
    } else if (randomNumber == 1) {
      movement.x += speed * deltaTime;
    } else if (randomNumber == 2) {
      movement.y -= speed * deltaTime;
    } else if (randomNumber == 3) {
      movement.y += speed * deltaTime;
    } else {
      // No movement
    }
  } else // Track player within range
  {
    if (dist >= 600) // Track
    {
      float distX = fabs(body.getPosition().x - p.getSkinPosition().x),
            distY = fabs(body.getPosition().y - p.getSkinPosition().y);
      if (distX >= 3 || distY >= 3) {
        if (body.getPosition().x < p.getSkinPosition().x) {
          movement.x += speed * deltaTime;
        }
        if (body.getPosition().x > p.getSkinPosition().x) {
          movement.x -= speed * deltaTime;
        }

        if (body.getPosition().y > p.getSkinPosition().y) {
          movement.y -= speed * deltaTime;
        }
        if (body.getPosition().y < p.getSkinPosition().y) {
          movement.y += speed * deltaTime;
        }
      } else {
        action = false;
      }
    } else // Shoot Bullet
    {
      if (fireRateControl >= 120) // Fire 1/3 shot/sec
      {
        fireRateControl = 0;
        row = 5;
        // play sound
        // action = true;
        Vector2f aimDir;
        aimDir = p.GetPosition() - body.getPosition();
        aimDir = aimDir / sqrt(pow(aimDir.x, 2) + pow(aimDir.y, 2));

        b.shape.setPosition(body.getPosition().x, body.getPosition().y);
        b.currVelocity = aimDir * b.maxSpeed;
        b.shape.setFillColor(Color::Red);
        bulletVector.push_back(b);
      }
    }
  }

  if (movement.x == 0.0f && movement.y == 0.0f && !action) // When idle
  {
    row = 0;
    /*if (isWalking)
    {
            walk.pause();
            isWalking = false;
    }*/
  } else // When moving
  {
    if (!action) {
      row = 1;
    }
    /*if (!isWalking)
    {
            walk.play();
            walk.setVolume(70);
            isWalking = true;
    }*/

    if (movement.x > 0.0f) {
      faceRight = true;
    } else if (movement.x < 0.0f) {
      faceRight = false;
    }
  }

  for (size_t i = 0; i < bulletVector.size(); i++) {
    bulletVector[i].shape.move(bulletVector[i].currVelocity);
    if (bulletVector[i].shape.getPosition().x < 0 ||
        bulletVector[i].shape.getPosition().y < 0 ||
        bulletVector[i].shape.getPosition().x >= rw.getSize().x ||
        bulletVector[i].shape.getPosition().y >= rw.getSize().y) {
      bulletVector.erase(bulletVector.begin() + i);
    }
  }

  if (health > 0) {
    animation.Update(row, deltaTime, faceRight); // Update the animation
    body.setTextureRect(animation.uvRect);
    hitbox.move(movement); // move hitbox and apply body texture to it
  }
  /*else
  {
          row = 2;
          if (animation.playRow(row,deltaTime,faceRight))
          {
                  animation.playRow(row, deltaTime, faceRight);
                  body.setTextureRect(animation.uvRect);
          }
          else
          {
                  hitbox.setPosition(999999.0f, 999999.0f);
          }
  }*/
  body.setPosition(hitbox.getPosition());
}

void Caster::Draw(RenderWindow &rw, float deltaTime) {
  // rw.draw(hitbox);
  /*if (row == 5)
  {
          while (animation.playRow(row, deltaTime, faceRight))
          {
                  body.setTextureRect(animation.uvRect);
                  rw.draw(body);
          }
  }
  else
  {
          rw.draw(body);
  }*/
  rw.draw(body);
  for (size_t i = 0; i < bulletVector.size(); i++) {
    rw.draw(bulletVector[i].shape);
  }
}

void Caster::resetStat() {
  row = 0;
  faceRight = true;
  randomNumber = 0;
  body.setPosition((float)(rand() % 1800), (float)(rand() % 900));
  hitbox.setPosition(body.getPosition());
  health = 40;
}

bool Caster::bulletCollision(Collider other, int number, float force) {
  if (bulletVector[number].getCollider().CheckCollision(other, force)) {
    return true;
  }
  return false;
}

void Caster::takeDamage(int damage) {
  const float iFrameLength = 0.1f;
  if (iFrame.getElapsedTime().asSeconds() >= iFrameLength) {
    health -= damage;
    iFrame.restart();
  }
}

bool Caster::isDead(RenderWindow &rw, float deltaTime) {
  if (health <= 0) {
    const int row = 2;
    /*while (animation.playRow(row, deltaTime, faceRight))
    {
            body.setTextureRect(animation.uvRect);
            rw.draw(body);
    }*/
    body.setFillColor(Color::Red);
    hitbox.setPosition(999999.0f, 999999.0f);
    return true;
  }
  return false;
}
