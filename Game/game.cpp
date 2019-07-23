#include "game.h"

#include <iostream>

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
	float x = 1.0;
	addShape(BezierSurface, -1, TRIANGLES);
	pickedShape = 1;
	shapeTransformation(yGlobalTranslate, 1);


	curve->MoveControlPoint(0, 0, false, glm::vec4(-1.5, 1, 0, 0));
	addShape(BezierSurface, -1, TRIANGLES);
	addShape(BezierSurface, -1, TRIANGLES);

	pickedShape = 2;
	shapeTransformation(xGlobalTranslate, 3);
	shapeTransformation(yGlobalTranslate, 1);
	pickedShape = 3;
	shapeTransformation(xGlobalTranslate, 6);
	shapeTransformation(yGlobalTranslate, 1);

	addShape(Cube, -1, LINES);
	pickedShape = 4;
	shapeTransformation(xLocalTranslate, 2);
	shapeTransformation(yLocalTranslate, 2);
	shapeTransformation(yScale, 0.010);
	shapeTransformation(xScale, 0.010);
	shapeTransformation(zScale, 0.010);
	chainParents.at(4) = 1;

	addShape(Cube, -1, LINES);
	pickedShape = 5;
	shapeTransformation(xLocalTranslate, 1);
	shapeTransformation(yLocalTranslate, 30);
	shapeTransformation(yScale, 0.010);
	shapeTransformation(xScale, 0.010);
	shapeTransformation(zScale, 0.010);
	chainParents.at(5) = 1;
	//cameras[0]->target = shapes[1];
	//cameras[0]->me = shapes[4];
	//translate all scene away from camera
	//myTranslate(glm::vec3(0, 0, -20), 0);

	pickedShape = 0;

	shapeTransformation(yScale, 10);
	shapeTransformation(xScale, 10);
	shapeTransformation(zScale, 10);




	pickedShape = -1;
	mySnake = new Snake(shapes.at(1), 1);
	mySnake->addPart(shapes.at(2), 2);
	mySnake->addPart(shapes.at(3), 3);
	pickedShape = 1;
	//shapeTransformation(xLocalTranslate, -5);


	pickedShape = -1;
	CSVReader reader("csvMap1.csv");
	createshapes(&reader, apple, Octahedron);
	createshapes(&reader, lvl1, Cube);
	createshapes(&reader, lvl2, Cube);
	//walls

	//{
	//	//floor
	//	pickedShape = shapes.size();
	//	addShape(Cube, -1, TRIANGLES);
	//	shapeTransformation(zLocalTranslate, (float)reader.z);
	//	shapeTransformation(xLocalTranslate, (float)reader.x);
	//	shapeTransformation(yLocalTranslate, -1 + 0.4);
	//	shapeTransformation(xScale, reader.x);
	//	shapeTransformation(zScale, reader.z);
	//	//wall base
	//	pickedShape = shapes.size();
	//	addShape(Cube, -1, TRIANGLES);
	//	//shapeTransformation(zLocalTranslate, (float)reader.z);
	//	shapeTransformation(xLocalTranslate, (float)reader.x);
	//	shapeTransformation(zLocalTranslate, -1);
	//	shapeTransformation(yLocalTranslate, 4);
	//	shapeTransformation(yLocalTranslate, 0.4);
	//	shapeTransformation(xScale, reader.x);
	//	shapeTransformation(yScale, 6);
	//	//shapeTransformation(zScale, reader.z);
	//	//wall right
	//	pickedShape = shapes.size();
	//	addShape(Cube, -1, TRIANGLES);
	//	shapeTransformation(zLocalTranslate, (float)reader.z);
	//	//shapeTransformation(xLocalTranslate, (float)reader.x);
	//	shapeTransformation(xLocalTranslate, -1);
	//	//shapeTransformation(xScale, reader.x);
	//	shapeTransformation(zScale, reader.z);
	//	shapeTransformation(yLocalTranslate, 4);
	//	shapeTransformation(yLocalTranslate, 0.4);
	//	shapeTransformation(yScale, 6);
	//	//wall left
	//	pickedShape = shapes.size();
	//	addShape(Cube, -1, TRIANGLES);
	//	shapeTransformation(zLocalTranslate, (float)reader.z);
	//	shapeTransformation(xLocalTranslate, 2 * (float)reader.x);
	//	shapeTransformation(xLocalTranslate, -1);
	//	//shapeTransformation(xScale, reader.x);
	//	shapeTransformation(zScale, reader.z);
	//	shapeTransformation(yLocalTranslate, 4);
	//	shapeTransformation(yLocalTranslate, 0.4);
	//	shapeTransformation(yScale, 6);
	//	//wall far
	//	pickedShape = shapes.size();
	//	addShape(Cube, -1, TRIANGLES);
	//	shapeTransformation(zLocalTranslate, 2*(float)reader.z);
	//	shapeTransformation(xLocalTranslate, (float)reader.x);
	//	shapeTransformation(zLocalTranslate, -1);
	//	shapeTransformation(yLocalTranslate, 4);
	//	shapeTransformation(yLocalTranslate, 0.4);
	//	shapeTransformation(xScale, reader.x);
	//	shapeTransformation(yScale, 6);
	//	//shapeTransformation(zScale, reader.z);
	//}

	pickedShape = 1;
	shapeTransformation(yLocalRotate, 90);
	shapeTransformation(xGlobalTranslate, 20);
	shapeTransformation(zGlobalTranslate, 10);

	Activate();
}

