#pragma once
#include "MovableGLM.h"
#include "VertexArray.hpp"
#include "shader.h"
#include "MeshConstructor.h"

#include "texture.h"

class Shape : public MovableGLM
{
private:

	int texID;

	bool isCopy;
	unsigned int mode;
	bool toRender;

public:
	int shaderID;
	std::string name = "";
	int boundingboxLoction = -1;

	std::pair<BoundingBox*, BoundingBox*> checkColsion(Shape* other);
	bool checkColsion2(Shape* other);

	MeshConstructor *mesh;

	Shape(const Shape& shape,unsigned int mode);

	Shape(const std::string& fileName,unsigned int mode);
	
	Shape(const int SimpleShapeType,unsigned int mode);

	Shape(Bezier1D *curve, unsigned int xResolution,unsigned int yResolution,bool is2D,unsigned int mode);

	void Draw( const std::vector<Shader*> shaders, const std::vector<Texture*> textures,bool isPicking);

	inline void Hide() {toRender = false;}

	inline void Unhide() {toRender = true;}

	inline bool Is2Render() {return toRender;}

	inline bool Is2D(){return mesh->Is2D();}

	inline void SetTexture(int id){
		texID = id;
	}

	inline void SetShader(int id){ shaderID = id;}

	inline int GetShader(){return shaderID;}

	virtual ~Shape(void);
};

