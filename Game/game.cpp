#include "game.h"
#include <iostream>
#include <math.h>
#define PI 3.14159265

static void printMat(const glm::mat4 mat)
{
	std::cout<<" matrix:"<<std::endl;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
			std::cout<< mat[j][i]<<" ";
		std::cout<<std::endl;
	}
}


Game::Game():Scene(){curve = 0;}

Game::Game(glm::vec3 position,float angle,float hwRelation,float near, float far) : Scene(position,angle,hwRelation,near,far)
{ 
	curve = new Bezier1D();
}

void Game::addShape(int type,int parent,unsigned int mode)
{
		chainParents.push_back(parent);
		if(type!=BezierLine && type!=BezierSurface)
			shapes.push_back(new Shape(type,mode));
		else
		{
			if(type == BezierLine)
				shapes.push_back(new Shape(curve,30,30,false,mode));
			else
				shapes.push_back(new Shape(curve,30,30,true,mode));
		}
}

void Game::Init()
{
	addShape(Axis, -1, LINES);
	addShape(Octahedron, -1, TRIANGLES);
	//addShape(Octahedron, -1, TRIANGLES);
	addShapeFromFile("../res/objs/torus.obj",-1,TRIANGLES);
	//addShape(Tethrahedron, -1, TRIANGLES);
	//addShapeCopy(3,2,LINE_LOOP);


	//translate all scene away from camera
	myTranslate(glm::vec3(0, 0, -20), 0);

	pickedShape = 0;

	shapeTransformation(yScale, 10);
	shapeTransformation(xScale, 10);
	shapeTransformation(zScale, 10);


	ReadPixel();

	pickedShape = 1;
	shapeTransformation(xGlobalTranslate, -3);
	//shapeTransformation(zLocalRotate, 30);
	//shapeTransformation(xLocalRotate, 10);
	//shapeTransformation(xLocalRotate, 10);
	//shapeTransformation(zLocalRotate, 37);



	pickedShape = 2;


	shapeTransformation(xGlobalTranslate, 5);
	//shapeTransformation(zLocalRotate, 30);
	//shapeTransformation(xLocalRotate, 10);
	//shapeTransformation(zLocalRotate, 100);
	shapeTransformation(yScale, 0.10f);
	shapeTransformation(xScale, 0.10f);
	shapeTransformation(zScale, 0.10f);



	ShowBoundingBox(1);
	ShowBoundingBox(2);


	addShape(Octahedron, -1, TRIANGLES);
	addShape(Octahedron, -1, TRIANGLES);
	addShape(Octahedron, -1, TRIANGLES);
	addShape(Octahedron, -1, TRIANGLES);
	pickedShape = -1;




	Activate();
}

	void Game::Update(const glm::mat4 &MVP,const glm::mat4 &Normal,const int  shaderIndx)
{
	Shader *s = shaders[shaderIndx];
	int r = ((pickedShape+1) & 0x000000FF) >>  0;
	int g = ((pickedShape+1) & 0x0000FF00) >>  8;
	int b = ((pickedShape+1) & 0x00FF0000) >> 16;
	s->Bind();
	s->SetUniformMat4f("MVP", MVP);
	s->SetUniformMat4f("Normal", Normal);
	s->SetUniform4f("lightDirection", 0.0f , 0.0f, -1.0f, 0.0f);
	if(shaderIndx == 0)
		s->SetUniform4f("lightColor",r/255.0f, g/255.0f, b/255.0f,1.0f);
	else 
		s->SetUniform4f("lightColor",0.1f,0.8f,0.7f,1.0f);
	s->Unbind();
}

