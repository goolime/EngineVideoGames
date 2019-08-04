#include <iostream>
#include <fstream>
#include <vector>
#include <iterator>
#include <string>
#include <glm/glm.hpp>
#include <sstream>
#include <algorithm>

enum objecscode
{
	goal = 1,
	apple = 2,
	mine = 4,
	rampN = 8,
	rampW = 16,
	rampS = 32,
	rampE = 64,
	wall = 128,
	lvl0 = 256,
	lvl1 = 512,
	lvl2 = 1024,
	lvl3 = 2048,
};
struct obj
{
	// pos.x = startx \ pos.y = startz\ pos.z = sizex\ pos.w = sizez 
	glm::vec4 postions;
	int lvl;
	int type;

};

class CSVReader
{
	std::vector<obj> myobjects;
	std::string fileName;
	std::string delimeter;
	std::vector < std::vector<int>> data;

	std::vector < std::vector<int>>  getData();
public:
	int x = 0;
	int z = 0;
	CSVReader(std::string filename, std::string delm = ",") :
		fileName(filename), delimeter(delm)
	{
		data = getData();
	}
	// Function to fetch data from a CSV File
	
	std::vector<obj> getallobjects(int type);
	glm::vec4 getobject(int value, glm::vec2 start);

};
