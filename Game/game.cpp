#include "game.h"
#include <iostream>
#include <glm/gtx/dual_quaternion.hpp>

static void printMat(const glm::mat4 mat)
{
	std::cout << " matrix:" << std::endl;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
			std::cout << mat[j][i] << " ";
		std::cout << std::endl;
	}
}

static void printMat(const glm::mat3x4 mat)
{
	std::cout << " matrix trans:" << std::endl;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 3; j++)
			std::cout << mat[j][i] << " ";
		std::cout << std::endl;
	}
}

static void printMat(glm::dualquat qat)
{
	std::cout << "qartnion:" << std::endl;

		std::cout << qat.real.x << " " << qat.real.y << " " << qat.real.z << " " << qat.real.w << " " << std::endl;
		std::cout << qat.dual.x << " " << qat.dual.y << " " << qat.dual.z << " " << qat.dual.w << " " << std::endl;
	

	std::cout << std::endl;
	
}


Game::Game() :Scene() { curve = 0; }

Game::Game(glm::vec3 position, float angle, float hwRelation, float near, float far) : Scene(position, angle, hwRelation, near, far)
{
	curve = new Bezier1D();
}

void Game::addShape(int type, int parent, unsigned int mode)
{
	chainParents.push_back(parent);
	if (type != BezierLine && type != BezierSurface)
		shapes.push_back(new Shape(type, mode));
	else
	{
		if (type == BezierLine)
			shapes.push_back(new Shape(curve, 30, 30, false, mode));
		else
			shapes.push_back(new Shape(curve, 30, 30, true, mode));
	}
}

