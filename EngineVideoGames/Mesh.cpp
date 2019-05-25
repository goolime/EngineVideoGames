#include "Mesh.h"
#include <limits>
#include <iostream>
void IndexedModel::CalcNormals()
{
    for(unsigned int i = 0; i < indices.size(); i += 3)
    {
        int i0 = indices[i];
        int i1 = indices[i + 1];
        int i2 = indices[i + 2];

        glm::vec3 v1 = positions[i1] - positions[i0];
        glm::vec3 v2 = positions[i2] - positions[i0];
        
        glm::vec3 normal = glm::normalize(glm::cross(v1, v2));
            
        normals[i0] += normal;
        normals[i1] += normal;
        normals[i2] += normal;
    }
    
    for(unsigned int i = 0; i < positions.size(); i++)
	{
        normals[i] = glm::normalize(normals[i]);
		colors[i] = (glm::vec3(1,1,1) + normals[i])/2.0f;
	}
}


IndexedModel CubeTriangles()
{
	Vertex vertices[] =
	{
		Vertex(glm::vec3(-1, -1, -1), glm::vec2(1, 0), glm::vec3(0, 0, -1),glm::vec3(0, 0, 1)),
		Vertex(glm::vec3(-1, 1, -1), glm::vec2(0, 0), glm::vec3(0, 0, -1),glm::vec3(0, 0, 1)),
		Vertex(glm::vec3(1, 1, -1), glm::vec2(0, 1), glm::vec3(0, 0, -1),glm::vec3(0, 0, 1)),
		Vertex(glm::vec3(1, -1, -1), glm::vec2(1, 1), glm::vec3(0, 0, -1),glm::vec3(0, 0, 1)),

		Vertex(glm::vec3(-1, -1, 1), glm::vec2(1, 0), glm::vec3(0, 0, 1),glm::vec3(0, 1, 1)),
		Vertex(glm::vec3(-1, 1, 1), glm::vec2(0, 0), glm::vec3(0, 0, 1),glm::vec3(0, 1, 1)),
		Vertex(glm::vec3(1, 1, 1), glm::vec2(0, 1), glm::vec3(0, 0, 1),glm::vec3(0, 1, 1)),
		Vertex(glm::vec3(1, -1, 1), glm::vec2(1, 1), glm::vec3(0, 0, 1),glm::vec3(0, 1, 1)),

		Vertex(glm::vec3(-1, -1, -1), glm::vec2(0, 1), glm::vec3(0, -1, 0),glm::vec3(0, 1, 0)),
		Vertex(glm::vec3(-1, -1, 1), glm::vec2(1, 1), glm::vec3(0, -1, 0),glm::vec3(0, 1, 0)),
		Vertex(glm::vec3(1, -1, 1), glm::vec2(1, 0), glm::vec3(0, -1, 0),glm::vec3(0, 1, 0)),
		Vertex(glm::vec3(1, -1, -1), glm::vec2(0, 0), glm::vec3(0, -1, 0),glm::vec3(0, 1, 0)),

		Vertex(glm::vec3(-1, 1, -1), glm::vec2(0, 1), glm::vec3(0, 1, 0),glm::vec3(1, 1, 0)),
		Vertex(glm::vec3(-1, 1, 1), glm::vec2(1, 1), glm::vec3(0, 1, 0),glm::vec3(1, 1, 0)),
		Vertex(glm::vec3(1, 1, 1), glm::vec2(1, 0), glm::vec3(0, 1, 0),glm::vec3(1, 1, 0)),
		Vertex(glm::vec3(1, 1, -1), glm::vec2(0, 0), glm::vec3(0, 1, 0),glm::vec3(1, 1, 0)),

		Vertex(glm::vec3(-1, -1, -1), glm::vec2(1, 1), glm::vec3(-1, 0, 0),glm::vec3(1, 0, 0)),
		Vertex(glm::vec3(-1, -1, 1), glm::vec2(1, 0), glm::vec3(-1, 0, 0),glm::vec3(1, 0, 0)),
		Vertex(glm::vec3(-1, 1, 1), glm::vec2(0, 0), glm::vec3(-1, 0, 0),glm::vec3(1, 0, 0)),
		Vertex(glm::vec3(-1, 1, -1), glm::vec2(0, 1), glm::vec3(-1, 0, 0),glm::vec3(1, 0, 0)),

		Vertex(glm::vec3(1, -1, -1), glm::vec2(1, 1), glm::vec3(1, 0, 0),glm::vec3(1, 0, 1)),
		Vertex(glm::vec3(1, -1, 1), glm::vec2(1, 0), glm::vec3(1, 0, 0),glm::vec3(1, 0, 1)),
		Vertex(glm::vec3(1, 1, 1), glm::vec2(0, 0), glm::vec3(1, 0, 0),glm::vec3(1, 0, 1)),
		Vertex(glm::vec3(1, 1, -1), glm::vec2(0, 1), glm::vec3(1, 0, 0),glm::vec3(1, 0, 1))
	};



	unsigned int indices[] =	{0, 1, 2,
							  0, 2, 3,

							  6, 5, 4,
							  7, 6, 4,

							  10, 9, 8,
							  11, 10, 8,

							  12, 13, 14,
							  12, 14, 15,

							  16, 17, 18,
							  16, 18, 19,

							  22, 21, 20,
							  23, 22, 20
	                          };

	    IndexedModel model;
	
	for(unsigned int i = 0; i < 24; i++)
	{
		model.positions.push_back(*vertices[i].GetPos());
		model.colors.push_back(*vertices[i].GetColor());
		model.normals.push_back(*vertices[i].GetNormal());
		model.weights.push_back(*vertices[i].GetWeight());
		model.texCoords.push_back(*vertices[i].GetTexCoord());
	
	}
	for(unsigned int i = 0; i < 36; i++)
        model.indices.push_back(indices[i]);
	
	return model;
}

