#pragma once
#include "scene.h"
#include "bezier1D.h"
#include "Snake.h"
#include "MapReader.h"

enum textureName
{
	box0= 0,
	bricks= 1,
	grass= 2,
	plane= 3,
	snake0= 4,
	snake1= 5,
};

class Game : public Scene
{
	

	Bezier1D *curve;
private:
	bool Hramp = false;
	bool Hwall = false;
	bool Happle = false;
	bool Hmines = false;
	CSVReader* reader= NULL;
	//end project
	Shape* Sgoal;
	std::vector<Shape*> Sapples;
	std::vector<Shape*> SMines;	
	std::vector<Shape*> Sramps;
	std::vector<Shape*> Swalls;
	Shape* SGate;
	int Camrasstart = 4;
	void creatWalls();
	void setCamera();
	void createshapes(CSVReader* reader, int type, int shapetype);
	Snake* mySnake;
	int score = 0;

public:
	bool canmove = true;
	int Curentcamera = 0;
	//ass2
	void checkCollsion();
	void CreatBoundingBox(int shape_num);
	void ShowBoundingBox(int shape, BoundingBox* bb);
	void updateBoundings(int shape_num, int boxNum);
	void updateBoundings(int shape_num, int boxNum, BoundingBox* bb);
	//tamir
	Game(void);
	Game(glm::vec3 position,float angle,float hwRelation,float near, float far);
	void Init();
	void addShape(int type,int parent,unsigned int mode);
//	void Update( glm::mat4 MVP ,glm::mat4 *jointTransforms,const int length,const int  shaderIndx);
	//void Update(const glm::mat4 &MVP, const glm::mat4 &Normal, const glm::mat4 &preNormal, const glm::mat4 &nextNormal, const int  shaderIndx, const glm::mat4 &scale); blending
	//ass3
	void Update(const glm::mat4 &MVP, const glm::mat4 &Normal, const int  shaderIndx);
	void Update(const glm::mat4 &MVP, const glm::mat4 &Normal, const glm::mat4 &preNormal, const glm::mat4 &nextNormal, const int  shaderIndx, const glm::mat4 &scale);
	std::vector<glm::vec3> skinning_shapes();


	//end project
	void MoveSnake( int type, float amount);
	void speed(int s);
	void hide(int type);

	void ControlPointUpdate();
	void WhenRotate();
	void WhenTranslate();
	void Motion();
	~Game(void);
};

