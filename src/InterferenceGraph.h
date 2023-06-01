#pragma once

#include "IR.h"

typedef std::vector<std::vector<int>> InterferenceMatrix;

class InterferenceGraph {
private:
	Variables* m_variables;		
	InterferenceMatrix m_matrix;	

public:
	InterferenceGraph(Variables v, Instructions& inst) : m_variables(calculateRegisterVariables(v))
	{
		makeInterferenceGraph(inst);
	}

	Variables* calculateRegisterVariables(Variables&);

	void setMatrix(InterferenceMatrix* mat) { m_matrix = *mat; }

	void printInterferenceMatrix();

	InterferenceMatrix& getMatrix() { 
		return m_matrix;
	}

	Variables& getVariables() {
		return *m_variables;
	}

	void makeInterferenceGraph(Instructions& instructions);
};
