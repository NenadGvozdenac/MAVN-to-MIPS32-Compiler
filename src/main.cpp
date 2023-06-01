#include "LexicalAnalysis.h"
#include "SyntaxAnalysis.h"
#include "LivenessAnalysis.h"
#include "IR.h"
#include "InterferenceGraph.h"
#include "SimplificationStack.h"
#include "ResourceAllocation.h"
#include "FileWriter.h"

#include <iostream>
#include <exception>
#include <assert.h>

using namespace std;

void TestirajKompajliranje(std::string inputFile, std::string outputFile) 
{
	try
	{
		bool retVal = false;

		LexicalAnalysis lex;

		if (!lex.readInputFile(inputFile))
			throw runtime_error("\nException! Failed to open input file!\n");

		lex.initialize();

		retVal = lex.doLexicalAnalysis();

		if (retVal)
		{
			cout << endl;
			cout << "------------------------------------------------------------------------" << endl;
			cout << "------------------------------------------------------------------------" << endl;
			cout << "--------------- Lexical analysis finished successfully! ----------------" << endl;
			cout << "------------------------------------------------------------------------" << endl;
			cout << "------------------------------------------------------------------------" << endl << endl;
			lex.printTokens();
		}
		else 
		{
			lex.printLexError();
			cout << endl;
			cout << "------------------------------------------------------------------------" << endl;
			cout << "------------------------------------------------------------------------" << endl;
			cout << "----------------------------- EXCEPTION! -------------------------------" << endl;
			cout << "----------------------- Lexical analysis failed! -----------------------" << endl;
			cout << "------------------------------------------------------------------------" << endl << endl;
			throw new runtime_error("\nException! Lexical Analysis failed!\n");
		}

		Variables variables;
		Instructions instructions;
		Labels labels;

		SyntaxAnalysis syntaxAnalysis(lex, variables, instructions, labels);

		retVal = syntaxAnalysis.doSyntaxAnalysis();

		if (retVal) 
		{
			cout << endl;
			cout << "------------------------------------------------------------------------" << endl;
			cout << "------------------------------------------------------------------------" << endl;
			cout << "--------------- Syntax analysis finished successfully! -----------------" << endl;
			cout << "------------------------------------------------------------------------" << endl;
			cout << "------------------------------------------------------------------------" << endl << endl;
		}
		else {
			cout << endl;
			cout << "------------------------------------------------------------------------" << endl;
			cout << "------------------------------------------------------------------------" << endl;
			cout << "----------------------------- EXCEPTION! -------------------------------" << endl;
			cout << "----------------------- Syntax analysis failed! ------------------------" << endl;
			cout << "------------------------------------------------------------------------" << endl << endl;
			throw new runtime_error("\nException! Syntax Analysis failed!\n");
		}

		printAll(variables, instructions, labels);

		syntaxAnalysis.findPredAndSucc(instructions);
		syntaxAnalysis.printInstructions(instructions);

		doLivenessAnalysis(instructions);

		cout << endl << endl;
		cout << "------------------------------------------------------------------------" << endl;
		cout << "------------------------------------------------------------------------" << endl;
		cout << "-------------- Instruction list after LIVENESS ANALYSIS: ---------------" << endl;
		cout << "------------------------------------------------------------------------" << endl;
		cout << "------------------------------------------------------------------------" << endl << endl;
		syntaxAnalysis.printInstructions(syntaxAnalysis.instructions);

		InterferenceGraph interferenceGraph(syntaxAnalysis.variables, syntaxAnalysis.instructions);
		interferenceGraph.printInterferenceMatrix();

		SimplificationStack simplificationStack;
		simplificationStack = doSimplification(interferenceGraph, __REG_NUMBER__);
		bool resourceAllocationSuccessfull = doResourceAllocation(simplificationStack, interferenceGraph);

		if (resourceAllocationSuccessfull) 
		{
			cout << endl;
			cout << "------------------------------------------------------------------------" << endl;
			cout << "------------------------------------------------------------------------" << endl;
			cout << "-------------- Resource allocation finished successfuly ----------------" << endl;
			cout << "------------------------------------------------------------------------" << endl;
			cout << "------------------------------------------------------------------------" << endl << endl;
		}
		else 
		{
			cout << endl;
			cout << "------------------------------------------------------------------------" << endl;
			cout << "------------------------------------------------------------------------" << endl;
			cout << "------------- Resource allocation NOT FINISHED successfuly  ------------" << endl;
			cout << "------------------------------------------------------------------------" << endl;
			cout << "------------------------------------------------------------------------" << endl << endl;
			throw new runtime_error("\nException! Resource allocation failed!\n");
		}

		cout << endl;
		cout << "------------------------------------------------------------------------" << endl;
		cout << "------------------------------------------------------------------------" << endl;
		cout << "------- Instruction list after UPDATE FROM RESOURCE ALLOCATION: --------" << endl;
		cout << "------------------------------------------------------------------------" << endl;
		cout << "------------------------------------------------------------------------" << endl << endl;

		Instruction::normalizeAssignmentsToVariables(instructions, variables);
		syntaxAnalysis.printInstructions(syntaxAnalysis.instructions);

		FileWriter* writerSimple = new FileWriter(outputFile, syntaxAnalysis.instructions, syntaxAnalysis.variables, syntaxAnalysis.labels);

		cout << "------------------------------------------------------------------------" << endl;
		cout << "------------------------------------------------------------------------" << endl;
		cout << "----------------- Complete overview of the simple code -----------------" << endl;
		cout << "------------------------------------------------------------------------" << endl;
		cout << "------------------------------------------------------------------------" << endl;
		writerSimple->write();

		cout << endl;
		cout << "------------------------------------------------------------------------" << endl;
		cout << "------------------------------------------------------------------------" << endl;
		cout << "------------------- Thank you for using my program! --------------------" << endl;
		cout << "------------------------------- NenadG ---------------------------------" << endl;
		cout << "------------------------------------------------------------------------" << endl;

		delete(writerSimple);
	}
	catch (runtime_error e)
	{
		cout << e.what() << endl;
		return;
	}
}

int main()
{
	std::string simpleFajlInput = "../examples/simple.mavn";
	std::string simpleFajlOutput = "../examples/simple.S";

	std::string multiplyFajlInput = "../examples/multiply.mavn";
	std::string multiplyFajlOutput = "../examples/multiply.S";

	TestirajKompajliranje(simpleFajlInput, simpleFajlOutput);
	// TestirajKompajliranje(multiplyFajlInput, simpleFajlOutput);

	return 0;
}
