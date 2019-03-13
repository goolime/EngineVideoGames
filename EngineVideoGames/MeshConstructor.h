#pragma once
#include <vector>
#include "glm\glm.hpp"
#include "VertexArray.hpp"
#include "Mesh.h"


class MeshConstructor
{
	IndexBuffer *ib;
	IndexedModel model;
	enum SimpleShapes
	{
		Axis,
		Cube,
		Octahedron,
		Tethrahedron
	};
	int initLine(IndexedModel &model,VertexArray &vao);
	int initMesh(IndexedModel &model,VertexArray &vao);
public:
	
	MeshConstructor(const int type, VertexArray &vao,int *indicesNum);
	inline unsigned int GetCount(){return ib->GetCount();}
	~MeshConstructor(void);

};
