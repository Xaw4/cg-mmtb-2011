#pragma once

#include <glm/glm.hpp>

class Bullet
{

private:
	static const float speed;

	float size;

	// true if shotten by an invader
	bool isEnemy;

public:
	glm::vec3 position;

	Bullet(glm::vec3 position, bool isEnemy);
	~Bullet();

	void draw();
	void update(float delta);

	float getSize(){return size;}
	/** returns true if this bullet was shot by an invader*/
	bool shotByEnemy(){return isEnemy;}
};

