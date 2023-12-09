#include "Player.h"
#include <iostream>
#include <math.h>
#include <vector>
#define RWWIDTH 1920
#define RWHEIGHT 1080
using namespace std;
Player::Player(Texture *texture, Vector2u imageCount, float switchTime,
               float speed)
    : animation(texture, imageCount,
                switchTime) // Also use the variables passed with player class
                            // constructor
{
  Texture tex;
  if (!tex.loadFromFile("Bullet.png")) // Failed texture setting
  {
    // Handle error
  }
  b.shape.setTexture(&tex);
  this->speed = speed; // controlling movement speed
  row = 0;             // start as idle
  faceRight = true;    // start as facing right (default of this sprite)

  if (!gunShotBuffer.loadFromFile("GunShot.wav")) {
    // Error handling
  }
  gunShot.setBuffer(gunShotBuffer);
  gunShot.setVolume(10);

  if (!punchBuffer.loadFromFile("Punch.wav")) {
    // Error handling
  }
  punchSound.setBuffer(punchBuffer);
  punchSound.setVolume(10);

  const float xSize = 100.0f, ySize = 100.0f;
  body.setSize(Vector2f(xSize, ySize));        // set size of the player
  body.setPosition(RWWIDTH / 2, RWHEIGHT / 2); // set position of player
  body.setOrigin(Vector2f(
      xSize / 2,
      ySize / 2)); // set origin to compatible with AABB collision check

  body.setTexture(
      texture); // set texture of player (no & since texture here is pointer)

  if (!walk.openFromFile("walk.wav")) {
    // cout << "Can't open walk file" << endl;
  }
  walk.setLoop(true);

  // Fitting the collider size
  hitbox.setSize(
      Vector2f(xSize - 35, ySize - 7)); // Only work for 100,100 body size!
  hitbox.setPosition(body.getPosition());
  hitbox.setOrigin(hitbox.getSize().x / 2, hitbox.getSize().y / 2);
  hitbox.setFillColor(Color::Blue);

  punchHitbox.setSize(Vector2f(xSize - 5, ySize - 7));
  punchHitbox.setFillColor(Color::Red);
  punchHitbox.setPosition(body.getPosition());
  punchHitbox.setOrigin(punchHitbox.getSize().x / 2,
                        punchHitbox.getSize().y / 2);

  healthBar.setFillColor(Color::Red);
  healthBar.setSize(Vector2f(xSize, ySize - 80));
  healthBar.setOrigin(hitbox.getOrigin());
  healthBar.setPosition(hitbox.getPosition().x - 20,
                        hitbox.getPosition().y - (hitbox.getSize().y / 2) + 20);
}

Player::~Player() {}

Collider Player::GetCollider() // Use hitbox at collision detector
{
  return Collider(hitbox);
}

Collider Player::GetPunchCollider() { return Collider(punchHitbox); }

Vector2f Player::GetPosition() { return body.getPosition(); }

void Player::resetStat() {
  body.setPosition(RWWIDTH / 2, RWHEIGHT / 2); // set position of player
  row = 0;                                     // start as idle
  faceRight = true; // start as facing right (default of this sprite)
  hitbox.setPosition(body.getPosition());
  punchHitbox.setPosition(body.getPosition());
  healthBar.setPosition(hitbox.getPosition().x - 20,
                        hitbox.getPosition().y - (hitbox.getSize().y / 2) + 20);
  healthBar.setFillColor(Color::Red);
  health = 100;
  fireRateControl = 0;
  meleeRateControl = 0;
  maxHealth = 100;
  killCount = 0;
  bullets.clear();
  bulletDam = 20;
  meleeDam = 25;
  body.setFillColor(Color::White);
}

