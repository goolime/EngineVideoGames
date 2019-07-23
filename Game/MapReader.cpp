#include "MapReader.h"


glm::vec4 CSVReader::getobject(int value, glm::vec2 start) {
	int x = 0, z = -1;
	while (start.x + x < data[start.y].size() && (data[(start.y)][(start.x + x)] & value) != 0) {
		x++;
		
	}

	bool testrow = true;
	
	while (testrow)
	{
		for (int i = 0; i < x; i++)
		{
			data[(start.y + z + 1)][(start.x + i)] = data[(start.y + z + 1)][(start.x + i)] - value; // delete item from here
		}

		z++;
		for (int i = 0; i < x & testrow; i++) {

			if (start.y + z + 1 >= data.size() || (data[(start.y + z + 1)][(start.x + i)] & value) == 0)
				testrow = false;
		}
		
	}
	return glm::vec4(start.x, start.y, x, z);
}

std::vector<obj> CSVReader::getallobjects(int type) {
	std::vector<obj> output;
	int x = 0, y = 0;
	for (; y < data.size(); y++)
	{
		for (x = 0; x < data[y].size(); x++)
		{
			if ((data[y][x] & type) != 0) {
				obj newobj;
				newobj.lvl = data[y][x] / (2*lvl0-1);
				newobj.postions = getobject(type, glm::vec2(x, y));
				newobj.type = type;
				output.push_back(newobj);
			}
			//	x = newobj.postions.z;// end of x
		}
	}
	return output;
}
std::vector < std::vector<int>>  CSVReader::getData()
{
	std::ifstream file(fileName);
	std::vector < std::vector<int > >dataList;
	std::string line = "";
	while (getline(file, line))
	{
		std::string  tmp = "";

		std::stringstream ss(line);
		std::vector<int> words;

		while (getline(ss, tmp, ','))
			words.push_back(stoi(tmp));


		dataList.push_back(words);
	}
	file.close();
	z = dataList.size();
	if(z>0)
		x = dataList[0].size();
	return dataList;
}
