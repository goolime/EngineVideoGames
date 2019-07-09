#pragma once
#include "shape.h"
#include "VertexArray.hpp"
#include "shader.h"
#include "MeshConstructor.h"
#include "game.h"
#include "texture.h"

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
	Game* myGame;
	SnakePart Head;
	SnakePart* Tail;
	glm::vec3 speed = glm::vec3(0.1,0,0);
	int myLvL=0;
public:
	int needLvL =0;
	Snake(Shape* Head,Game* g,int l);
	void addPart(Shape* part,int l);
	void move();


};