void Game::ShowBoundingBox(int shape_num) {
	int p = pickedShape;
	addShape(Cube, -1, LINE_LOOP);
	pickedShape = shapes.size() - 1;
	MeshConstructor* mShape = shapes.at(shape_num)->mesh;
	B_Node* bNode = mShape->boundingTree;
	BoundingBox* bb = &bNode->bb;
	bb->updateValues(shapes.at(shape_num)->makeTrans(), shapes.at(shape_num)->makeScale());
	shapeTransformation(xGlobalTranslate, 0);
	shapeTransformation(yGlobalTranslate, 0);
	shapeTransformation(zGlobalTranslate, 0);

	float rotate = atan2(bb->newzInit.z, bb->newzInit.y) * 180 / PI - 90;
	//std::cout << -rotate << "\n";
	shapeTransformation(xLocalRotate, -rotate);

	//we never rotate in y
	rotate = atan2(-1 * bb->newzInit.x, sqrt(bb->newzInit.z *bb->newzInit.z + bb->newzInit.y*bb->newzInit.y)) * 180 / PI;
	//std::cout << rotate << "\n";
	shapeTransformation(yLocalRotate, rotate);

	rotate = atan2(bb->newxInit.x, bb->newyInit.x) * 180 / PI - 90;
	//std::cout << -rotate << "\n";
	shapeTransformation(zLocalRotate, -rotate);

	shapeTransformation(xGlobalTranslate, bb->newCenter.x);
	shapeTransformation(yGlobalTranslate, bb->newCenter.y);
	shapeTransformation(zGlobalTranslate, bb->newCenter.z);

	shapeTransformation(xScale, bb->newsize.x);
	shapeTransformation(yScale, bb->newsize.y);
	shapeTransformation(zScale, bb->newsize.z);
	pickedShape = p;
}


void Game::ShowBoundingBox(int shape, BoundingBox* bb) {
	int p = pickedShape;
	addShape(Cube, -1, LINE_LOOP);
	pickedShape = shapes.size() - 1;
	
	bb->updateValues(shapes.at(shape)->makeTrans(), shapes.at(shape)->makeScale());
	shapeTransformation(xGlobalTranslate, 0);
	shapeTransformation(yGlobalTranslate, 0);
	shapeTransformation(zGlobalTranslate, 0);

	float rotate = atan2(bb->newzInit.z, bb->newzInit.y) * 180 / PI - 90;
	//std::cout << -rotate << "\n";
	shapeTransformation(xLocalRotate, -rotate);

	//we never rotate in y
	rotate = atan2(-1 * bb->newzInit.x, sqrt(bb->newzInit.z *bb->newzInit.z + bb->newzInit.y*bb->newzInit.y)) * 180 / PI;
	//std::cout << rotate << "\n";
	shapeTransformation(yLocalRotate, rotate);

	rotate = atan2(bb->newxInit.x, bb->newyInit.x) * 180 / PI - 90;
	//std::cout << -rotate << "\n";
	shapeTransformation(zLocalRotate, -rotate);

	shapeTransformation(xGlobalTranslate, bb->newCenter.x);
	shapeTransformation(yGlobalTranslate, bb->newCenter.y);
	shapeTransformation(zGlobalTranslate, bb->newCenter.z);
	shapeTransformation(yScale, bb->newsize.y);
	shapeTransformation(xScale, bb->newsize.x);
	shapeTransformation(zScale, bb->newsize.z);
	pickedShape = p;
}

void Game::updateBoundings(int shape_num,int boxNum) {
	int p = pickedShape;
	pickedShape = boxNum;
	shapes.at(pickedShape) = new  Shape(Cube, LINE_LOOP);
	MeshConstructor* mShape = shapes.at(shape_num)->mesh;
	B_Node* bNode = mShape->boundingTree;
	BoundingBox* bb = &bNode->bb;
	bb->updateValues(shapes.at(shape_num)->makeTrans(), shapes.at(shape_num)->makeScale());
	shapeTransformation(xGlobalTranslate, 0);
	shapeTransformation(yGlobalTranslate, 0);
	shapeTransformation(zGlobalTranslate, 0);

	float rotate = atan2(bb->newzInit.z, bb->newzInit.y) * 180 / PI - 90;
	//std::cout << -rotate << "\n";
	shapeTransformation(xLocalRotate, -rotate);

	//we never rotate in y
	rotate = atan2(-1 * bb->newzInit.x, sqrt(bb->newzInit.z *bb->newzInit.z + bb->newzInit.y*bb->newzInit.y)) * 180 / PI;
	//std::cout << rotate << "\n";
	shapeTransformation(yLocalRotate, rotate);

	rotate = atan2(bb->newxInit.x, bb->newyInit.x) * 180 / PI - 90;
	//std::cout << -rotate << "\n";
	shapeTransformation(zLocalRotate, -rotate);

	shapeTransformation(xGlobalTranslate, bb->newCenter.x);
	shapeTransformation(yGlobalTranslate, bb->newCenter.y);
	shapeTransformation(zGlobalTranslate, bb->newCenter.z);
	shapeTransformation(yScale, bb->newsize.y);
	shapeTransformation(xScale, bb->newsize.x);
	shapeTransformation(zScale, bb->newsize.z);
	pickedShape = p;
}

