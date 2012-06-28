#pragma once

#include <glm/glm.hpp>

#include <oogl/Model.h>

class Ship
{

private:

	static const float speed;

	//size of the collison-sphere
	float size;

	// used for explosion-animation
	bool isDead;
	bool isDying;

	// time cummulated from the time beeing shot
	float deadTime;

	bool movesLeft;
	bool movesRight;


public:

	glm::vec3 position;
	static oogl::Model* model;
	Ship();
	~Ship();

	void moveLeft();
	void moveRight();
	void stopMovement();

	// a method to start the dieing process
	void die();

	void draw();
	void update(float delta);

	bool wasShot(){ return isDying; }
	bool hasDied(){ return isDead; }

	float getSize(){ return size; }
};

