#include <iostream>
#include <cstdlib>
#include <ctime>

#include <GLee.h>
#include <GL/gl.h>		// OpenGL header
#include <GL/glu.h>		// OpenGL Utility header
#include <GL/glut.h>	// GLUT header

#include <oogl/gl_error.h>
#include <oogl/timer.h>

#include <oogl/Model.h>
#include <oogl/Image.h>

#include <glm/glm.hpp>

#include "Game.h"

using std::cout;
using std::endl;

int windowWidth=800;
int windowHeight=600;


//oogl::Model *mShip = NULL;
std::auto_ptr<oogl::Model> modelShip;
std::auto_ptr<oogl::Model> modelInvaders;

Game game;

void cleanup();

float getDeltaTime();
void update(int);

void init() {
	glEnable(GL_DEPTH_TEST); // we enable the depth test, to handle occlusions
	modelInvaders.reset(oogl::loadModel("models/space_frigate/space_frigate_0.3ds"));
	modelShip.reset(oogl::loadModel("models/shipA/shipA_3DS.3ds"));
	glDepthFunc(GL_LESS);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glBlendEquation(GL_FUNC_ADD);
	// enable lighting
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	Invader::model = modelInvaders.get();
	Ship::model = modelShip.get();


	game.setState(1);
}

//oogl::Model *mShip = NULL;
/**
 * called when a frame should be rendered
 */
void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(70, ((float)windowWidth)/windowHeight,0.1f, 100);

	glMatrixMode(GL_MODELVIEW);

	glPushMatrix();{
		game.draw();
		//modelShip->render();
	}glPopMatrix();


	LOG_GL_ERRORS();
	glutSwapBuffers();
}

/**
 * called when the window was resized
 * @param w new window width in pixel
 * @param h new window height in pixel
 */
void reshape(int w, int h) {
	windowWidth = w;
	windowHeight = h;
	game.setAspectRatio(w/(float)h);

	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(70, ((float)w)/h,0.1f, 100);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

/**
 * called when nothing else needs to be done
 */
void idle() {
	//force a redisplay
	glutPostRedisplay();
}

/**
 * called when the user pressed a key
 * @param key ASCII character code
 * @param x mouse x position in pixel relative to the window, when the key was pressed
 * @param y mouse y position in pixel relative to the window, when the key was pressed
 */
void keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 27: //27=esc
		exit(0);
		break;
	case 'a':
		cout << "left" << endl;
		game.moveLeft();
		break;
	case 'd':
		cout << "right" << endl;
		game.moveRight();
		break;
	case ' ':
	case 's':
		cout << "shot" << endl;
		game.shoot();
		break;
	case 'n':
		game.start();
		break;
	case 'p':
		switch(game.getState()){
		case 0:
			game.setState(1);
			break;
		case 1:
			game.setState(0);
			break;
		}
		break;
	default:
		
		cout << "Not assigned: " << key << endl;
		
	}
	glutPostRedisplay();
}
/**
 * called when the user relases a key
 * @param key ASCII character code
 * @param x mouse x position in pixel relative to the window, when the key was pressed
 * @param y mouse y position in pixel relative to the window, when the key was pressed
 */
void keyboardUp(unsigned char key, int x, int y) {
	switch (key) {
	case 'a':
		cout << "no-left" << endl;
		game.stopMovement();
		break;
	case 'd':
		cout << "no-right" << endl;
		game.stopMovement();
		break;
	default:
		cout << "Not assigned(up): " << key << endl;
		
	}
	glutPostRedisplay();
}

/**
 * called when the user pressed or released a mouse key
 * @param button which mouse button was pressed, one of GLUT_LEFT_BUTTON, GLUT_MIDDLE_BUTTON and GLUT_RIGHT_BUTTON
 * @param state button pressed (GLUT_DOWN) or released(GLUT_UP)
 * @param x mouse x position in pixel relative to the window, when the mouse button was pressed
 * @param y mouse y position in pixel relative to the window, when the mouse button was pressed
 */
void mouse(int button, int state, int x, int y) {
	glutPostRedisplay();
}

/**
 * called when the mouse moves
 * @param x mouse x position in pixel relative to the window
 * @param y mouse x position in pixel relative to the window
 */
void mouseMotion(int x, int y) {
}


int setupGLUT(int argc, char** argv) {
	glutInit(&argc, argv);
	// glutInitContextVersion(3, 0);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_ALPHA);
	glutInitWindowSize(windowWidth, windowHeight);
	glutInitWindowPosition(100, 100);

	int windowId = glutCreateWindow("Space");

	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutKeyboardUpFunc(keyboardUp);
	glutMouseFunc(mouse);
	glutMotionFunc(mouseMotion);

	glutTimerFunc(25, update, 0);  //request to call in at least 25ms

	return windowId;
}

int main(int argc, char** argv) {
	setupGLUT(argc, argv);

	oogl::dumpGLInfos();

	init();

	glutMainLoop();

	return 0;
}



/// returns the time difference in seconds since the last call of it
float getDeltaTime(){
	// I am aware, that on the first call of this function it will
	// return (almost) zero - but just on the first call
	static unsigned long lasttime = clock();
	static unsigned long thistime;

	thistime = clock();

	float delta;
	delta = (float)(thistime - lasttime)/CLOCKS_PER_SEC;
	lasttime = thistime;

	return delta;
}

void update(int value){
	float delta = getDeltaTime();

	game.update(delta);

	glutPostRedisplay();
	glutTimerFunc(25, update, 0); //request to call again in at least 25ms

}