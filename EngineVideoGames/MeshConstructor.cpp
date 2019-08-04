#define GLEW_STATIC
#include <GL\glew.h>
#include "MeshConstructor.h"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include "bezier2D.h"
#include "obj_loader.h"
//to delete
#include <iostream>


/*
BoundingBox B_Node::getBoundingbox(int low, int high, std::vector<glm::vec3>* points) {
	if (low == high)
		return getDefultBoundingbox(points->at(low));
	glm::vec3 maxPoint(std::numeric_limits<float>::min(), std::numeric_limits<float>::min(), std::numeric_limits<float>::min());
	glm::vec3 minPoint(std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
	for (int i = low; i <= high; i++)
	{
		if (points->at(i).x > maxPoint.x)
			maxPoint.x = points->at(i).x;
		if (points->at(i).y > maxPoint.y)
			maxPoint.y = points->at(i).y;
		if (points->at(i).z > maxPoint.z)
			maxPoint.z = points->at(i).z;

		if (points->at(i).x < minPoint.x)
			minPoint.x = points->at(i).x;
		if (points->at(i).y < minPoint.y)
			minPoint.y = points->at(i).y;
		if (points->at(i).z < minPoint.z)
			minPoint.z = points->at(i).z;
	}
	glm::vec3 half(0.5, 0.5, 0.5);
	glm::vec3 center = (maxPoint + minPoint) * half;
	glm::vec3 size = (maxPoint - minPoint);
	return BoundingBox(center, size, glm::vec3(1, 0, 0), glm::vec3(0, 1, 0), glm::vec3(0, 0, 1));
}//natai*/

glm::vec3 getMax(glm::vec3 v1, glm::vec3 v2) {
	if (v2.x > v1.x)
		v1.x =v2.x;
	if (v2.y > v1.y)
		v1.y = v2.y;
	if (v2.z > v1.z)
		v1.z = v2.z;
	return v1;
}
glm::vec3 getMin(glm::vec3 v1, glm::vec3 v2) {
	if (v2.x < v1.x)
		v1.x = v2.x;
	if (v2.y < v1.y)
		v1.y = v2.y;
	if (v2.z < v1.z)
		v1.z = v2.z;
	return v1;
}

glm::vec3 getMax(Node* node)
{	
	 glm::vec3 max(node->data);
	 if (node->left != NULL)
		max = getMax(max,getMax(node->left));
	 if (node->right != NULL)
		 max = getMax(max, getMax(node->right));
	 return max;
}

glm::vec3 getMin(Node* node)
{
	glm::vec3 min(node->data);
	if (node->left != NULL)
		min = getMin(min, getMin(node->left));
	if (node->right != NULL)
		min = getMin(min, getMin(node->right));
	return min;
}

BoundingBox B_Node::getBoundingbox(Node node) {
	if (node.right == NULL && node.left == NULL)
		return getDefultBoundingbox(glm::vec3(node.data));
	glm::vec3 maxPoint(getMax(&node));
	glm::vec3 minPoint(getMin(&node));
	// no for we do recursion


	glm::vec3 half(0.5, 0.5, 0.5);
	glm::vec3 center = (maxPoint + minPoint) * half;
	glm::vec3 size = (maxPoint - minPoint)*half;
	return BoundingBox(center, size, glm::vec3(1, 0, 0), glm::vec3(0, 1, 0), glm::vec3(0, 0, 1));

}

B_Node::~B_Node()
{
	//free(bb);
	if (left != NULL)
		delete left;
	if (right != NULL)
		delete right;
}

BoundingBox B_Node::getDefultBoundingbox(glm::vec3 point) {
	return	BoundingBox(point, glm::vec3(0.1, 0.1, 0.1), glm::vec3(1, 0, 0), glm::vec3(0, 1, 0), glm::vec3(0, 0, 1));
}

