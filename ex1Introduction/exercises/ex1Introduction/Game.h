#pragma once

#include <glm/glm.hpp>
#include <vector>

#include "Ship.h"
#include "Invader.h"
#include "Bullet.h"

class Game
{

private:

	// how many invaders should spawn in x/z dimension
	static const unsigned X_COUNT_INVADERS;
	static const unsigned Z_COUNT_INVADERS;

	// distance between two invader origins
	static const float INVADER_SPAN;

	// state of the game
	// 0 .. running
	// 1 .. paused
	// 2 .. gameOver
	int gameState;

	Ship player;

	std::vector<Invader> invaders;

	//vector of invader bullets
	std::vector<Bullet> bullets;

	// bullet player is just shooting
	Bullet* shot;

	glm::vec3 invaderDirection;

	void spawnInvaders();


public:
	static const float FIELD_WIDTH;
	static const float FIELD_HEIGHT;

	// if the invader come to this distance, the player looses
	static const float DEATH_DISTANCE;


	// the time a player or Invader needs to die (death-animation)
	static const float DYING_TIME;

	Game();
	~Game();

	void start();
	
	// functions to provide an interface to move the player
	void moveLeft();
	void moveRight();
	void stopMovement();

	void draw();
	void drawInvaders();
	void drawBullets();
	void update(float delta);
	void updateInvaders(float delta);
	void shoot();

	int getState(){ return gameState; }
	void setState(int state){ gameState = state; } 
};

