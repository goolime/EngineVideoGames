#include "game.h"
#include <math.h>
#include <iostream>
#include <glm/gtx/dual_quaternion.hpp>
#include <Windows.h>
#include <mmsystem.h>


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

Game::Game(glm::vec3 position, float angle, float hwRelation, float dnear, float dfar) : Scene(position, angle, hwRelation, dnear, dfar)
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


	//textures
	this->AddTexture("../res/textures/box0.bmp");
	this->AddTexture("../res/textures/bricks.jpg");
	this->AddTexture("../res/textures/grass.bmp");
	this->AddTexture("../res/textures/plane.png");
	this->AddTexture("../res/textures/snake.jpg");
	this->AddTexture("../res/textures/snake1.png");

	this->AddTexture("../res/textures/apple.jpg");
	this->AddTexture("../res/textures/flor.jpg");
	this->AddTexture("../res/textures/goal.jpg");
	this->AddTexture("../res/textures/mine.jpg");
	this->AddTexture("../res/textures/skin.jpg");

	pickedShape = shapes.size();
	addShape(Axis, -1, LINES);
	shapeTransformation(yScale, 10);
	shapeTransformation(xScale, 10);
	shapeTransformation(zScale, 10);


	//pickedShape = shapes.size();
	//addShape(Cube, -1, TRIANGLES);
	//shapes[pickedShape]->Hide();
	//shapeTransformation(yScale, 0.10);
	//shapeTransformation(xScale, 0.10);
	//shapeTransformation(zScale, 0.10);



	//snake body
	{
		pickedShape = shapes.size();
		addShape(BezierSurface, -1, TRIANGLES);
		shapes[pickedShape]->SetTexture(box0);
		shapes[pickedShape]->shaderID = 3;
		shapeTransformation(yGlobalTranslate, 1);
		//shapeTransformation(xGlobalTranslate, 3 );
		mySnake = new Snake(shapes.at(pickedShape), pickedShape);
		shapes[pickedShape]->name = "snake head";
		//curve->MoveControlPoint(0, 0, false, glm::vec4(-1.5, 1, 0, 0));
		for (int i = 0; i < 5; i++) {
			curve->MoveControlPoint(0, 0, false, glm::vec4(3 + 3 * i - 1.5, 1, 0, 0));
			curve->MoveControlPoint(0, 1, false, glm::vec4(4 + 3 * i - 1.5, 2, 0, 0));
			curve->MoveControlPoint(0, 2, false, glm::vec4(5 + 3 * i - 1.5, 0.5, 0, 0));
			if (i == 3)
				curve->MoveControlPoint(0, 3, false, glm::vec4(6 + 3 * i - 1.5, 0, 0, 0));
			else
				curve->MoveControlPoint(0, 3, false, glm::vec4(6 + 3 * i - 1.5, 1, 0, 0));
			shapes[pickedShape]->shaderID = 3;
			pickedShape = shapes.size();
			addShape(BezierSurface, -1, TRIANGLES);
			shapes[pickedShape]->SetTexture(box0);
			//	shapeTransformation(xGlobalTranslate, 3 * i);
			shapeTransformation(yGlobalTranslate, 1);
			mySnake->addPart(shapes.at(pickedShape), pickedShape);

			shapes[pickedShape]->name = "snake body";
		}

	}
	//camera
	setCamera();
	//translate all scene away from camera
	//myTranslate(glm::vec3(0, 0, -20), 0);



	reader = new CSVReader("csvMap.csv");
	createshapes(reader, apple, Octahedron);
	createshapes(reader, rampN, Minsara);
	createshapes(reader, rampS, Minsara);
	createshapes(reader, rampW, Minsara);
	createshapes(reader, rampE, Minsara);
	createshapes(reader, wall, Cube);
	createshapes(reader, apple, Octahedron);
	createshapes(reader, mine, Octahedron);
	createshapes(reader, goal, Octahedron);
	//add levels only after adding other shapes
	createshapes(reader, lvl1, Cube);
	createshapes(reader, lvl2, Cube);
	createshapes(reader, lvl3, Cube);
	//walls
	creatWalls();



	pickedShape = 1;
	shapeTransformation(xGlobalTranslate, 40);
	shapeTransformation(zGlobalTranslate, 10);
	shapeTransformation(yLocalRotate, 90);

	//updateBoundings(1, 4);

	Activate();
}