BoundingBox*  B_Node::isColiding(glm::mat4 myMat, BoundingBox &other, glm::mat4 otherMat, int lvl, glm::mat4 b1scale, glm::mat4 b2scale) {
	if (!bb.checkCollision(myMat, other, otherMat, b1scale, b2scale))
		return NULL;
	if ((left == NULL && right == NULL) || pointNumber <= 3||lvl >=5)
		return &bb;
	BoundingBox* isThereCollsion = NULL;
	if (right != NULL) {
		isThereCollsion = right->isColiding(myMat, other, otherMat, lvl + 1, b1scale, b2scale);
		if (isThereCollsion != NULL)
			return isThereCollsion;
	}// is collding in right side
	if (isThereCollsion == NULL && left != NULL) {
		isThereCollsion =left->isColiding(myMat, other, otherMat, lvl + 1, b1scale, b2scale);
		if (isThereCollsion !=NULL) // if not in right maybe in left
			return isThereCollsion;

	}	return isThereCollsion;// this will be the first collsion to be found
}
//return my bb that hit his
std::pair<BoundingBox*, BoundingBox*> B_Node::isColiding(glm::mat4 myMat, B_Node* other, glm::mat4 otherMat, int lvl, glm::mat4 b1scale, glm::mat4 b2scale) {
	if (!bb.checkCollision(myMat, other->bb, otherMat, b1scale, b2scale))
		return  { NULL,NULL };
	/*std::cout << "found collistion at lvl - " << lvl << "\n";
	std::cout <<"vec3("<< bb.center.x <<","<<bb.center.y <<","<< bb.center.z <<")\n";*/
	//!bb.checkCollision(myMat, other->bb, otherMat);
	if ((right == NULL && left == NULL) || pointNumber <= 6|| lvl >=5) {
		BoundingBox* hisBB = other->isColiding(otherMat, bb, myMat, 0, b2scale, b1scale);
		if (hisBB!=NULL)//mybe &bb and yes i do swich the mats here also i wantt to return the other bb
			return {&bb,hisBB};
	}
	std::pair<BoundingBox*, BoundingBox*> output = {NULL,NULL};
	if (right != NULL)
		output = right->isColiding(myMat, other, otherMat, lvl + 1,b1scale,b2scale); // if i was smart i will do a swich here 
	//like so
	//output = other.right->isColiding(otherMat,this,myMat);
	if (output.first == NULL && left != NULL)
		output = left->isColiding(myMat, other, otherMat, lvl + 1,b1scale,b2scale);
	return output;
}
B_Node::B_Node():bb(glm::vec3(0, 0, 0), glm::vec3(0.1, 0.1, 0.1), glm::vec3(1, 0, 0), glm::vec3(0, 1, 0), glm::vec3(0, 0, 1)) {
}

B_Node::B_Node(Node* Root, glm::vec3 offset, bool first) : bb(getBoundingbox(*Root)) {
	//if (first)
	//{
	//
	//}
	offset = bb.center;
	bb.offset = offset;
	bb.center = bb.center - offset;

	pointNumber++;
	if (Root->left != NULL) {
		left = new B_Node(Root->left, offset, false); // watch out need to delete later
		pointNumber += left->pointNumber;
	}
	if (Root->right != NULL) {
		right = new B_Node(Root->right, offset, false); // watch out need to delete later
		pointNumber += right->pointNumber;
	}
}

B_Node::B_Node(Node* Root):bb(getBoundingbox(*Root)) {
	
	//low = 1;
	//high = 1;
	//bb = getBoundingbox(*Root);
	pointNumber++;
	if (Root->left != NULL) {
		left = new B_Node(Root->left); // watch out need to delete later
		pointNumber += left->pointNumber;
	}
	if (Root->right != NULL) {
		right = new B_Node(Root->right); // watch out need to delete later
		pointNumber += right->pointNumber;
	}
}

