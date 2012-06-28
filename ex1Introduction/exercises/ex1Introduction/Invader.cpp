#include "Invader.h"

#include "Game.h"

#include <iostream>
#include <cstdlib>

#include <GLee.h>
#include <GL/gl.h>		// OpenGL header
#include <GL/glu.h>		// OpenGL Utility header
#include <GL/glut.h>	// GLUT header

#include <glm/gtx/random.hpp>

using glm::vec3;

Invader::Invader(glm::vec3 position, glm::vec3 direction):
	position(position),
	size(0.6f),
	isDead(false),
	isDying(false),
	direction(direction)
{
}


Invader::~Invader()
{
}

void Invader::die(){
	if(!isDying && !isDead){
		isDying = true;
		deadTime= 0;
	}
}

void Invader::draw(){

	glPushMatrix();{

		glTranslatef(position.x, position.y, position.z);
		
		// if alive -> draw regular
		if(!isDying){
			glColor3f(0.2f, 0.0f, 0.2f);
			glutSolidCube(size*2);
			glColor3f(0.8f, 0.1f, 0.1f);
			glutWireCube(size*2);
		} else { // if dying -> draw deathanimation


			glEnable (GL_BLEND);
			glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			float anim =(deadTime/Game::DYING_TIME);
			// this makes a better explosion animation
			vec3 explosiontrans;
			explosiontrans = glm::gtx::random::vecRand3(0.01f, 0.15f);
			
			glTranslatef(explosiontrans.x, explosiontrans.y, explosiontrans.z);

			//std::cout << "i die: " << anim << std::endl;
			glColor4f(0.9f, 0.2f, 0.0f, 1-anim);
			glutSolidSphere( anim * 2 * size , 30, 30);

			explosiontrans = glm::gtx::random::vecRand3(0.01f, 0.15f);
			
			glTranslatef(explosiontrans.x, explosiontrans.y, explosiontrans.z);

			//std::cout << "i die: " << anim << std::endl;
			glColor4f(0.9f, 0.8f, 0.0f, 1-anim);
			glutSolidSphere( anim *1.5f* size , 30, 30);

			glDisable(GL_BLEND);
		}

	}glPopMatrix();


}


bool Invader::update(float delta){
	//std::cout << "inv: " << position.x << "," << position.y << "," << position.y << std::endl;
	
	// is alife -> move 
	if(!isDying && !isDead){
		position += direction*delta;
		if( (position.x < 0 && direction.x < 0) 
			|| (Game::FIELD_WIDTH < position.x && direction.x > 0)) {
			return true;
		} 
	} 
	if(isDying && !isDead) { // isdying -> do death-animation
		deadTime += delta;
		if(deadTime > Game::DYING_TIME){
			isDead = true;
		}
	}
	return false;
}

void Invader::turn(){
	direction = vec3(-direction.x, 0, direction.z);
}

//returns true if this invader is the Death range
bool Invader::isOutOfBounds(){
	return position.z > -Game::DEATH_DISTANCE;
}