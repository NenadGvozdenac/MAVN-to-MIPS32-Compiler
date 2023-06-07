// Autor: Nenad Gvozdenac, Datum: 07.06.2023

#pragma once

#include "IR.h"

// InterferenceMatrix that consists of an array of an array of vectors (should be NxN matrix)
typedef std::vector<std::vector<int>> InterferenceMatrix;

// Class that is used to interact with the interference graph
class InterferenceGraph {
private:
	// Data
	Variables* m_variables;			// Variables stored in the graph
	InterferenceMatrix m_matrix;	// Matrix stored in the graph

public:
	// Constructor for the interference graph
	InterferenceGraph(Variables v, Instructions& inst);

	// Function that goes through all variables and checks whether they are registers or not
	Variables* calculateRegisterVariables(Variables&);

	// Function that prints the interference matrix to the console
	void printInterferenceMatrix();

	// Function that retrieves the matrix of this object
	InterferenceMatrix& getMatrix();

	// Function that retrieves the variables of this object
	Variables& getVariables();

	void makeInterferenceGraph(Instructions& instructions);
};
