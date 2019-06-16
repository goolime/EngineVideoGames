#pragma once
#include "scene.h"
#include "bezier1D.h"

class Game : public Scene
{
	Bezier1D *curve;
public:

	void ShowBoundingBox(int shape_num);
	void ShowBoundingBox(int shape, BoundingBox* bb);
	void updateBoundings(int shape_num, int boxNum);
	
	void updateBoundings(int shape_num, int boxNum, BoundingBox* bb);
	Game(void);
	Game(glm::vec3 position,float angle,float hwRelation,float near, float far);
	void Init();
	void addShape(int type,int parent,unsigned int mode);
//	void Update( glm::mat4 MVP ,glm::mat4 *jointTransforms,const int length,const int  shaderIndx);
	void Update(const glm::mat4 &MVP, const glm::mat4 &Normal, const glm::mat4 &preNormal, const glm::mat4 &nextNormal, const int  shaderIndx, const glm::mat4 &scale);
	//void Update(const glm::mat4 &MVP,const glm::mat4 &Normal,const int  shaderIndx);
	void ControlPointUpdate();
	void WhenRotate();
	void WhenTranslate();
	void Motion();
	~Game(void);
};