void Player::Update(float deltaTime, RenderWindow &rw) {
  ////cout << body.getPosition().x << " " << body.getPosition().y << endl;
  Vector2f movement(
      0.0f,
      0.0f); // Every time this method is called, movement will always be (0,0)

  // NOTE: Player can't move while attacking
  if (Keyboard::isKeyPressed(Keyboard::A) && !action) {
    movement.x -= speed * deltaTime;
  }
  if (Keyboard::isKeyPressed(Keyboard::D) && !action) {
    movement.x += speed * deltaTime;
  }
  if (Keyboard::isKeyPressed(Keyboard::W) && !action) {
    movement.y -= speed * deltaTime;
  }
  if (Keyboard::isKeyPressed(Keyboard::S) && !action) {
    movement.y += speed * deltaTime;
  }

  // Prevent out of bound
  if (hitbox.getPosition().x - hitbox.getSize().x / 2 < 0) {
    ////cout << "OOB Left" << endl;
    hitbox.setPosition(
        Vector2f(hitbox.getSize().x / 2, hitbox.getPosition().y));
  }
  if (hitbox.getPosition().x + hitbox.getSize().x / 2 > rw.getSize().x) {
    ////cout << "OOB Right" << endl;
    hitbox.setPosition(
        Vector2f(25 + hitbox.getPosition().x - hitbox.getSize().x / 2,
                 hitbox.getPosition().y));
  }
  if (hitbox.getPosition().y - hitbox.getSize().y / 2 < 0) {
    ////cout << "OOB Top" << endl;
    hitbox.setPosition(
        Vector2f(hitbox.getPosition().x, hitbox.getSize().y / 2));
  }
  if (hitbox.getPosition().y + hitbox.getSize().y > rw.getSize().y) {
    ////cout << "OOB Bottom" << endl;
    hitbox.setPosition(
        Vector2f(hitbox.getPosition().x,
                 40 + hitbox.getPosition().y - hitbox.getSize().y / 2));
  }

  fireRateControl++;
  meleeRateControl++;
  if (Mouse::isButtonPressed(Mouse::Left)) {
    // Do melee action
    if (meleeRateControl >= 10) // 4 hits/sec
    {
      row = 4;
      meleeRateControl = 0;
      punchSound.play();
      action = true;
      isPunching = true;
    }
  }
  /*else if(!Mouse::isButtonPressed(Mouse::Left))
  {
          isPunching = false;
  }*/
  else if (Mouse::isButtonPressed(Mouse::Right)) {
    if (fireRateControl >= 10) // Will fire at 4 shots/sec (40/10)
    {
      fireRateControl = 0; // Reset fire rate value
      row = 5;
      gunShot.play();
      action = true;
      Vector2f mousePos, aimDir;
      mousePos = Vector2f(rw.mapPixelToCoords(Mouse::getPosition(rw)).x,
                          rw.mapPixelToCoords(Mouse::getPosition(rw)).y);
      aimDir = mousePos - body.getPosition();
      aimDir = aimDir / sqrt(pow(aimDir.x, 2) + pow(aimDir.y, 2));
      ////cout << aimDir.x << " " << aimDir.y << endl;

      b.shape.setPosition(body.getPosition().x,
                          body.getPosition().y -
                              45); // Adjust bullet to gun point
      b.currVelocity = aimDir * b.maxSpeed;
      ////cout << b.currVelocity.x << " " << b.currVelocity.y << endl;
      b.shape.setFillColor(Color::White);
      bullets.push_back(b);
    }
  } else {
    action = false;
    isPunching = false;
  }

  for (size_t i = 0; i < bullets.size(); i++) // Update bullet position
  {
    ////cout << i << "#\t" << bullets[i].shape.getPosition().x << " " <<
    /// bullets[i].shape.getPosition().y << endl;
    bullets[i].shape.move(bullets[i].currVelocity);
    if (bullets[i].shape.getPosition().x < 0 ||
        bullets[i].shape.getPosition().y < 0 ||
        bullets[i].shape.getPosition().x >= rw.getSize().x ||
        bullets[i].shape.getPosition().y >= rw.getSize().y) {
      bullets.erase(bullets.begin() + i);
    }
  }

  if (movement.x == 0.0f && movement.y == 0.0f && !action) // When idle
  {
    row = 0;
    if (isWalking) // If already walking...
    {
      walk.pause();
      isWalking = false;
    }

    Vector2f mousePos = Vector2f(rw.mapPixelToCoords(Mouse::getPosition(rw)).x,
                                 rw.mapPixelToCoords(Mouse::getPosition(rw)).y);
    if (mousePos.x < body.getPosition().x) {
      faceRight = false;
    } else if (mousePos.x > body.getPosition().x) {
      faceRight = true;
    }
  } else // When moving
  {
    if (!action) {
      row = 1;
    }
    if (!isWalking && !action) // If wasn't walking yet...
    {
      walk.play();
      walk.setVolume(70);
      isWalking = true;
    }
    Vector2f mousePos = Vector2f(rw.mapPixelToCoords(Mouse::getPosition(rw)).x,
                                 rw.mapPixelToCoords(Mouse::getPosition(rw)).y);

    if (movement.x > 0.0f || mousePos.x > body.getPosition().x) {
      faceRight = true;
      if (mousePos.x < body.getPosition().x) {
        faceRight = false;
      }
    } else if (movement.x < 0.0f || mousePos.x < body.getPosition().x) {
      faceRight = false;
      if (mousePos.x > body.getPosition().x) {
        faceRight = true;
      }
    }
  }

  animation.Update(row, deltaTime, faceRight); // Update the animation
  body.setTextureRect(animation.uvRect);

  const float xSize = 100.0f, ySize = 20.0f;

  float percentHealth = (health / maxHealth) * 100;

  hitbox.move(movement); // move hitbox and apply body texture to it;
  healthBar.setSize(Vector2f(percentHealth, ySize));
  healthBar.setPosition(hitbox.getPosition().x - 20,
                        hitbox.getPosition().y - (hitbox.getSize().y / 2) + 20);
  body.setPosition(hitbox.getPosition());
  punchHitbox.setPosition(hitbox.getPosition());
}

void Player::Draw(RenderWindow &rw) {
  rw.draw(body);
  for (size_t i = 0; i < bullets.size(); i++) {
    rw.draw(bullets[i].shape);
  }
  rw.draw(healthBar);
}

bool Player::bulletCollision(Collider other, int number, float force) {
  if (bullets[number].getCollider().CheckCollision(other, force)) {
    return true;
  }
  return false;
}

void Player::setBulletTexture(Texture *tex) { b.setBulletTexture(tex); }

void Player::eraseBullet(int i) {
  bullets.erase(bullets.begin() + i);
  // cout << "Erase bullet at " << i << endl;
}

void Player::setBulletPosition(int i, float a, float b) {
  bullets[i].shape.setPosition(Vector2f(a, b));
}

Vector2f Player::getBulletPosition(int i) {
  return bullets[i].shape.getPosition();
}

bool Player::isDead(RenderWindow &rw, float deltaTime) {
  if (health <= 0) {
    const int row = 3;
    while (animation.playRow(row, deltaTime, faceRight)) {
      body.setTextureRect(animation.uvRect);
      rw.draw(body);
    }
    hitbox.setPosition(999999.0f, 999999.0f);
    walk.pause();
    return true;
  }
  return false;
}