B_Node::B_Node(const B_Node &other):bb(other.bb.center, other.bb.size, other.bb.xInit, other.bb.yInit, other.bb.zInit) {
	//low = other.low;
	//high = other.high;
	pointNumber = other.pointNumber;
	if (other.left != NULL)
		*left = *other.left;
	if (other.right != NULL)
		*right = *other.right;;
}

std::pair<BoundingBox*, BoundingBox*> MeshConstructor::checkCollsion(MeshConstructor* other, glm::mat4 hisTrans, glm::mat4 myTrans, glm::mat4 b1scale, glm::mat4 b2scale) {
	return boundingTree->isColiding(myTrans, other->boundingTree, hisTrans, 0, b1scale, b2scale);
}

//v2
bool B_Node::isColiding2(glm::mat4 myMat, B_Node* other, glm::mat4 otherMat, int lvl, glm::mat4 b1scale, glm::mat4 b2scale) {
	
	if (lvl > 6)
		return true;
	if (!bb.checkCollision(myMat, other->bb, otherMat, b1scale, b2scale))
		return  false;

	bool option1 = true;
	bool option2 = true;
	bool option3 = true;
	bool option4 = true;




	if(left != NULL &other->left!=NULL)
		option1 = left->isColiding2(myMat,other->left,otherMat,lvl+1,b1scale,b2scale);
	if (left != NULL & other->right != NULL)
		option2 = left->isColiding2(myMat,other->right,otherMat,lvl+1,b1scale,b2scale);
	if (right != NULL & other->left != NULL)
		option3 = right->isColiding2(myMat,other->left,otherMat,lvl+1,b1scale,b2scale);
	if (right != NULL & other->right != NULL)
		option4 = right->isColiding2(myMat,other->right,otherMat,lvl+1,b1scale,b2scale);

	if (!option1 & !option2 & !option3 & !option4)
		return false;
	return true;



}


bool MeshConstructor::checkCollsion2(MeshConstructor* other, glm::mat4 hisTrans, glm::mat4 myTrans, glm::mat4 b1scale, glm::mat4 b2scale) {
	return boundingTree->isColiding2(myTrans, other->boundingTree, hisTrans, 0, b1scale, b2scale);
}


//colstion-----------------------------------------------------------

MeshConstructor::MeshConstructor(const int type)
{
	IndexedModel myModle = AxisGenerator();
	switch (type)
	{
	case Axis:
	//	myModle = AxisGenerator();
		InitLine(myModle);
		break;
	case Cube:
		myModle = CubeTriangles();
		InitMesh(myModle);
		break;
	case Octahedron:
		myModle = OctahedronGenerator();
		InitMesh(myModle);
		break;
	case Tethrahedron:
		myModle = TethrahedronGenerator();
		InitMesh(myModle);
		break;
	case Minsara:
		myModle = MinsaraTriangles();
		InitMesh(myModle);
		break;
	default:
		break;

	}


	std::list<Node::vecType> point_list;
	for (auto i = 0; i < (myModle).positions.size(); i++)
	{

		point_list.push_back(Node::vecType((myModle).positions.at(i),1));
	}
	Kdtree kd;
	kd.makeTree(point_list);
	boundingTree = new B_Node(kd.getRoot(),glm::vec3(1,1,1),true);
}



MeshConstructor::MeshConstructor(const std::string& fileName)
{
	IndexedModel myModle = OBJModel(fileName).ToIndexedModel();
	InitMesh(myModle);

	std::list<Node::vecType> point_list;
	for (auto i = 0; i < (myModle).positions.size(); i++)
	{

		point_list.push_back(Node::vecType((myModle).positions.at(i), 1));
	}
	Kdtree kd;
	kd.makeTree(point_list);
	boundingTree = new B_Node(kd.getRoot(), glm::vec3(1, 1, 1), true);
}

