#define GLEW_STATIC
#include <GL\glew.h>
#include <utility> 
#include "Snake.h"
#include "Log.hpp"


Snake::Snake(Shape* Head,Game* g,int l) {
	myGame = g;
	this->Head.offsetpostion = glm::vec3(0);
	this->Head.offsetRotion = glm::vec3(0);
	this->Head.me = Head;
	this->Head.Ifallow = NULL;
	this->Head.MySon = NULL;
	this->Head.loctionInSapes = l;
	Tail = &this->Head;
}
void Snake::addPart(Shape* part,int l) {
	SnakePart* newPart = new SnakePart();
	newPart->me = part;
	Tail->MySon = newPart;
	newPart->Ifallow = Tail;
	newPart->MySon = NULL;
	newPart->loctionInSapes = l;
	// need cheking
	newPart->offsetpostion = glm::vec3(part->makeTrans()[0].w - Tail->me->makeTrans()[0].w,
		part->makeTrans()[1].w - Tail->me->makeTrans()[1].w,
		part->makeTrans()[2].w - Tail->me->makeTrans()[2].w);
	//float offx = glm::atan(part->makeTrans()[2].y, part->makeTrans()[2].z) -  glm::atan(Tail->me->makeTrans()[2].y, Tail->me->makeTrans()[2].z);
	//float offy = glm::asin(-part->makeTrans()[2].x) - glm::asin(-Tail->me->makeTrans()[2].x)  ;
	Tail = newPart;
	// Tail->me->makeTransScale() - part->makeTransScale();
}
void Snake::move() {
	int p = myGame->pickedShape;
	SnakePart* temp = &Head;// we do not move the Head here
	while (temp->MySon != NULL)
	{
		temp = temp->MySon;
		// for now only do postion chase wiithout rotation
		glm::vec3 popPostion = glm::vec3(temp->Ifallow->me->makeTrans()[0].w, 0, temp->Ifallow->me->makeTrans()[2].w);
		glm::vec3 myPos = glm::vec3(temp->me->makeTrans()[0].w, 0, temp->me->makeTrans()[2].w);
		glm::vec3 newPlace = popPostion + temp->offsetpostion;
		glm::vec3 moveVec = glm::vec3(0.4)*(newPlace - myPos);
		myGame->pickedShape = temp->loctionInSapes;
		myGame->shapeTransformation(myGame->xLocalTranslate, moveVec.x);
		myGame->shapeTransformation(myGame->zLocalTranslate, moveVec.z);

	}
	myGame->pickedShape = p;
}