#define GLEW_STATIC
#include <GL\glew.h>
#include <math.h>
#include <utility> 
#include "Snake.h"
#include "shape.h"
#include "Log.hpp"

Snake::Snake() {}

Snake::Snake(Shape* Head,int l) {

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
	glm::mat4 test = part->makeTransScale();
	newPart->offsetpostion = glm::vec3(part->makeTransScale()[3].x - Tail->me->makeTransScale()[3].x,
		part->makeTransScale()[3].y - Tail->me->makeTransScale()[3].y,
		part->makeTransScale()[3].z - Tail->me->makeTransScale()[3].z);
	//float offx = glm::atan(part->makeTrans()[2].y, part->makeTrans()[2].z) -  glm::atan(Tail->me->makeTrans()[2].y, Tail->me->makeTrans()[2].z);
	//float offy = glm::asin(-part->makeTrans()[2].x) - glm::asin(-Tail->me->makeTrans()[2].x)  ;
	Tail = newPart;
	// Tail->me->makeTransScale() - part->makeTransScale();
}
void Snake::move(Scene* myGame) {
	lvlChange(myGame);
	int p = myGame->pickedShape;

	SnakePart* temp = &Head;// we do not move the Head here
	
	//move head;
	
	glm::vec3 unitvector(-0.01*speed, 0, 0);

	glm::vec3 myMove = glm::mat3x3(temp->me->makeTransScale()) * unitvector;
	myGame->shapeTransformation(myGame->xGlobalTranslate, myMove.x);
//	myGame->shapeTransformation(myGame->yGlobalTranslate, myMove.y);
	myGame->shapeTransformation(myGame->zGlobalTranslate, myMove.z);

	while (temp->MySon != NULL)
	{
		temp = temp->MySon;

		myGame->pickedShape = temp->loctionInSapes;
		// for now only do postion chase wiithout rotation
		glm::vec3 popPostion = glm::vec3(temp->Ifallow->me->makeTransScale()[3].x, temp->Ifallow->me->makeTransScale()[3].y, temp->Ifallow->me->makeTransScale()[3].z);
		glm::vec3 myPos = glm::vec3(temp->me->makeTransScale()[3].x, temp->me->makeTransScale()[3].y, temp->me->makeTransScale()[3].z);
		glm::vec3 newPlace = popPostion + glm::mat3x3(temp->Ifallow->me->makeTransScale()) * temp->offsetpostion;
		//glm::vec3 newPlace = popPostion +  temp->offsetpostion;
		glm::vec3 moveVec = glm::vec3(0.01*speed)*(newPlace - myPos);
		myGame->shapeTransformation(myGame->xGlobalTranslate, moveVec.x);
		myGame->shapeTransformation(myGame->yGlobalTranslate, moveVec.y);
		myGame->shapeTransformation(myGame->zGlobalTranslate, moveVec.z);


		//rotion
		glm::vec3 unitvector(1, 0, 0);
		glm::vec3 popOrition = glm::normalize( glm::mat3x3(temp->Ifallow->me->makeTransScale()) * unitvector);
		glm::vec3 myOrition =glm::normalize( glm::mat3x3( temp->me->makeTransScale()) * unitvector);
		//x
		glm::vec3 atala(0, 1, 1);
		glm::vec3 crossy = glm::cross(popOrition * atala, myOrition * atala);
		float cosO = (popOrition.y * myOrition.y + popOrition.z * myOrition.z) / (glm::length(glm::vec2(popOrition.y,popOrition.z))*glm::length(glm::vec2(myOrition.y,myOrition.z)));
		float alpha = (glm::acos(cosO));
		if (crossy.x > 0)
			alpha = -alpha;
		if (alpha< M_PI&& alpha > -M_PI&&glm::abs(alpha) > 0.02)
		myGame->shapeTransformation(myGame->xLocalRotate, alpha);





		//y
		 atala= glm::vec3(1, 0, 1);
		 crossy=  glm::cross(popOrition * atala, myOrition * atala);
		 cosO = (popOrition.x * myOrition.x + popOrition.z * myOrition.z) / (glm::length(glm::vec2(popOrition.x, popOrition.z))*glm::length(glm::vec2(myOrition.x, myOrition.z)));
		 alpha =(glm::acos(cosO));
		if (crossy.y > 0)
			alpha =- alpha;
		if (alpha< M_PI&& alpha > -M_PI&&glm::abs(alpha) > 0.02)
		myGame->shapeTransformation(myGame->yLocalRotate, alpha);


		//z
		atala = glm::vec3(1, 1, 0);
		 crossy = glm::cross(popOrition * atala, myOrition * atala);
		 cosO = (popOrition.x * myOrition.x + popOrition.y * myOrition.y) / (glm::length(glm::vec2(popOrition.x, popOrition.y))*glm::length(glm::vec2(myOrition.x, myOrition.y)));
		 alpha = (glm::acos(cosO));
		if (crossy.z > 0)
			alpha = -alpha;
		if(alpha< M_PI&& alpha > -M_PI && glm::abs(alpha) > 0.02)
			myGame->shapeTransformation(myGame->zLocalRotate, alpha);

	}
	myGame->pickedShape = p;
}