IndexedModel AxisGenerator()
{
	IndexedModel model;

	LineVertex axisVertices[] = 
	{
		LineVertex(glm::vec3(1,0,0),glm::vec3(1,0,0)),
		LineVertex(glm::vec3(-1,0,0),glm::vec3(1,0,0)),
		LineVertex(glm::vec3(0,1,0),glm::vec3(0,1,0)),
		LineVertex(glm::vec3(0,-1,0),glm::vec3(0,1,0)),
		LineVertex(glm::vec3(0,0,1),glm::vec3(0,0,1)),
		LineVertex(glm::vec3(0,0,-1),glm::vec3(0,0,1)),
	};


	 unsigned int axisIndices[] = 
	{
		0,1,
		2,3,
		4,5
	};

	 for(unsigned int i = 0; i < 6; i++)
	{
		model.positions.push_back(*axisVertices[i].GetPos());
		model.colors.push_back(*axisVertices[i].GetColor());

	}
	for(unsigned int i = 0; i < 6; i++)
        model.indices.push_back(axisIndices[i]);
	
	return model;
}

IndexedModel TethrahedronGenerator()
{

	Vertex vertices[] =
	{
		Vertex(glm::vec3(1, 1, 1), glm::vec2(1, 0), glm::vec3(-1, -1, -1),glm::vec3(0, 0, 1)),
		Vertex(glm::vec3(-1, 1, -1), glm::vec2(0, 0), glm::vec3(-1, -1, -1),glm::vec3(0, 0, 1)),
		Vertex(glm::vec3(-1, -1, 1), glm::vec2(0, 1), glm::vec3(-1, -1, -1),glm::vec3(0, 0, 1)),

		Vertex(glm::vec3(1, 1, 1), glm::vec2(1, 0), glm::vec3(1, -1, 1),glm::vec3(0, 1, 1)),
		Vertex(glm::vec3(-1, -1, 1), glm::vec2(0, 0), glm::vec3(1, -1, 1),glm::vec3(0, 1, 1)),
		Vertex(glm::vec3(1, -1, -1), glm::vec2(0, 1), glm::vec3(1, -1, 1),glm::vec3(0, 1, 1)),

		Vertex(glm::vec3(1, 1, 1), glm::vec2(0, 1), glm::vec3(1, 1, -1),glm::vec3(0, 1, 0)),
		Vertex(glm::vec3(-1, 1, -1), glm::vec2(1, 1), glm::vec3(1, 1, -1),glm::vec3(0, 1, 0)),
		Vertex(glm::vec3(1, -1, -1), glm::vec2(1, 0), glm::vec3(1, 1, -1),glm::vec3(0, 1, 0)),

		Vertex(glm::vec3(-1, 1, -1), glm::vec2(0, 1), glm::vec3(-1, -1, -1),glm::vec3(1, 1, 0)),
		Vertex(glm::vec3(-1, -1, 1), glm::vec2(1, 1), glm::vec3(-1, -1, -1),glm::vec3(1, 1, 0)),
		Vertex(glm::vec3(1, -1, -1), glm::vec2(1, 0), glm::vec3(-1, -1, -1),glm::vec3(1, 1, 0)),

	};



	unsigned int indices[] =	{0, 1, 2,
							     3, 4, 5,
								 6,7,8,
								 9,10,11
						
	                          };

	    IndexedModel model;
	
	for(unsigned int i = 0; i < 12; i++)
	{
		model.positions.push_back(*vertices[i].GetPos());
		model.colors.push_back(*vertices[i].GetColor());
		model.normals.push_back(*vertices[i].GetNormal());
	    model.weights.push_back(*vertices[i].GetWeight());
		model.texCoords.push_back(*vertices[i].GetTexCoord());
		//model.weights.push_back(glm::vec3(0,1,0));
	}
	for(unsigned int i = 0; i < 12; i++)
        model.indices.push_back(indices[i]);
	
	return model;
}

