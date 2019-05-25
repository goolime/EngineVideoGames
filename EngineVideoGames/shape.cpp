#define GLEW_STATIC
#include <GL\glew.h>
#include <utility> 
#include "shape.h"
#include "Log.hpp"


Shape::Shape(const Shape& shape,unsigned int mode)
{
	
	mesh = new MeshConstructor(*shape.mesh);
	//tex = shape.tex;
	isCopy = true;
	this->mode = mode;
	toRender = true;
	texID = -1;
	shaderID = 1;

}

Shape::Shape(const std::string& fileName, unsigned int mode){
	mesh = new MeshConstructor(fileName);
	isCopy = false;
	this->mode = mode;
	toRender = true;
	texID = -1;
	shaderID = 1;

}

Shape::Shape(const int SimpleShapeType,unsigned int mode)
{
	mesh = new MeshConstructor(SimpleShapeType);
	//mesh->Bind();
	this->mode = mode;
	isCopy = false;
	toRender = true;
	texID = -1;
	shaderID = 1;

}

Shape::Shape(Bezier1D *curve, unsigned int xResolution,unsigned int yResolution,bool is2D,unsigned int mode)
{
	mesh = new MeshConstructor(curve,is2D,xResolution,yResolution);
	this->mode = mode;
	isCopy = false;
	toRender = true;
	texID = -1;
	shaderID = 1;

}


void Shape::Draw( const std::vector<Shader*> shaders, const std::vector<Texture*> textures,bool isPicking)
{
	if(texID>=0)
		textures[texID]->Bind();
	if(isPicking)
		shaders[0]->Bind();
	else
		shaders[shaderID]->Bind();
	mesh->Bind();
	/*if(isCopy)
		glDrawArrays(GL_TRIANGLES, 0, indicesNum);
	else*/
	GLCall(glDrawElements(mode,mesh->GetIndicesNum(), GL_UNSIGNED_INT, 0));
	mesh->Unbind();
}

Shape::~Shape(void)
{
	if(!isCopy)
	{
		if(mesh)
			delete mesh;
	}
}

std::pair<BoundingBox*, BoundingBox*>  Shape::checkColsion(Shape* other) {
	 return mesh->checkCollsion(other->mesh, other->makeTrans(), makeTrans(),makeTransScale(),other->makeTransScale());
}