MeshConstructor::MeshConstructor(Bezier1D *curve, bool isSurface, unsigned int resT, unsigned int resS)
{
	if (isSurface)
	{
		Bezier2D surface(*curve, glm::vec3(0, 0, 1), 4);
		IndexedModel myModle = surface.GetSurface(resT, resS);
		InitMesh(myModle);

		std::list<Node::vecType> point_list;
		for (auto i = 0; i < (myModle).positions.size(); i++)
		{
			
			point_list.push_back(Node::vecType((myModle).positions.at(i), 1));
		}
		Kdtree kd;
		kd.makeTree(point_list);

		boundingTree = new B_Node(kd.getRoot(), glm::vec3(1, 1, 1), true);
	}
	else
	{
		IndexedModel myModle = curve->GetLine(resT);
		InitLine(myModle);
		std::list<Node::vecType> point_list;
		for (auto i = 0; i < (myModle).positions.size(); i++)
		{

			point_list.push_back(Node::vecType((myModle).positions.at(i), 1));
		}
		Kdtree kd;
		kd.makeTree(point_list);
		boundingTree = new B_Node(kd.getRoot(), glm::vec3(1, 1, 1), true);
	}
}

MeshConstructor::MeshConstructor(const MeshConstructor &mesh)
{
	indicesNum = mesh.indicesNum;
	if (mesh.is2D)
		CopyMesh(mesh);
	else
		CopyLine(mesh);
}

MeshConstructor::~MeshConstructor(void)
{
	delete boundingTree;
	if (ib)
		delete ib;
	for (unsigned int i = 0; i < vbs.size(); i++)
	{
		if (vbs[i])
			delete vbs[i];
	}
}

void MeshConstructor::InitLine(IndexedModel &model) {

	int verticesNum = model.positions.size();
	indicesNum = model.indices.size();

	vao.Bind();

	for (int i = 0; i < 2; i++)
	{
		vbs.push_back(new VertexBuffer(model.GetData(i), verticesNum * sizeof(model.positions[0])));
		vao.AddBuffer(*vbs.back(), i, 3, GL_FLOAT);
	}

	ib = new IndexBuffer((unsigned int*)model.GetData(5), indicesNum);

	vao.Unbind();
	is2D = false;

}

void MeshConstructor::InitMesh(IndexedModel &model) {

	int verticesNum = model.positions.size();
	indicesNum = model.indices.size();

	vao.Bind();

	for (int i = 0; i < 4; i++)
	{
		vbs.push_back(new VertexBuffer(model.GetData(i), verticesNum * sizeof(model.positions[0])));
		vao.AddBuffer(*vbs.back(), i, 3, GL_FLOAT);
	}
	vbs.push_back(new VertexBuffer(model.GetData(4), verticesNum * sizeof(model.texCoords[0])));
	vao.AddBuffer(*vbs.back(), vbs.size() - 1, 2, GL_FLOAT);

	ib = new IndexBuffer((unsigned int*)model.GetData(5), indicesNum);

	vao.Unbind();
	is2D = true;

}

void MeshConstructor::CopyLine(const MeshConstructor &mesh) {

	vao.Bind();

	for (int i = 0; i < 2; i++)
	{
		vbs.push_back(new VertexBuffer(*(mesh.vbs[i])));
		vao.AddBuffer(*vbs.back(), i, 3, GL_FLOAT);
	}

	ib = new IndexBuffer(*mesh.ib);

	vao.Unbind();

	is2D = false;

}

void MeshConstructor::CopyMesh(const MeshConstructor &mesh) {

	vao.Bind();

	for (int i = 0; i < 4; i++)
	{
		vbs.push_back(new VertexBuffer(*(mesh.vbs[i])));
		vao.AddBuffer(*vbs.back(), i, 3, GL_FLOAT);
	}
	vbs.push_back(new VertexBuffer(*(mesh.vbs[3])));	
	vao.AddBuffer(*vbs.back(),3,2,GL_FLOAT);
	
	
	ib = new IndexBuffer(*mesh.ib);
	//ib = mesh.ib;
	vao.Unbind();

	is2D = true;

}