void Game::updateBoundings(int shape_num, int boxNum,BoundingBox* bb) {
	int p = pickedShape;
	pickedShape = boxNum;
	shapes.at(pickedShape) = new  Shape(Cube, LINE_LOOP);
	bb->updateValues(shapes.at(shape_num)->makeTrans(), shapes.at(shape_num)->makeScale());
	shapeTransformation(xGlobalTranslate, 0);
	shapeTransformation(yGlobalTranslate, 0);
	shapeTransformation(zGlobalTranslate, 0);

	float rotate = atan2(bb->newzInit.z, bb->newzInit.y) * 180 / PI - 90;
	//std::cout << -rotate << "\n";
	shapeTransformation(xLocalRotate, -rotate);

	//we never rotate in y
	rotate = atan2(-1 * bb->newzInit.x, sqrt(bb->newzInit.z *bb->newzInit.z + bb->newzInit.y*bb->newzInit.y)) * 180 / PI;
	//std::cout << rotate << "\n";
	shapeTransformation(yLocalRotate, rotate);

	rotate = atan2(bb->newxInit.x, bb->newyInit.x) * 180 / PI - 90;
	//std::cout << -rotate << "\n";
	shapeTransformation(zLocalRotate, -rotate);

	shapeTransformation(xGlobalTranslate, bb->newCenter.x);
	shapeTransformation(yGlobalTranslate, bb->newCenter.y);
	shapeTransformation(zGlobalTranslate, bb->newCenter.z);
	shapeTransformation(yScale, bb->newsize.y);
	shapeTransformation(xScale, bb->newsize.x);
	shapeTransformation(zScale, bb->newsize.z);
	pickedShape = p;
}

void Game::WhenRotate()
{
	//if(pickedShape>=0)
	//	printMat(GetShapeTransformation());
}

void Game::WhenTranslate()
{
	if(pickedShape>=0)
	{
		glm::vec4 pos = GetShapeTransformation()*glm::vec4(0,0,0,1);
	//	std::cout<<"( "<<pos.x<<", "<<pos.y<<", "<<pos.z<<")"<<std::endl;
	}
}
int counter = 0;
bool moving = true;
void Game::Motion()
{
	if (isActive)
	{
		if (moving) {
			int p = pickedShape;
			pickedShape = 2;
			shapeTransformation(xLocalRotate, 0.3);
			
			shapeTransformation(zLocalRotate,0.3);
			pickedShape = 1;
			shapeTransformation(xLocalTranslate, 0.005);
			counter++;
			if (counter % 10 == 1)
			{
				std::pair<BoundingBox*, BoundingBox*> pair = shapes.at(1)->checkColsion(shapes.at(2));

				if (pair.first != NULL)
				{

					pickedShape = 3;
					shapeTransformation(xGlobalTranslate, 15);
					pickedShape = 4;
					shapeTransformation(xGlobalTranslate, 15);
					ShowBoundingBox(1, pair.first);
					std::cout << "found collsion\n";
					if(pair.second == NULL)
						std::cout << "error\n";
					else
					{
						ShowBoundingBox(2, pair.second);
					}
					
					moving = false;

				}
			}
			else
		

				
				updateBoundings(1, 3);
				updateBoundings(2, 4);
				MeshConstructor* mShape = shapes.at(2)->mesh;
				B_Node* bNode = mShape->boundingTree;
				BoundingBox* bb = &bNode->right->bb;
				updateBoundings(2, 5,bb);
				 mShape = shapes.at(2)->mesh;
				 bNode = mShape->boundingTree;
				 bb = &bNode->right->right->right->right->bb;
				updateBoundings(2, 6, bb);
				 mShape = shapes.at(2)->mesh;
				 bNode = mShape->boundingTree;
				 bb = &bNode->right->right->bb;
				updateBoundings(2, 7, bb);
				 mShape = shapes.at(2)->mesh;
				 bNode = mShape->boundingTree;
				 bb = &bNode->right->right->right->bb;
				updateBoundings(2, 8, bb);
				counter = 0;
			
			pickedShape = p;
		}
	}
}

Game::~Game(void)
{
	delete curve;
}