IndexedModel OctahedronGenerator()
{
	Vertex vertices[] =
	{
		Vertex(glm::vec3( 0, 0,-1), glm::vec2(1, 0), glm::vec3(1, 1, -1),glm::vec3(0, 0, 1)),
		Vertex(glm::vec3(0, 1, 0), glm::vec2(0, 0), glm::vec3(1, 1, -1),glm::vec3(0, 0, 1)),
		Vertex(glm::vec3(1, 0, 0), glm::vec2(0, 1), glm::vec3(1, 1, -1),glm::vec3(0, 0, 1)),
		
		Vertex(glm::vec3(0, 0, -1), glm::vec2(1, 0), glm::vec3(1, -1, -1),glm::vec3(0, 1, 1)),
		Vertex(glm::vec3(0,-1, 0), glm::vec2(0, 0), glm::vec3(1, -1, -1),glm::vec3(0, 1, 1)),
		Vertex(glm::vec3(1, 0, 0), glm::vec2(0, 1), glm::vec3(1, -1, -1),glm::vec3(0, 1, 1)),

		Vertex(glm::vec3(0, 0, -1), glm::vec2(0, 1), glm::vec3(-1, 1, -1),glm::vec3(0, 1, 0)),
		Vertex(glm::vec3(0, 1, 0), glm::vec2(1, 1), glm::vec3(-1, 1, -1),glm::vec3(0, 1, 0)),
		Vertex(glm::vec3(-1, 0, 0), glm::vec2(1, 0), glm::vec3(-1, 1, -1),glm::vec3(0, 1, 0)),
		
		Vertex(glm::vec3(0, 0, -1), glm::vec2(0, 1), glm::vec3(-1, -1, -1),glm::vec3(1, 1, 0)),
		Vertex(glm::vec3(0, -1, 0), glm::vec2(1, 1), glm::vec3(-1, -1, -1),glm::vec3(1, 1, 0)),
		Vertex(glm::vec3(-1, 0, 0), glm::vec2(1, 0), glm::vec3(-1, -1, -1),glm::vec3(1, 1, 0)),		

		Vertex(glm::vec3(0, 0, 1), glm::vec2(1, 1), glm::vec3(-1, -1, 1),glm::vec3(1, 0, 0)),
		Vertex(glm::vec3(0, -1, 0), glm::vec2(1, 0), glm::vec3(-1, -1, 1),glm::vec3(1, 0, 0)),
		Vertex(glm::vec3(-1, 0, 0), glm::vec2(0, 0), glm::vec3(-1, -1, 1),glm::vec3(1, 0, 0)),
		

		Vertex(glm::vec3(0, 0, 1), glm::vec2(1, 1), glm::vec3(1, -1, 1),glm::vec3(1, 0, 1)),
		Vertex(glm::vec3(0, -1, 0), glm::vec2(1, 0), glm::vec3(1, -1, 1),glm::vec3(1, 0, 1)),
		Vertex(glm::vec3(1, 0, 0), glm::vec2(0, 0), glm::vec3(1, -1, 1),glm::vec3(1, 0, 1)),

		Vertex(glm::vec3(0, 0, 1), glm::vec2(1, 1), glm::vec3(1, 1, 1),glm::vec3(0, 0, 0)),
		Vertex(glm::vec3(0,1, 0), glm::vec2(1, 0), glm::vec3(1, 1, 1),glm::vec3(0, 0, 0)),
		Vertex(glm::vec3(1, 0, 0), glm::vec2(0, 0), glm::vec3(1, 1, 1),glm::vec3(0, 0, 0)),

		Vertex(glm::vec3(0, 0, 1), glm::vec2(1, 1), glm::vec3(-1, 1, 1),glm::vec3(0.7f, 0.7f, 0.7f)),
		Vertex(glm::vec3(0, 1, 0), glm::vec2(1, 0), glm::vec3(-1, 1, 1),glm::vec3(0.7f, 0.7f, 0.7f)),
		Vertex(glm::vec3(-1, 0, 0), glm::vec2(0, 0), glm::vec3(-1, 1, 1),glm::vec3(0.7f, 0.7f, 0.7f)),
		
	};



	unsigned int indices[] =	{0, 1, 2,
							     3, 4, 5,
								 6,7,8,
								 9,10,11,
								 12,13,14, 
								 15,16,17,
								 18,19,20,
								 21,22,23,
	                          };

	    IndexedModel model;
	
	for(unsigned int i = 0; i < 24; i++)
	{
		model.positions.push_back(*vertices[i].GetPos());
		model.colors.push_back(*vertices[i].GetColor());
		model.normals.push_back(*vertices[i].GetNormal());
			model.weights.push_back(*vertices[i].GetWeight());
		model.texCoords.push_back(*vertices[i].GetTexCoord());
//		model.weights.push_back(glm::vec3(0,1,0));
	}
	for(unsigned int i = 0; i < 24; i++)
        model.indices.push_back(indices[i]);
	
	return model;
}