void Game::createshapes(CSVReader* reader, int type, int shapetype)
{
	std::vector<obj> myobj = reader->getallobjects(type);
	for each (obj o in myobj)
	{
		int turn = 0;
		int p = 0;
		switch (type)
		{
		case rampW:
			turn++;
		case rampN:
			turn++;
		case rampE:
			turn++;
		case rampS:
		{
			p = shapes.size();
			//std::cout << o.postions.x << "," << o.postions.y << "," << o.postions.z << "," << o.postions.w << std::endl;
			addShape(shapetype, -1, TRIANGLES);
			pickedShape = p;
			shapeTransformation(yGlobalTranslate, 2 * o.lvl + 0.4 + 1); // the cube is size 2x2x2 so that is y we place it in pos 2*o.pos
			shapeTransformation(xGlobalTranslate, 2 * o.postions.x + o.postions.z - 3);
			shapeTransformation(zGlobalTranslate, 2 * o.postions.y + o.postions.w);
			if (type == rampE || type == rampW)
			{
				shapeTransformation(xScale, o.postions.w + 1); // +1 for perfect feat
				shapeTransformation(zScale, o.postions.z);
			}
			else
			{
				shapeTransformation(xScale, o.postions.z); //pos.x = startx \ pos.y = startz\ pos.z = sizex\ pos.w = sizez 
				shapeTransformation(zScale, o.postions.w + 1); // +1 for perfect feat
			}
			shapeTransformation(yLocalRotate, 90 * turn);

		}

		shapes[pickedShape]->SetTexture(grass);
		// collstion ------
		{

			for (int i = 0; i < o.postions.z; i += 2)
			{
				for (int j = 0; j <= o.postions.w; j += 2)
				{
					p = shapes.size();
					//std::cout << o.postions.x << "," << o.postions.y << "," << o.postions.z << "," << o.postions.w << std::endl;
					addShape(Cube, -1, TRIANGLES);
					pickedShape = p;
					shapeTransformation(yGlobalTranslate, 2 * o.lvl + 0.4 + 1); // the cube is size 2x2x2 so that is y we place it in pos 2*o.pos
					shapeTransformation(xGlobalTranslate, 2 * o.postions.x + 2*i - 2);
					shapeTransformation(zGlobalTranslate, 2 * o.postions.y +2*j);
					//if (type == rampE || type == rampW)
					//{
					//	shapeTransformation(xScale, o.postions.w + 1); // +1 for perfect feat
					//	shapeTransformation(zScale, o.postions.z);
					//}
					//else
					//{
					//	shapeTransformation(xScale, o.postions.z); //pos.x = startx \ pos.y = startz\ pos.z = sizex\ pos.w = sizez 
					//	shapeTransformation(zScale, o.postions.w + 1); // +1 for perfect feat
					//}

					shapes[pickedShape]->SetTexture(plane);
					Sramps.push_back(shapes[pickedShape]);
					//shapes[pickedShape]->Hide();
				}
			}
		}
		break;
		case wall:
			for (int i = 0; i < o.postions.z; i += 4)
			{
				for (int j = 0; j <= o.postions.w; j += 4)
				{
					p = shapes.size();
					addShape(shapetype, -1, TRIANGLES);
					pickedShape = p;
					shapeTransformation(yGlobalTranslate, 2*o.lvl + 0.4 +1);
					shapeTransformation(xGlobalTranslate, 2 * o.postions.x +2*i - 2);
					shapeTransformation(zGlobalTranslate, 2 * o.postions.y + 2*j);
					//shapeTransformation(xScale, o.postions.z); //pos.x = startx \ pos.y = startz\ pos.z = sizex\ pos.w = sizez 
					//shapeTransformation(zScale, o.postions.w + 1); 
					shapes[pickedShape]->SetTexture(-1);
					Swalls.push_back(shapes[pickedShape]);

					shapes[pickedShape]->SetTexture(plane);
				}
			}
			break;
		case lvl0:
		case lvl1:
		case lvl2:
		case lvl3:
			p = shapes.size();
			//std::cout << o.postions.x << "," << o.postions.y << "," << o.postions.z << "," << o.postions.w << std::endl;
			addShape(shapetype, -1, TRIANGLES);
			pickedShape = p;
			shapeTransformation(yGlobalTranslate, 2 * o.lvl + 0.4 - 1); // the cube is size 2x2x2 so that is y we place it in pos 2*o.pos
			shapeTransformation(xGlobalTranslate, 2 * o.postions.x + o.postions.z - 2);
			shapeTransformation(zGlobalTranslate, 2 * o.postions.y + o.postions.w);
			shapeTransformation(xScale, o.postions.z); //pos.x = startx \ pos.y = startz\ pos.z = sizex\ pos.w = sizez 
			shapeTransformation(zScale, o.postions.w + 1); // +1 for perfect feat

			shapes[pickedShape]->SetTexture(bricks);


			break;
		case apple:

			//for (int i = 0; i < o.postions.z; i++)
			//{
			//	for (int j = 0; j < o.postions.w; j++)
			//	{
			p = shapes.size();
			//std::cout << o.postions.x << "," << o.postions.y << "," << o.postions.z << "," << o.postions.w << std::endl;
			addShape(shapetype, -1, TRIANGLES);

			pickedShape = p;

			shapeTransformation(yLocalTranslate, 2 * o.lvl + 1);
			shapeTransformation(xLocalTranslate, 2 * (o.postions.x) + 1);
			shapeTransformation(zLocalTranslate, 2 * (o.postions.y) + 1);
			shapeTransformation(xScale, 0.25);
			shapeTransformation(zScale, 0.25);
			shapeTransformation(yScale, 0.25);
			Sapples.push_back(shapes[pickedShape]);

			shapes[pickedShape]->SetTexture(tapple);
			//	}
			//}

			break;
		case mine:

			//for (int i = 0; i < o.postions.z; i++)
			//{
			//	for (int j = 0; j < o.postions.w; j++)
			//	{
			p = shapes.size();
			//std::cout << o.postions.x << "," << o.postions.y << "," << o.postions.z << "," << o.postions.w << std::endl;
			addShape(shapetype, -1, TRIANGLES);
			pickedShape = p;

			shapeTransformation(yLocalTranslate, 2 * o.lvl + 1);
			shapeTransformation(xLocalTranslate, 2 * (o.postions.x) + 1);
			shapeTransformation(zLocalTranslate, 2 * (o.postions.y) + 1);

			SMines.push_back(shapes[pickedShape]);

			shapes[pickedShape]->SetTexture(tmine);
			//	}
			//}

			break;
		case goal:
			p = shapes.size();
			//std::cout << o.postions.x << "," << o.postions.y << "," << o.postions.z << "," << o.postions.w << std::endl;
			addShape(shapetype, -1, TRIANGLES);
			pickedShape = p;

			shapeTransformation(yLocalTranslate, 2 * o.lvl + 1);
			shapeTransformation(xLocalTranslate, 2 * (o.postions.x) + 1);
			shapeTransformation(zLocalTranslate, 2 * (o.postions.y) + 1);

			Sgoal = (shapes[pickedShape]);
			
			shapes[pickedShape]->SetTexture(gold);
		default:
			break;
		}

		//shapes.at(pickedShape).
		//change color
	}
	//cameras[0]->Update();
	pickedShape = -1;

}

