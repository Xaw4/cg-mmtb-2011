#pragma once

#include <GLee.h>
#include <GL/gl.h>		// OpenGL header
#include <GL/glu.h>		// OpenGL Utility header
#include <GL/glut.h>	// GLUT header

#include <glm/glm.hpp>

#include <oogl/Model.h>

namespace InvaderMaterial{

	const float ambient[] = {.1f,0,0, 1.0f};
	const float diffuse[] = {.7f,0,0, 1.0f};
	const float specular[] = {3,0,0, 1.0f};
	const float emission[] = {.1f,0,0, 1.0f};

	const float shininess = 128;


	/// using a struct, because compiler dont likes a plain function in a namespace
	struct setMaterial{
		setMaterial(){
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
			glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emission);

			glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
		}
	};
}

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

	
public:	glm::vec3 position;

	static oogl::Model* model;

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

