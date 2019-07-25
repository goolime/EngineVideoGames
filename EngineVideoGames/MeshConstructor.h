#pragma once
#include <vector>
#include "glm\glm.hpp"
#include "VertexArray.hpp"
#include "Mesh.h"
#include "bezier1D.h"
#include "kdtree.h"
#include "Mesh.h"

class B_Node
{
private:

	int pointNumber=0;
	//BoundingBox getBoundingbox(int low, int high, std::vector<glm::vec3>* points);//natai done
	BoundingBox getBoundingbox(Node n);//natai done
	BoundingBox getBoundingbox(Node n, glm::vec3 offset);//natai done
	BoundingBox getDefultBoundingbox(glm::vec3 point);//natai done
	BoundingBox* isColiding(glm::mat4 myMat, BoundingBox &otherbb, glm::mat4 otherMat,int lvl, glm::mat4 b1scale, glm::mat4 b2scale);
public:
	BoundingBox bb;
	//int low;
	//int high;
	B_Node* left;
	B_Node* right;
	B_Node(Node* root);//natai done
	B_Node(Node* root, glm::vec3 offset,bool first);//natai done
	B_Node(const B_Node &other);//natai done
	B_Node();// to delete
	std::pair<BoundingBox*, BoundingBox*> isColiding(glm::mat4 myMat, B_Node *other, glm::mat4 otherMat,int lvl, glm::mat4 b1scale, glm::mat4 b2scale);//gal
	bool isColiding2(glm::mat4 myMat, B_Node *other, glm::mat4 otherMat,int lvl, glm::mat4 b1scale, glm::mat4 b2scale);//gal
	
	~B_Node(void); // need work
};

class MeshConstructor
{
	VertexArray vao;
	IndexBuffer *ib;
	//TO DO: add bounding box data base and build it in the constructor 

	std::vector<VertexBuffer*> vbs;
	bool is2D;
	int unsigned indicesNum;

	void InitLine(IndexedModel &model);
	void InitMesh(IndexedModel &model);
	void CopyMesh(const MeshConstructor &mesh);
	void CopyLine(const MeshConstructor &mesh);


public:
	B_Node* boundingTree;
	//TO DO: add collision detection function which get other MeshConstructor and Mat4 of related transformasions. The function may return a pointer to the relevant Bounding Box when collide
	std::pair<BoundingBox*, BoundingBox*> checkCollsion(MeshConstructor* other, glm::mat4 hisTrans,glm::mat4 myTrans, glm::mat4 b1scale, glm::mat4 b2scale);//done
	bool checkCollsion2(MeshConstructor* other, glm::mat4 hisTrans,glm::mat4 myTrans, glm::mat4 b1scale, glm::mat4 b2scale);//done
	
	//gal

	enum SimpleShapes
	{
		Axis,
		Cube,
		Octahedron,
		Tethrahedron,
		BezierLine,
		BezierSurface,
	};
	MeshConstructor(const int type);//natai done
	MeshConstructor(Bezier1D *curve, bool isSurface, unsigned int resT, unsigned int resS);//natai done 
	MeshConstructor(const MeshConstructor &mesh);//gal
	MeshConstructor(const std::string& fileName);//gal

	void Bind() { vao.Bind(); }
	void Unbind() { vao.Unbind(); }
	inline unsigned int GetIndicesNum() { return indicesNum; }
	inline bool Is2D() { return is2D; }
	~MeshConstructor(void);

};

