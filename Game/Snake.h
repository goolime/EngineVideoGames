#include "scene.h"
#define BASIC_SPEED 20

struct SnakePart
{
	int loctionInSapes;
	Shape* me;
	SnakePart* Ifallow;
	SnakePart* MySon;
	glm::vec3 offsetpostion;
	glm::vec3 offsetRotion;
};

class Snake 
{
private:

	void lvlChange(Scene* myGame);
	//int speed=1;
	int a = 0;

	SnakePart* Tail = NULL;
	//glm::vec3 speed = glm::vec3(0.1,0,0);
	int myLvL=0;

public:
	float zangle = 0;
	SnakePart Head;
	int canColiod = 0;
	int needLvL =0;
	Snake();
	Snake(Shape* Head,int l);
	void addPart(Shape* part,int l);
	void move(Scene* myGame);
	int speed = BASIC_SPEED;


};