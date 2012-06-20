#pragma once

#include <glm/glm.hpp>

class Invader
{

private:

	float size;

	
	// used for explosion-animation
	bool isDead;
	bool isDying;

	// time cummulated from the time beeing shot
	float deadTime;

	//the direction this invader is moving (multi-directional speed)
	glm::vec3 direction;

	

public:
	glm::vec3 position;

	Invader(glm::vec3 position, glm::vec3 direction);
	~Invader();

	// a method to start the dieing process
	void die();

	void draw();
	// returns true if it runns out of bounds
	bool update(float delta);

	// forces inveader to turn
	void turn();

	//returns true if this invader is in the Death range
	bool isOutOfBounds();

	bool wasShot(){ return isDying; }
	bool hasDied(){return isDead;}
	float getSize(){return size;}
};

