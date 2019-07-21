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
	snake = 1,
	snakebody = 2,
	ramp = 4,
	wall = 8,
	apple = 16,
	Gate = 32,
	lvl0 = 128,
	lvl1 = 256,
	lvl2 = 512,
	lvl3 = 1024,
};
struct obj
{
	glm::vec4 postions;
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
	CSVReader(std::string filename, std::string delm = ",") :
		fileName(filename), delimeter(delm)
	{
		data = getData();
	}
	// Function to fetch data from a CSV File
	
	std::vector<obj> getallobjects(int type);
	glm::vec4 getobject(int value, glm::vec2 start);

};
