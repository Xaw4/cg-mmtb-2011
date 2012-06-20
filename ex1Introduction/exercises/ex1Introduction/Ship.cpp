#include "Ship.h"
#include "Game.h"
#include <iostream>
#include <cstdlib>

#include <GLee.h>
#include <GL/gl.h>		// OpenGL header
#include <GL/glu.h>		// OpenGL Utility header
#include <GL/glut.h>	// GLUT header

#include <glm/gtx/random.hpp>

using glm::vec3;

const float Ship::speed = 10.0f;



Ship::Ship() :
	position(Game::FIELD_WIDTH/2, 0, 0),
	size(1),
	isDead(false),
	isDying(false),
	movesLeft(false),
	movesRight(false)
{
}


Ship::~Ship()
{
}



void Ship::moveLeft(){
	movesLeft=true;
	movesRight=false;
}
void Ship::moveRight(){
	movesRight=true;
	movesLeft=false;
}
void Ship::stopMovement(){
	movesRight=false;
	movesLeft=false;
}

void Ship::die(){
	if(!(isDying || isDead)){
		isDying = true;
		deadTime = 0;
	}
}

void Ship::draw(){
	glPushMatrix();{

		glTranslatef(position.x, position.y, position.z);
		if(!isDying){
			if(movesLeft){
				glRotatef(20, 0, 0, 1);
			} else if(movesRight){
				glRotatef(-20, 0, 0, 1);
			}
		
			glColor3f(0.1f, 0.8f, 0.1f);
			glutSolidCube(size*2);
			glColor3f(0.8f, 0.1f, 0.1f);
			glutWireCube(size*2);

		} else {// if dying -> draw deathanimation

			
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


void Ship::update(float delta){
	// move if player alive / not in death animation
	if(!(isDying || isDead)){
		if(movesLeft && position.x > 0){
			position.x-=delta*speed;
		} else if (movesRight && position.x < Game::FIELD_WIDTH){
			position.x+=delta*speed;
		}
	} else {
		deadTime += delta;
		// if the animationtime is over - player is dead
		if(deadTime > Game::DYING_TIME){
			isDead = true;
		}
	}
	
	//std::cout << "sh: " << position.x << "," << position.y << "," << position.y << std::endl;
}
