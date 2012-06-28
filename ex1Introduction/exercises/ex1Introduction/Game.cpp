#include "Game.h"

#include <iostream>
#include <cstdlib>

#include <GLee.h>
#include <GL/gl.h>		// OpenGL header
#include <GL/glu.h>		// OpenGL Utility header
#include <GL/glut.h>	// GLUT header

#include <glm/glm.hpp>
#include <glm/gtx/random.hpp>

using glm::vec3;
using std::vector;



const float Game::FIELD_WIDTH = 21;
const float Game::FIELD_HEIGHT= 20;

const float Game::DEATH_DISTANCE= 4;

const unsigned Game::X_COUNT_INVADERS = 7;
const unsigned Game::Z_COUNT_INVADERS = 4;

const float Game::INVADER_SPAN = 1.7f;

const float Game::DYING_TIME = 0.5f;

Game::Game() :
		shot(0),
		invaderDirection(3.0f, 0, 0.3f)
		
{
	spawnInvaders();
	//shipModel = oogl::loadModel("models/fighter.3ds", oogl::Model::LOAD_SET_SMOOTHING_GROUP);
}


Game::~Game()
{
	if(shot){
		delete shot;
	}

	//delete shipModel;
}

void Game::start(){
	invaders.clear();
	bullets.clear();

	if(shot) {
		delete shot;
		shot=0;
	}

	player = Ship();
	spawnInvaders();

	gameState = 0;
}

void Game::spawnInvaders(){
	// x position of the first invader per row
	const float firstX = (Game::FIELD_WIDTH/2) - ((Game::X_COUNT_INVADERS/2)*Game::INVADER_SPAN);
	// x,z position of the spwning invader
	float x = firstX;
	float z = -Game::FIELD_HEIGHT;
	for(unsigned iz=0; iz<Game::Z_COUNT_INVADERS; ++iz){
		for(unsigned ix=0; ix<Game::X_COUNT_INVADERS; ++ix){
			Invader inv( vec3(x, 0, z), invaderDirection);
			invaders.push_back(inv);

			x+=Game::INVADER_SPAN;
		}
		x=firstX;
		z+=Game::INVADER_SPAN;
	}
	
}


void Game::moveLeft(){
	player.moveLeft();
}
void Game::moveRight(){
	player.moveRight();
}
void Game::stopMovement(){
	player.stopMovement();
}

void Game::draw(){
	/*static const vec3 lookat(
		Game::FIELD_WIDTH/2,
		-Game::FIELD_HEIGHT,
		0);*/
	glPushMatrix();{

		vec3 eye(player.position + vec3(0, 4.0f, 5.0f));
		vec3 lookat(player.position + vec3(0, 0, -Game::FIELD_HEIGHT*(2/3)));
		
		
		//std::cout << "eye: " << eye.x << "," << eye.y << "," << eye.y << std::endl;
		//std::cout << "lookat: " << lookat.x << "," << lookat.y << "," << lookat.y << std::endl;

		gluLookAt(
			eye.x, eye.y, eye.z,
			lookat.x, lookat.y, lookat.z,
			0,1,0);

		player.draw();
		drawInvaders();
		drawBullets();

		if(shot) shot->draw();

	}glPopMatrix();


}

void Game::drawInvaders(){
	vector<Invader>::iterator it;
	for(it=invaders.begin(); it!=invaders.end(); ++it){
		it->draw();
	}
}

void Game::drawBullets(){
	vector<Bullet>::iterator it = bullets.begin();
	while(it!=bullets.end()){
		it->draw();
		++it;
	}
}

void Game::update(float delta){
	if( player.hasDied() ){
		gameState = 2;
	}
	if( gameState == 0){
		player.update(delta);

		updateInvaders(delta);
	
		//update bullet
		if(shot){
			if(shot->position.z < -(Game::FIELD_HEIGHT+0.3f) ){
				delete shot;
				shot=0;
			} else{
				shot->update(delta);
			}
		}
		// update bulletS
		vector<Bullet>::iterator it = bullets.begin();
		while(it!=bullets.end()){
			// is the distance between player an ship low ?
			const float distance= glm::distance(it->position, player.position);
			const float minDist = it->getSize() + player.getSize();
			if (distance < minDist){
				player.die();
				it = bullets.erase(it);
			} else {
				it->update(delta);
				++it;
			}
		}
	}

}

void Game::updateInvaders(float delta){

	//value if invaders should turn (comes from last call)
	static bool shouldTurn = false;

	// flag if it should turn next time
	bool shouldTurnNext = false;

	vector<Invader>::iterator it = invaders.begin();
	while(it!=invaders.end()){
		if(it->hasDied()){
			it = invaders.erase(it);
		} else {
			// check if it should die
			if(shot && !it->wasShot()){
				const float distance = glm::distance(it->position, shot->position);
				const float minDist = it->getSize() + shot->getSize();
				// is the Vector betwean the two (shot and invader) longer than 
				// the two collision radiuses? 
				if( distance < minDist ){
						it->die();

						delete shot;
						shot=0;
				}
			}

			// let it turn if needed
			if(shouldTurn){
				it->turn();
			}

			shouldTurnNext |= it->update(delta);
			if(it->isOutOfBounds()){
				player.die();
			}

			++it;
		}
	}

	// shooting invaders
	float chancevalue=glm::gtx::random::compRand1<float>();
	//  having a 10 % chance to shoot
	if( 0.5f < chancevalue && chancevalue < 0.51f  && 
			invaders.size() > 0){
		int invaderNum = glm::gtx::random::compRand1<int>(0, invaders.size()-1);
		bullets.push_back(Bullet(invaders[invaderNum].position, true));
	}

	shouldTurn = shouldTurnNext;
}

void Game::shoot(){
	//player.shoot();

	if(shot==0){
		shot = new Bullet(player.position, false);
		
		std::cout << "shoot: " << shot->position.x << ","
			<< shot->position.y << "," 
			<< shot->position.y << std::endl;
	}
}