void Game::Init()
{
	addShape(Axis, -1, LINES);
//	addShapeFromFile("../res/objs/torus.obj",-1,TRIANGLES);
//	addShapeCopy(1, -1, TRIANGLES);
	float x = 1.0;
	addShape(BezierSurface, -1, TRIANGLES);
	//curve->MoveControlPoint(0, 0, false, glm::vec4(-1 - x, 1, 0, 0));
	curve->MoveControlPoint(0, 0, false, glm::vec4(3-x, 1, 0, 0));
	curve->MoveControlPoint(0, 1, false, glm::vec4(4-x, 1, 0, 0));
	curve->MoveControlPoint(0, 2, false, glm::vec4(5-x, 1, 0, 0));
	curve->MoveControlPoint(0, 3, false, glm::vec4(6-x, 1, 0, 0));
	

	//addShape(BezierSurface, -1, TRIANGLES);
	//pickedShape = 2;
	//shapeTransformation(xLocalTranslate, 2);
	//shapeTransformation(xGlobalTranslate, 2);
	addShape(BezierSurface, -1, TRIANGLES);
	curve->MoveControlPoint(0, 0, false, glm::vec4(6-x, 1, 0, 0));
	curve->MoveControlPoint(0, 1, false, glm::vec4(7-x, 1, 0, 0));
	curve->MoveControlPoint(0, 2, false, glm::vec4(8-x, 1, 0, 0));
	curve->MoveControlPoint(0, 3, false, glm::vec4(9-x, 1, 0, 0));
	//pickedShape = 3;
	//shapeTransformation(xLocalTranslate, 2);
	//shapeTransformation(xGlobalTranslate, 2);
	addShape(BezierSurface, -1, TRIANGLES);
	curve->MoveControlPoint(0, 0, false, glm::vec4(9 - x, 1, 0, 0));
	curve->MoveControlPoint(0, 1, false, glm::vec4(10 - x, 1, 0, 0));
	curve->MoveControlPoint(0, 2, false, glm::vec4(11 - x, 1, 0, 0));
	curve->MoveControlPoint(0, 3, false, glm::vec4(12 - x, 1, 0, 0));
	//pickedShape = 4;
	//shapeTransformation(xLocalTranslate, 2);
	//shapeTransformation(xGlobalTranslate, 2);
	
//	curve->MoveControlPoint(0, 3, false, glm::vec4(2 - x, 0, 0, 0));
	addShape(BezierSurface, -1, TRIANGLES);
	curve->MoveControlPoint(0, 0, false, glm::vec4(12 - x, 1, 0, 0));
	curve->MoveControlPoint(0, 1, false, glm::vec4(13 - x, 1, 0, 0));
	curve->MoveControlPoint(0, 2, false, glm::vec4(14 - x, 1, 0, 0));
	curve->MoveControlPoint(0, 3, false, glm::vec4(15 - x, 0, 0, 0));
	//pickedShape = 5;
	//shapeTransformation(xLocalTranslate, 2);
	//shapeTransformation(xGlobalTranslate, 2);
	addShape(BezierSurface, -1, TRIANGLES);
	//translate all scene away from camera
	setParent(2, 1);
	setParent(3, 2);
	setParent(4, 3);
	setParent(5, 4);
	myTranslate(glm::vec3(0, 0, -20), 0);

	pickedShape = 0;

	shapeTransformation(yScale, 10);
	shapeTransformation(xScale, 10);
	shapeTransformation(zScale, 10);


	ReadPixel();





	pickedShape = -1;
	Activate();
}
int count = 0;
void Game::Update(const glm::mat4 &MVP, const glm::mat4 &myNormal, const glm::mat4 &preNormal, const glm::mat4 &nextNormal, const int  shaderIndx, const glm::mat4 &scale)
{

	glm::mat3x4 pretranlation(glm::transpose(preNormal));
	glm::mat3x4 nexttranlation(glm::transpose(nextNormal));
	glm::dualquat prequat= glm::dualquat(glm::dualquat_cast(pretranlation));
	glm::dualquat nextquat = glm::dualquat(glm::dualquat_cast(nexttranlation));

	glm::mat3x4 tranlation(glm::transpose(myNormal));//glm::transpose(shapes.at(pickedShape)->makeTrans()));
	glm::dualquat myquat(glm::dualquat_cast(tranlation));


	Shape* shape = shapes.at(pickedShape);
	Shader *s = shaders[shaderIndx];
	//glm::vec3 wight = s.get
	int r = ((pickedShape + 1) & 0x000000FF) >> 0;
	int g = ((pickedShape + 1) & 0x0000FF00) >> 8;
	int b = ((pickedShape + 1) & 0x00FF0000) >> 16;
	s->Bind();
	s->SetUniformMat4f("MVP", MVP);
	s->SetUniformMat4f("Normal", myNormal);
	s->SetUniformMat4f("preNormal", preNormal);
	s->SetUniformMat4f("scale", scale);
	s->SetUniform4f("lightDirection", 0.0f, 0.0f, -1.0f, 0.0f);
	s->SetUniform4f("prequaternion", prequat.real.x, prequat.real.y, prequat.real.z, prequat.real.w);
	s->SetUniform4f("myquaternion", myquat.real.x, myquat.real.y, myquat.real.z, myquat.real.w);
	s->SetUniform4f("nextquaternion", nextquat.real.x, nextquat.real.y, nextquat.real.z, nextquat.real.w);

	s->SetUniform4f("prequaterniondual", prequat.dual.x, prequat.dual.y, prequat.dual.z, prequat.dual.w);
	s->SetUniform4f("myquaterniondual", myquat.dual.x, myquat.dual.y, myquat.dual.z, myquat.dual.w);
	s->SetUniform4f("nextquaterniondual", nextquat.dual.x, nextquat.dual.y, nextquat.dual.z, nextquat.dual.w);
	//s->SetUniform4f("prequaternion", prequat.real.w, prequat.real.x, prequat.real.y, prequat.real.z);
	//s->SetUniform4f("myquaternion", myquat.real.w, myquat.real.x, myquat.real.y, myquat.real.z);
	//s->SetUniform4f("nextquaternion", nextquat.real.w, nextquat.real.x, nextquat.real.y, nextquat.real.z);

	//s->SetUniform4f("prequaterniondual", prequat.dual.w, prequat.dual.x, prequat.dual.z, prequat.dual.z);
	//s->SetUniform4f("myquaterniondual", myquat.dual.w, myquat.dual.x, myquat.dual.y, myquat.dual.z);
	//s->SetUniform4f("nextquaterniondual", nextquat.dual.w, nextquat.dual.x, nextquat.dual.y, nextquat.dual.z);
	if (shaderIndx == 0)
		s->SetUniform4f("lightColor", r / 255.0f, g / 255.0f, b / 255.0f, 1.0f);
	else
		s->SetUniform4f("lightColor", 0.1f, 0.8f, 0.7f, 1.0f);
	s->Unbind();

}

void Game::WhenRotate()
{
	//if (pickedShape >= 0)
	//	printMat(GetShapeTransformation());
}


void Game::WhenTranslate()
{
	if (pickedShape >= 0)
	{
		glm::vec4 pos = GetShapeTransformation()*glm::vec4(0, 0, 0, 1);
		//	std::cout<<"( "<<pos.x<<", "<<pos.y<<", "<<pos.z<<")"<<std::endl;
	}
}

void Game::Motion()
{
	if (false)
	{
		ChainMove(1, zLocalRotate, 0.1);
		ChainMove(1, xLocalTranslate, 0.0001);
		ChainMove(2, xLocalTranslate, 0.0001);
	//	int p = pickedShape;
	//	pickedShape = 2;
	//	shapeTransformation(zLocalRotate, 0.75);
	//	pickedShape = p;
	}
}

Game::~Game(void)
{
	delete curve;
}
