#include "scene.h"

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

	int speed=5;
	int a = 0;
	SnakePart Head;
	SnakePart* Tail = NULL;
	//glm::vec3 speed = glm::vec3(0.1,0,0);
	int myLvL=0;

public:
	int needLvL =0;
	Snake();
	Snake(Shape* Head,int l);
	void addPart(Shape* part,int l);
	void move(Scene* myGame);


};