void Game::CreatBoundingBox(int shape_num)
{
	int p = pickedShape;
	addShape(Cube, -1, LINE_LOOP);
	pickedShape = shapes.size() - 1;
	shapes[shape_num]->boundingboxLoction = pickedShape;
	MeshConstructor* mShape = shapes.at(shape_num)->mesh;
	B_Node* bNode = mShape->boundingTree;
	BoundingBox* bb = &bNode->bb;
	bb->updateValues(shapes.at(shape_num)->makeTrans(), shapes.at(shape_num)->makeScale());
	//shapeTransformation(xGlobalTranslate, 0);
	//shapeTransformation(yGlobalTranslate, 0);
	//shapeTransformation(zGlobalTranslate, 0);

	float rotate = atan2(bb->newzInit.z, bb->newzInit.y) * 180 / M_PI - 90;
	//std::cout << -rotate << "\n";
	shapeTransformation(xLocalRotate, -rotate);

	//we never rotate in y
	rotate = atan2(-1 * bb->newzInit.x, sqrt(bb->newzInit.z *bb->newzInit.z + bb->newzInit.y*bb->newzInit.y)) * 180 / M_PI;
	//std::cout << rotate << "\n";
	shapeTransformation(yLocalRotate, rotate);

	rotate = atan2(bb->newxInit.x, bb->newyInit.x) * 180 / M_PI - 90;
	//std::cout << -rotate << "\n";
	shapeTransformation(zLocalRotate, -rotate);

	//shapeTransformation(xGlobalTranslate, bb->newCenter.x);
	//shapeTransformation(yGlobalTranslate, bb->newCenter.y);
	//shapeTransformation(zGlobalTranslate, bb->newCenter.z);

	shapeTransformation(xScale, bb->newsize.x);
	shapeTransformation(yScale, bb->newsize.y);
	shapeTransformation(zScale, bb->newsize.z);

	chainParents.at(pickedShape) = shape_num;
	pickedShape = p;
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

void Game::Update(const glm::mat4 &MVP, const glm::mat4 &myNormal, const glm::mat4 &preNormal, const glm::mat4 &nextNormal, const int  shaderIndx, const glm::mat4 &scale)
{

	glm::mat3x4 pretranlation(glm::transpose(preNormal));
	glm::mat3x4 nexttranlation(glm::transpose(nextNormal));
	glm::dualquat prequat = glm::dualquat(glm::dualquat_cast(pretranlation));
	glm::dualquat nextquat = glm::dualquat(glm::dualquat_cast(nexttranlation));

	glm::mat3x4 tranlation(glm::transpose(myNormal));//glm::transpose(shapes.at(pickedShape)->makeTrans()));
	glm::dualquat myquat(glm::dualquat_cast(tranlation));


	Shape* shape = shapes.at(pickedShape);
	Shader *s = shaders[3];
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

std::vector<glm::vec3> Game::skinning_shapes() {
	std::vector<glm::vec3> skin;
	SnakePart* temp = &mySnake->Head;
	while (temp != NULL)
	{

		glm::vec3 skinloction(1);
		if (temp->Ifallow == NULL) // only head
			skinloction.x = temp->loctionInSapes;
		else
			skinloction.x = temp->Ifallow->loctionInSapes;

		skinloction.y = temp->loctionInSapes;

		if (temp->MySon == NULL) // only tail
			skinloction.z = temp->loctionInSapes;
		else
			skinloction.z = temp->MySon->loctionInSapes;

		skin.push_back(skinloction);

		temp = temp->MySon;
	}

	return skin;
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
int count = 0;
int count2 = 0;
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
		cameras[0]->Update(mySnake->Head.me, shapes[Camrasstart + Curentcamera], glm::vec3(0, 1, 0), glm::vec3(0, 0, 0));
		if (canmove)
			mySnake->move(this);
		//if (count== 30) {
			checkCollsion();
			count = 0;
	//	}
		count++;
	}
	pickedShape = p;
}

void Game::checkCollsion() {
	if(Sgoal !=NULL)
		if (mySnake->Head.me->checkColsion2(Sgoal)) {
			// TODO: end game win
			PlaySound(TEXT("../res/sound/win.wav"), NULL, SND_ASYNC);
			std::cout << "you have won!!!" << std::endl;
			std::cout << "your score is " << score << std::endl;
			mySnake->speed = 0;
		}

	if (!Happle)
		for each (Shape* a in Sapples)
		{
			if (a->Is2Render() && mySnake->Head.me->checkColsion2(a)) {
				if (!(mySnake->canColiod & apple))
				{
					std::cout << "hit - apple" << std::endl;
					PlaySound(TEXT("../res/sound/apple.wav"), NULL, SND_ASYNC);
					score++;
					std::cout << "your scoure is " << score << std::endl;
					if (score % 3 == 0)
						mySnake->speed++;
					a->Hide();
				}
			}
		}
	if (!Hmines)
		for each (Shape* a in SMines)
		{
			if (a->Is2Render() && mySnake->Head.me->checkColsion2(a)) {
				if (!(mySnake->canColiod & mine))
				{
					std::cout << "hit - mine" << std::endl;
					PlaySound(TEXT("../res/sound/mine.wav"), NULL, SND_ASYNC);
					if (score == 0) {
						std::cout << "GAME OVER" << std::endl;
						std::cout << "your score is " << score << std::endl;
						mySnake->speed = 0;
					}
					else {
						score = 0;
						std::cout << "your scoure is 0" << std::endl;
					}

					a->Hide();
				}
			}
		}


	if (!Hramp)
		for each (Shape* r in Sramps)
		{
			//if (r->Is2Render() && mySnake->Head.me->checkColsion2(r)) {
			if (mySnake->Head.me->checkColsion2(r)) {
				std::cout << "hit - ramp" << std::endl;
				if (!(mySnake->canColiod & rampS))
				{
					mySnake->canColiod += rampS;
					PlaySound(TEXT("../res/sound/water1.wav"), NULL, SND_ASYNC);
					// TODO: calc next level no time

					mySnake->needLvL++;
					//mySnake->neddLvL--;
				}
			}
		}

	if (!Hwall)
		for each (Shape* r in Swalls)
		{
			if (mySnake->Head.me->checkColsion2(r)) {
				std::cout << "hit - wall" << std::endl;
				std::cout << "GAME OVER" << std::endl;
				std::cout << "your score is " << score << std::endl;
				mySnake->speed = 0;
			}
		}




}

void Game::speed(int s) {
	mySnake->speed+=s;
	if (mySnake->speed < 1)
		mySnake->speed = 1;
}


void Game::hide(int type) {
	switch (type)
	{
	case rampS:
		if (Hramp)
		{
			for each (Shape* r in Sramps)
			{
				r->Unhide();
			}
			Hramp = false;
		}
		else
		{
			for each (Shape* r in Sramps)
			{
				r->Hide();
			}
			Hramp = true;
		}
		break;
	case wall:
		if (Hwall)
		{
			for each (Shape* r in Swalls)
			{
				r->Unhide();
			}
			Hwall = false;
		}
		else
		{
			for each (Shape* r in Swalls)
			{
				r->Hide();
			}
			Hwall = true;
		}
		break;
	case apple:
		if (Happle)
		{
			for each (Shape* r in Sapples)
			{
				r->Unhide();
			}
			Happle = false;
		}
		else
		{
			for each (Shape* r in Sapples)
			{
				r->Hide();
			}
			Happle = true;
		}
		break;
	case mine:
		if (Hmines)
		{
			for each (Shape* r in SMines)
			{
				r->Unhide();
			}
			Hmines = false;
		}
		else
		{
			for each (Shape* r in SMines)
			{
				r->Hide();
			}
			Hmines = true;
		}
		break;
	default:
		break;
	}
}

void Game::MoveSnake(int type, float amount) {

	int p = pickedShape;
	pickedShape = mySnake->Head.loctionInSapes;
	shapeTransformation(type, amount);
	pickedShape = p;
}

void Game::creatWalls() {
	//floor
	pickedShape = shapes.size();
	addShape(Cube, -1, TRIANGLES);
	shapes[pickedShape]->SetTexture(bricks);
	shapeTransformation(zLocalTranslate, (float)reader->z);
	shapeTransformation(xLocalTranslate, (float)reader->x);
	shapeTransformation(yLocalTranslate, -1 + 0.4);
	shapeTransformation(xScale, reader->x);
	shapeTransformation(zScale, reader->z);
	//wall base
	pickedShape = shapes.size();
	addShape(Cube, -1, TRIANGLES);
	shapes[pickedShape]->SetTexture(bricks);
	//shapeTransformation(zLocalTranslate, (float)reader->z);
	shapeTransformation(xLocalTranslate, (float)reader->x);
	shapeTransformation(zLocalTranslate, -1);
	shapeTransformation(yLocalTranslate, 4);
	shapeTransformation(yLocalTranslate, 0.4);
	shapeTransformation(xScale, reader->x);
	shapeTransformation(yScale, 6);
	//shapeTransformation(zScale, reader->z);
	//wall right
	pickedShape = shapes.size();
	addShape(Cube, -1, TRIANGLES);
	shapes[pickedShape]->SetTexture(bricks);
	shapeTransformation(zLocalTranslate, (float)reader->z);
	//shapeTransformation(xLocalTranslate, (float)reader->x);
	shapeTransformation(xLocalTranslate, -1);
	//shapeTransformation(xScale, reader->x);
	shapeTransformation(zScale, reader->z);
	shapeTransformation(yLocalTranslate, 4);
	shapeTransformation(yLocalTranslate, 0.4);
	shapeTransformation(yScale, 6);
	//wall left
	pickedShape = shapes.size();
	addShape(Cube, -1, TRIANGLES);
	shapes[pickedShape]->SetTexture(bricks);
	shapeTransformation(zLocalTranslate, (float)reader->z);
	shapeTransformation(xLocalTranslate, 2 * (float)reader->x);
	shapeTransformation(xLocalTranslate, -3);
	//shapeTransformation(xScale, reader->x);
	shapeTransformation(zScale, reader->z);
	shapeTransformation(yLocalTranslate, 4);
	shapeTransformation(yLocalTranslate, 0.4);
	shapeTransformation(yScale, 6);
	//wall far
	pickedShape = shapes.size();
	addShape(Cube, -1, TRIANGLES);
	shapes[pickedShape]->SetTexture(bricks);
	shapeTransformation(zLocalTranslate, 2 * (float)reader->z);
	shapeTransformation(xLocalTranslate, (float)reader->x);
	shapeTransformation(zLocalTranslate, -3);
	shapeTransformation(yLocalTranslate, 4);
	shapeTransformation(yLocalTranslate, 0.4);
	shapeTransformation(xScale, reader->x);
	shapeTransformation(yScale, 6);
	//shapeTransformation(zScale, reader->z);
}
void Game::setCamera() {

	pickedShape = shapes.size();
	Camrasstart = pickedShape;
	addShape(Cube, -1, LINES);
	shapeTransformation(xLocalTranslate, 2.4);
	shapeTransformation(yLocalTranslate, 2);
	shapeTransformation(yScale, 0.010);
	shapeTransformation(xScale, 0.010);
	shapeTransformation(zScale, 0.010);
	chainParents.at(pickedShape) = 1;
	shapes[pickedShape]->Hide();

	pickedShape = shapes.size();
	addShape(Cube, -1, LINES);
	shapeTransformation(xLocalTranslate, 8);
	shapeTransformation(yLocalTranslate, 30);
	shapeTransformation(yScale, 0.010);
	shapeTransformation(xScale, 0.010);
	shapeTransformation(zScale, 0.010);
	chainParents.at(pickedShape) = 1;
	shapes[pickedShape]->Hide();

	pickedShape = shapes.size();
	addShape(Cube, -1, LINES);
	shapeTransformation(xLocalTranslate, -4);
	shapeTransformation(zLocalTranslate, -8);
	shapeTransformation(yLocalTranslate, 0.5);
	shapeTransformation(yScale, 0.010);
	shapeTransformation(xScale, 0.010);
	shapeTransformation(zScale, 0.010);
	chainParents.at(pickedShape) = 1;
	shapes[pickedShape]->Hide();



}


Game::~Game(void)
{
	delete curve;
}