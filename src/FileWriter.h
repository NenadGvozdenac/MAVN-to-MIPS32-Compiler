// Autor: Nenad Gvozdenac, Datum: 07.06.2023

#pragma once
#include <fstream>

#include "IR.h"

// Class used to represent the writer for writing into the .S file
class FileWriter {
private:
	// Data
	std::string fileName;
	Instructions& instructions;
	Variables& variables;
	Labels& labels;
public:
	// Constructor
	FileWriter(std::string fileName, Instructions& instructions, Variables& variables, Labels& labels);

	// Function used to write into the file (fileName), the instructions, variables and labels
	void write();
};