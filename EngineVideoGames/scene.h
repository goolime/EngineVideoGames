#pragma once
#include "shader.h"
#include "shape.h"
#include "camera.h"
#include "VertexArray.hpp"
#include <vector>

class Scene : public MovableGLM
{
	
	std::vector<Shape*> shapes;
	std::vector<Camera*> cameras; //light will have the properties of camera
	std::vector<VertexArray*> vaos; 
	
	Shape *axisMesh;
	int verticesSize;
	int indicesSize;

	float depth;
	double xold, yold,xrel, yrel;
	int cameraIndx;

protected:
		std::vector<Shader*> shaders;
	std::vector<int> chainParents;
	int pickedShape;
	int direction;
	static const int scaleFactor =3;
	bool isActive;
public:
	enum axis{xAxis,yAxis,zAxis};
	enum transformations{xLocalTranslate,yLocalTranslate,zLocalTranslate,xGlobalTranslate,yGlobalTranslate,zGlobalTranslate,
		xLocalRotate,yLocalRotate,zLocalRotate,xGlobalRotate,yGlobalRotate,zGlobalRotate,xScale,yScale,zScale,xCameraTranslate,yCameraTranslate,zCameraTranslate};
	Scene();
	Scene(glm::vec3 position,float angle,float hwRelation,float near, float far);
	void addShape(int type,int parent);
	void addShape(const std::string& fileName,int parent);
	void addShape(const std::string& fileName,const std::string& textureFileName,int parent);
	void addShape(Vertex* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices,int parent);
	void addShape(Vertex* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices,const std::string& textureFileName,int parent);
	void addShape(int Cyparts,int linkPosition,int parent);
	void addShape(int CylParts,int linkPosition,const std::string& textureFileName,int parent);
	
	void addShader(const std::string& fileName);
	void addBox(const std::string& fileName, int parent);
	void addAxis();

	//void Update( int const shaderIndx);
	virtual void Update( glm::mat4 MVP ,glm::mat4 *jointTransforms,const int length,const int  shaderIndx);

	glm::mat4 GetViewProjection(int indx) const;
	glm::mat4 GetShapeTransformation() const;
	void draw(int shaderIndx,int cameraIndx,bool drawAxis);
	void shapeTransformation(int type,float amt);
	void shapeRotation(glm::vec3 v, float ang,int indx);
	//void inline setPicked(int pickID){pickedShape = pickID;}
	float picking(double x,double y);
	void resize(int width,int hight);
	//void updateTipPosition(int indx);
	glm::vec3 getTipPosition(int indx);
	glm::vec3 getDistination(int indx);
	glm::vec3 getAxisDirection(int indx,int axis);
	inline void setParent(int indx,int newValue) {chainParents[indx]=newValue;}
	inline float GetNear(int cameraIndx)
	{
		return cameras[cameraIndx]->GetNear();
	}
	inline float GetFar(int cameraIndx)
	{
		return cameras[cameraIndx]->GetFar();
	}
	inline float GetWHRelation(int cameraIndx)
	{
		return cameras[cameraIndx]->GetWHRelation();
	}

	inline float GetAngle(int cameraIndx)
	{
		return cameras[cameraIndx]->GetAngle();
	}

	//inline void SetMousePosition(double xpos, double ypos){xold =xpos; yold=ypos;}
	void updatePosition(double xpos, double ypos);
	void mouseProccessing(int button);
	bool inline IsActive() const { return isActive;} 
	virtual ~Scene(void);

	//static LineVertex axisVertices[6];

	//static unsigned int axisIndices[6];

	//static Vertex vertices[24] ;
	//
	//static unsigned int indices[24] ;

};