void Snake::lvlChange(Scene* myGame) {
	int lvldiff = needLvL - myLvL;
	SnakePart* temp = &Head;
	myGame->pickedShape = temp->loctionInSapes;
	// for now only do postion chase wiithout rotation
	glm::vec3 myPos = glm::vec3(temp->me->makeTransScale()[3]);
	glm::vec3 popPostion = glm::vec3(0, 2*lvldiff+1+2*myLvL, 0);
	glm::vec3 newPlace = popPostion;
	//glm::vec3 newPlace = popPostion +  temp->offsetpostion;
	glm::vec3 moveVec = glm::vec3(0.01*speed)*(newPlace - myPos);
	//myGame->shapeTransformation(myGame->xGlobalTranslate, moveVec.x);
	myGame->shapeTransformation(myGame->yGlobalTranslate, moveVec.y);
	//myGame->shapeTransformation(myGame->zGlobalTranslate, moveVec.z);

	if ((lvldiff > 0 && popPostion.y <= myPos.y+0.01)|( lvldiff < 0 && popPostion.y >= myPos.y-0.01))
	{
		canColiod -= 32; //ramps 
		myLvL = needLvL;
		myGame->shapeTransformation(myGame->zLocalRotate, -zangle);
		zangle = 0;
	}


	
		////rotion
		//glm::vec3 unitvector(1, 0, 0);

		//glm::vec3 myOrition = glm::normalize(glm::mat3x3(temp->me->makeTransScale()) * unitvector);
		///*x+z = size 1 vector
		//if i want 45* il give it y = 1;

		//*/

		//glm::vec3 popOrition = glm::normalize(glm::vec3(myOrition.x,glm::sqrt(myOrition.x*myOrition.x + myOrition.z*myOrition.z)*lvldiff, myOrition.z));
		////x
		//glm::vec3 atala(0, 1, 1);
		//glm::vec3 crossy = glm::cross(popOrition * atala, myOrition * atala);
		//float cosO = (popOrition.y * myOrition.y + popOrition.z * myOrition.z) / (glm::length(glm::vec2(popOrition.y, popOrition.z))*glm::length(glm::vec2(myOrition.y, myOrition.z)));
		//float alpha = (glm::acos(cosO));
		//if (crossy.x > 0)
		//	alpha = -alpha;
		//if (alpha< M_PI&& alpha > -M_PI&&glm::abs(alpha)> 0.01)
		//	myGame->shapeTransformation(myGame->xLocalRotate, alpha);
		////y
		//atala = glm::vec3(1, 0, 1);
		//crossy = glm::cross(popOrition * atala, myOrition * atala);
		//cosO = (popOrition.x * myOrition.x + popOrition.z * myOrition.z) / (glm::length(glm::vec2(popOrition.x, popOrition.z))*glm::length(glm::vec2(myOrition.x, myOrition.z)));
		//alpha = (glm::acos(cosO));
		//if (crossy.y > 0)
		//	alpha = -alpha;
		//if (alpha< M_PI&& alpha > -M_PI && glm::abs(alpha) > 0.01)
		//	myGame->shapeTransformation(myGame->yLocalRotate, alpha);


		////z
		//atala = glm::vec3(1, 1, 0);
		//crossy = glm::cross(popOrition * atala, myOrition * atala);
		//cosO = (popOrition.x * myOrition.x + popOrition.y * myOrition.y) / (glm::length(glm::vec2(popOrition.x, popOrition.y))*glm::length(glm::vec2(myOrition.x, myOrition.y)));
		//alpha = (glm::acos(cosO));
		//if (crossy.z > 0)
		//	alpha = -alpha;
		//if (alpha< M_PI&& alpha > -M_PI && glm::abs(alpha) > 0.01)
		//	myGame->shapeTransformation(myGame->zLocalRotate, alpha);
	
}