void Game::createshapes(CSVReader* reader, int type, int shapetype)
{
	std::vector<obj> myobj = reader->getallobjects(type);
	for each (obj o in myobj)
	{
		int p = 0;
		switch (type)
		{
		case ramp:
		case wall:
		case lvl0:
		case lvl1:
		case lvl2:
		case lvl3:
			 p = shapes.size();
			//std::cout << o.postions.x << "," << o.postions.y << "," << o.postions.z << "," << o.postions.w << std::endl;
			addShape(shapetype, -1, TRIANGLES);
			pickedShape = p;	
			shapeTransformation(yGlobalTranslate, 2 * o.lvl + 0.4 - 1);
			shapeTransformation(xGlobalTranslate,	2*o.postions.x+o.postions.z);
			shapeTransformation(zGlobalTranslate, 2*o.postions.y+o.postions.w);
			shapeTransformation(xScale, o.postions.z); //pos.x = startx \ pos.y = startz\ pos.z = sizex\ pos.w = sizez 
			shapeTransformation(zScale, o.postions.w);

			break;
		case apple:
		case Gate:
			for (int i = 0; i < o.postions.z; i++)
			{
				for (int j = 0; j < o.postions.w; j++)
				{
					 p = shapes.size();
					//std::cout << o.postions.x << "," << o.postions.y << "," << o.postions.z << "," << o.postions.w << std::endl;
					addShape(shapetype, -1, TRIANGLES);
					pickedShape = p;

					shapeTransformation(yLocalTranslate, 2*o.lvl + 1);
					shapeTransformation(xLocalTranslate, 2*(o.postions.x + i)+1);
					shapeTransformation(zLocalTranslate, 2*(o.postions.y + j)+1);
					shapeTransformation(xScale, 0.25);
					shapeTransformation(zScale, 0.25);
					shapeTransformation(yScale, 0.25);

				}
			}

			break;
		case snake:
		case snakebody:
			break;
		default:
			break;
		}

		//shapes.at(pickedShape).
		//change color
	}
	//cameras[0]->Update();
	pickedShape = -1;
}

void Game::ShowBoundingBox()
{}


void Game::Update(const glm::mat4 &MVP, const glm::mat4 &Normal, const int  shaderIndx)
{
	Shader *s = shaders[shaderIndx];
	int r = ((pickedShape + 1) & 0x000000FF) >> 0;
	int g = ((pickedShape + 1) & 0x0000FF00) >> 8;
	int b = ((pickedShape + 1) & 0x00FF0000) >> 16;
	s->Bind();
	s->SetUniformMat4f("MVP", MVP);
	s->SetUniformMat4f("Normal", Normal);
	s->SetUniform4f("lightDirection", 0.0f, 0.0f, -1.0f, 0.0f);
	if (shaderIndx == 0)
		s->SetUniform4f("lightColor", r / 255.0f, g / 255.0f, b / 255.0f, 1.0f);
	else
		s->SetUniform4f("lightColor", 0.1f, 0.8f, 0.7f, 1.0f);
	s->Unbind();
}

void Game::WhenRotate()
{
	//if(pickedShape>=0)
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
float count = 0;
void Game::Motion()
{

	//cameras[0]->MoveForward(0.01);

	int p = pickedShape;
	if (isActive)
	{
		//shapeTransformation(yLocalRotate, 0.2);
		//myTranslate(glm::vec3(0, 0, -20- count ), 0);
		//count += 0.0001;
		pickedShape = 1;
		//shapeTransformation(xLocalTranslate, -0.005);
		//shapeTransformation(xLocalTranslate, -0.001);
		//shapeTransformation(yLocalRotate, 0.1);
		cameras[0]->Update(shapes[1],shapes[Camrasstart + Curentcamera],glm::vec3(0,1.5,0),glm::vec3(0,0,0));
		mySnake->move(this);

	}
	pickedShape = p;
}

void Game::MoveSnake(int pick, int type, float amount) {
	
int p = pickedShape;
	pickedShape = pick;
	shapeTransformation(type, amount);
	pickedShape = p;
}

Game::~Game(void)
{
	delete curve;
}