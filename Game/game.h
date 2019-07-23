#pragma once
#include "scene.h"
#include "bezier1D.h"
#include "Snake.h"
#include "MapReader.h"

class Game : public Scene
{

	Bezier1D *curve;
private:
	int Camrasstart = 4;
	
	void createshapes(CSVReader* reader, int type, int shapetype);
	Snake* mySnake;
public:
	int Curentcamera = 0;
	void ShowBoundingBox(int shape_num);
	void ShowBoundingBox(int shape, BoundingBox* bb);
	void updateBoundings(int shape_num, int boxNum);
	
	void updateBoundings(int shape_num, int boxNum, BoundingBox* bb);
	Game(void);
	Game(glm::vec3 position,float angle,float hwRelation,float near, float far);
	void Init();
	void addShape(int type,int parent,unsigned int mode);
//	void Update( glm::mat4 MVP ,glm::mat4 *jointTransforms,const int length,const int  shaderIndx);
	//void Update(const glm::mat4 &MVP, const glm::mat4 &Normal, const glm::mat4 &preNormal, const glm::mat4 &nextNormal, const int  shaderIndx, const glm::mat4 &scale); blending
	void Update(const glm::mat4 &MVP, const glm::mat4 &Normal, const int  shaderIndx);
	
	void MoveSnake(int pick, int type, float amount);

	void ControlPointUpdate();
	void WhenRotate();
	void WhenTranslate();
	void Motion();
	~Game(void);
};

