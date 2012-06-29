#pragma once

#include <glm/glm.hpp>
#include <vector>

#include "Ship.h"
#include "Invader.h"
#include "Bullet.h"

#include <oogl/gl_error.h>

// #include <oogl/Model.h>

/**
	this class manages the whole logic
**/
class Game{

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

	// container for all invaders (dead invaders will be removed)
	std::vector<Invader> invaders;

	//vector of invader bullets
	std::vector<Bullet> bullets;

	// bullet player is just shooting
	Bullet* shot;

	/// initial direction for invaders
	glm::vec3 invaderDirection;

	/// used for the perspective proj in the draw func.
	float aspectRatio;

	//oogl::Model* shipModel;

	void spawnInvaders();


public:
	static const float FIELD_WIDTH;
	static const float FIELD_HEIGHT;

	// if the invader come to this distance, the player looses
	static const float DEATH_DISTANCE;


	// the time a player or Invader needs to die (death-animation)
	static const float DYING_TIME;

	static GLuint PAUSETEX;

	Game();
	~Game();

	/// (re)starts the game (deletes shots and invaders, replaceses them
	/// and renews the player and sets gamestate to started
	void start();
	
	// functions to provide an interface to move the player
	void moveLeft();
	void moveRight();
	void stopMovement();

	void draw();
	void drawInvaders();
	void drawBullets();

	void drawPause();

	void update(float delta);
	void updateInvaders(float delta);
	void shoot();

	int getState(){ return gameState; }
	void setState(int state){ gameState = state; } 

	float getAspectRatio(){ return aspectRatio; }
	void setAspectRatio(float ar){ aspectRatio = ar; }

};