bool BoundingBox::checkCollision(glm::mat4 myMat, BoundingBox other, glm::mat4 otherMat, glm::mat4 b1scale, glm::mat4 b2scale) {
	bool myAline = this->checkCollision(myMat, *this, other, otherMat,b1scale,b2scale);
	bool hiAline = other.checkCollision(otherMat, other, *this, myMat, b1scale, b2scale);
	return  myAline && hiAline;
}

glm::mat4 getTransformation(glm::vec3 u, glm::vec3 v) {
	glm::vec3 axis = glm::cross(u, v);
	float angle = glm::acos(glm::dot(u, v));
	return glm::rotate(angle, axis);
}

bool axsisCut(float b1min, float b1max, float b2min, float b2max) {
	return (b1min <= b2max && b2max <= b1max) || (b1min <= b2min && b2min <= b1max) || (b1min >= b2min && b1max <= b2max);
}

glm::vec3 dup(glm::vec3 v, glm::mat4 m, double t) {
	glm::vec4 v4 = glm::vec4(v, t);
	glm::vec4 tmpans = m * v4;
	return glm::vec3(tmpans.x, tmpans.y, tmpans.z);
}
void BoundingBox::updateValues(glm::mat4 trans, glm::mat4 scale) {
	glm::mat3 inlarge((scale));
	glm::mat3 rotate(( trans));

	newCenter = glm::vec3(center.x, center.y, center.z) * glm::transpose(rotate);
	newCenter = glm::vec3(trans[3].x +newCenter.x, trans[3].y+ newCenter.y, trans[3].z  + newCenter.z);// * glm::transpose( rotate);
	//newCenter = newCenter + (offset*inlarge);// glm::transpose(rotate)*inlarge);
	newxInit = xInit * rotate;// dup(xInit, trans, 0);
	newyInit = yInit * rotate;// (yInit, trans, 0);
	newzInit = zInit * rotate;// (zInit, trans, 0);
	newsize = size * inlarge;// dup(size, trans, 1);
}
bool BoundingBox::checkCollision(glm::mat4 myMat, BoundingBox b1, BoundingBox b2, glm::mat4 otherMat, glm::mat4 b1scale, glm::mat4 b2scale) {

	b1.updateValues(myMat,b1scale);
	b2.updateValues(otherMat,b2scale);
	glm::mat3 to_b1 = glm::mat3();
	to_b1[0] = glm::vec3(b1.newxInit[0], b1.newyInit[0], b1.newzInit[0]);
	to_b1[1] = glm::vec3(b1.newxInit[1], b1.newyInit[1], b1.newzInit[1]);
	to_b1[2] = glm::vec3(b1.newxInit[2], b1.newyInit[2], b1.newzInit[2]);

	glm::mat3 to_b2 = glm::mat3();
	to_b2[0] = glm::vec3(b2.newxInit[0], b2.newyInit[0], b2.newzInit[0]);
	to_b2[1] = glm::vec3(b2.newxInit[1], b2.newyInit[1], b2.newzInit[1]);
	to_b2[2] = glm::vec3(b2.newxInit[2], b2.newyInit[2], b2.newzInit[2]);

	glm::mat3 from_b1 = glm::inverse(to_b1);

	glm::mat3 b1_to_b2 = to_b2 * from_b1;

	glm::vec3 b1xb2 = b1_to_b2 * b1.newxInit;
	glm::vec3 b1yb2 = b1_to_b2 * b1.newyInit;
	glm::vec3 b1zb2 = b1_to_b2 * b1.newzInit;

	float b1minx = std::numeric_limits<float>::max(), b1maxx = -std::numeric_limits<float>::max()
		, b1miny = std::numeric_limits<float>::max(), b1maxy = -std::numeric_limits<float>::max()
		, b1minz = std::numeric_limits<float>::max(), b1maxz = -std::numeric_limits<float>::max();
	float b2minx = std::numeric_limits<float>::max(), b2maxx = -std::numeric_limits<float>::max()
		, b2miny = std::numeric_limits<float>::max(), b2maxy = -std::numeric_limits<float>::max()
		, b2minz = std::numeric_limits<float>::max(), b2maxz = -std::numeric_limits<float>::max();

	for (int i = -1; i < 2; i++) {
		for (int j = -1; j < 2; j++) {
			for (int k = -1; k < 2; k++) {
				glm::vec3 p = b1.newCenter + ((float)i * b1xb2) * b1.newsize.x + ((float)j * b1yb2) * b1.newsize.y + ((float)k * b1zb2) * b1.newsize.z;
				b1minx = std::fmin(p.x, b1minx);
				b1maxx = std::fmax(p.x, b1maxx);
				b1miny = std::fmin(p.y, b1miny);
				b1maxy = std::fmax(p.y, b1maxy);
				b1minz = std::fmin(p.z, b1minz);
				b1maxz = std::fmax(p.z, b1maxz);
			}
		}
	}

	for (int i = -1; i < 2; i++) {
		for (int j = -1; j < 2; j++) {
			for (int k = -1; k < 2; k++) {
				glm::vec3 p = b2.newCenter + ((float)i * b2.newxInit) * b2.newsize.x + ((float)j * b2.newyInit) * b2.newsize.y + ((float)k * b2.newzInit) * b2.newsize.z;
				b2minx = std::fmin(p.x, b2minx);
				b2maxx = std::fmax(p.x, b2maxx);//ronge
				b2miny = std::fmin(p.y, b2miny);
				b2maxy = std::fmax(p.y, b2maxy);
				b2minz = std::fmin(p.z, b2minz);
				b2maxz = std::fmax(p.z, b2maxz);
			}
		}
	}
	return axsisCut(b1minx, b1maxx, b2minx, b2maxx)
		&& axsisCut(b1miny, b1maxy, b2miny, b2maxy)
		&& axsisCut(b1minz, b1maxz, b2minz, b2maxz);
}
