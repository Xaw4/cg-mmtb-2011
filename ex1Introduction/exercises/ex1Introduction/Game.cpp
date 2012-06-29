#include "Game.h"

#include <iostream>
#include <cstdlib>

#include <GLee.h>
#include <GL/gl.h>		// OpenGL header
#include <GL/glu.h>		// OpenGL Utility header
#include <GL/glut.h>	// GLUT header

#include <glm/glm.hpp>
#include <glm/gtx/random.hpp>
#include <oogl/Image.h>

using glm::vec3;
using std::vector;



const float Game::FIELD_WIDTH = 21;
const float Game::FIELD_HEIGHT= 20;

const float Game::DEATH_DISTANCE= 4;

const unsigned Game::X_COUNT_INVADERS = 7;
const unsigned Game::Z_COUNT_INVADERS = 4;

const float Game::INVADER_SPAN = 1.7f;

const float Game::DYING_TIME = 0.5f;


GLuint Game::PAUSETEX = -1;

Game::Game() :
		shot(0),
		invaderDirection(3.0f, 0, 0.3f),
		aspectRatio(4/3.0f)
{
	//shipModel = oogl::loadModel("models/fighter.3ds", oogl::Model::LOAD_SET_SMOOTHING_GROUP);
	start();

	/// load texture

	//load a image using the library and devil
		std::auto_ptr<oogl::Image> image(oogl::loadImage("models/R2Head.jpg"));
		glGenTextures(1, &Game::PAUSETEX); //generate a new texture

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, Game::PAUSETEX); //bind our texture

		//what should be done, if you want to access a texture outside the normal 0..1 range?
		//one of GL_REPEAT, GL_CLAMP, GL_MIRRORED_REPEAT, GL_CLAMP_TO_BORDER
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		//set texture interpolation method to use linear interpolation (no MIPMAPS)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		//define the texture and load data
		glTexImage2D(GL_TEXTURE_2D,
			0, //mip map level 0..top
			GL_RGBA, //internal format of the data in memory
			image->getWidth(),
			image->getHeight(),
			0,//border width in pixels (can either be 1 or 0)
			image->getFormat(),	// Image format (i.e. RGB, RGBA, BGR etc.)
			image->getType(),// Image data type
			image->getData());// The actual image data itself

		glBindTexture(GL_TEXTURE_2D, 0); //unbind again
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
		if(gameState==0){ // playing
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			gluPerspective(70, aspectRatio, 0.1f, 100);

			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();

			vec3 eye(player.position + vec3(0, 4.0f, 5.0f));
			vec3 lookat(player.position + vec3(0, 0, -Game::FIELD_HEIGHT*(2/3)));
		
		
			//std::cout << "eye: " << eye.x << "," << eye.y << "," << eye.y << std::endl;
			//std::cout << "lookat: " << lookat.x << "," << lookat.y << "," << lookat.y << std::endl;

			gluLookAt(
				eye.x, eye.y, eye.z,
				lookat.x, lookat.y, lookat.z,
				0,1,0);


			if(shot) shot->draw();

			drawInvaders();
			drawBullets();

			player.draw();
		} else if(gameState==1){ //paused
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			glOrtho(0, 1, 0, 1, 
					0, 1);
			// we now hvae a normalized drawing room (0..1 width, 0..1 height, 0..1 depth)
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();

			glPushMatrix();
			
			glDisable(GL_LIGHTING);

			glEnable(GL_TEXTURE_2D);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, PAUSETEX);

			glEnable(GL_COLOR_MATERIAL); //use vertex colors instead of material colors
			glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE); //use the vertex color as diffuse color
			
			glBegin(GL_QUADS);{

				glColor4f(1,1,1,1);
				glNormal3f(0,0,1);


				glTexCoord2f(0,0);
				glVertex3f(0,0,0.5f);

				glTexCoord2f(0,1);
				glVertex3f(0,1,0.5f);

				glTexCoord2f(1,1);
				glVertex3f(1,1,0.5f);

				glTexCoord2f(1,0);
				glVertex3f(1,0,0.5f);

			}glEnd();

			glDisable(GL_COLOR_MATERIAL);

			glBindTexture(GL_TEXTURE_2D, 0);
			glDisable(GL_TEXTURE_2D);
			
			
			glEnable(GL_LIGHTING);

			glPopMatrix();
			
		}

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

void Game::drawPause(){

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