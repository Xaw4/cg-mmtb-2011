#include "Bullet.h"

#include <iostream>
#include <cstdlib>

#include <GLee.h>
#include <GL/gl.h>		// OpenGL header
#include <GL/glu.h>		// OpenGL Utility header
#include <GL/glut.h>	// GLUT header

using glm::vec3;

const float Bullet::speed=6.5f;

Bullet::Bullet(vec3 position, bool isEnemy) :
	size(0.2f),
	position(position),
	isEnemy(isEnemy)
{
}


Bullet::~Bullet()
{
}



void Bullet::draw(){

	glPushMatrix();{
		glTranslatef(position.x, position.y, position.z);
		glColor3f(0.8f, 0.8f, 0.1f);
		glutSolidSphere(size, 30, 30);
	}glPopMatrix();

}


void Bullet::update(float delta){
	vec3 distance(0,0, delta * Bullet::speed);
	distance *= isEnemy ? 1 : -3;
	position += distance;
}