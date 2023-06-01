#pragma once

#include "IR.h"
#include <fstream>


class FileWriter {
private:
	std::string fileName;
	Instructions& instructions;
	Variables& variables;
	Labels& labels;
public:
	FileWriter(std::string fileName, Instructions& instructions, Variables& variables, Labels& labels) : 
		fileName(fileName), 
		instructions(instructions),
		variables(variables),
		labels(labels)
	{}

	void write();
};