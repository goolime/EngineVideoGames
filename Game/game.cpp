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


	curve->MoveControlPoint(0, 0, false, glm::vec4(-1.5, 1, 0, 0));
	addShape(BezierSurface, -1, TRIANGLES);
	addShape(BezierSurface, -1, TRIANGLES);

	pickedShape = 2;
	shapeTransformation(xGlobalTranslate, 3);
	pickedShape = 3;
	shapeTransformation(xGlobalTranslate, 6);

	addShape(Cube, -1, LINES);
	pickedShape = 4;
	shapeTransformation(xLocalTranslate, 1);
	shapeTransformation(yLocalTranslate, 2);
	shapeTransformation(yScale, 0.010);
	shapeTransformation(xScale, 0.010);
	shapeTransformation(zScale, 0.010);
	chainParents.at(4) = 1;
	cameras[0]->target = shapes[1];
	cameras[0]->me = shapes[4];
	cameras[0]->Update();
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
	
	createshapes(&reader, lvl1, Cube, lvl1 / lvl0 - 1);
	createshapes(&reader, lvl2, Cube, lvl2 / lvl0 - 1);
	createshapes(&reader, apple, Octahedron, 1);
	//createshapes(&reader,)


	//glm::vec4 pos = reader.getobject(apple, glm::vec2(7, 10));
	//std::cout << pos.x << "," << pos.y << "," << pos.z << "," << pos.w << std::endl;
	//// Get the data from CSV File
	//std::vector < std::vector<std::string> >dataList = reader.getData();
	//// Print the content of row by row on screen
	//for (std::vector < std::string >vec : dataList)
	//{
	//	for (std::string data : vec)
	//	{
	//		std::cout << data << " , ";
	//	}
	//	std::cout << std::endl;
	//}
	

	Activate();
}

void Game::createshapes(CSVReader* reader, int type, int shapetype, int lvl)
{
	std::vector<obj> myobj = reader->getallobjects(type);
	for each (obj o in myobj)
	{
		int p = shapes.size();
		std::cout << o.postions.x << "," << o.postions.y << "," << o.postions.z << "," << o.postions.w << std::endl;
		addShape(shapetype, -1, TRIANGLES);
		pickedShape = p;
		shapeTransformation(xScale, o.postions.z); //pos.x = startx \ pos.y = startz\ pos.z = sizex\ pos.w = sizez 
		shapeTransformation(zScale, o.postions.w);
		shapeTransformation(yGlobalTranslate, lvl);
		shapeTransformation(xGlobalTranslate, o.postions.x / 4);
		shapeTransformation(zGlobalTranslate, -o.postions.y / 4);
		//shapes.at(pickedShape).
		//change color
	}
	//cameras[0]->Update();
	pickedShape = -1;
}


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
		cameras[0]->Update();
		mySnake->move(this);

	}
	pickedShape = p;
}

void Game::ChainMove(int pick, int type, float amount) {
int p = pickedShape;
	pickedShape = 1;
	shapeTransformation(type, amount);
	
}

Game::~Game(void)
{
	delete curve;
}