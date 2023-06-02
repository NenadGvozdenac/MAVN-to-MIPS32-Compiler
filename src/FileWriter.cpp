#include "FileWriter.h"
#include <sstream>
#include <exception>

void FileWriter::write() 
{
	try {
		std::vector<std::string> registers = { "", "$t0", "$t1", "$t2", "$t3", "$t4" };
		std::stringstream ss;

		ss << ".globl main\n\n";
		ss << ".data\n";
		for (Variable* variable : variables) {
			if (variable->getVariableType() == Variable::MEM_VAR)
				ss << variable->getName() << ":\t.word " << variable->getValue() << "\n";
		}

		ss << "\n.text\n";

		for (Instruction* instruction : instructions)
		{

			for (Label* label : labels)
			{
				if (label->getPosition() == instruction->m_position)
				{
					ss << label->getNameOfLabel() << ":\n";
				}
			}

			std::string tempString = instruction->m_instructionTemplate + "\n";

			ss << "\t";
			while (tempString.find("`d") != std::string::npos)
			{
				int commandPos = tempString.find("`d");
				tempString.replace(commandPos, 2, registers[instruction->getDst().front()->getVariableAssignment()]);
			}

			while (tempString.find("`s") != std::string::npos)
			{
				int commandPos = tempString.find("`s");
				tempString.replace(commandPos, 2, instruction->getSrc().front()->getVariableType() == Variable::MEM_VAR ? instruction->getSrc().front()->getName() : registers[instruction->getSrc().front()->getVariableAssignment()]);
				instruction->m_src.remove(instruction->getSrc().front());
			}

			while (tempString.find("`i") != std::string::npos)
			{
				int commandPos = tempString.find("`i");
				tempString.replace(commandPos, 2, std::to_string(instruction->m_number));
			}

			while (tempString.find("`n") != std::string::npos)
			{
				int commandPos = tempString.find("`n");
				tempString.replace(commandPos, 2, instruction->m_label->getNameOfLabel());
			}

			ss << tempString;
		}

		std::ofstream file(this->fileName);

		if (!file.is_open()) {
			throw new std::runtime_error("\nException! Syntax Analysis failed!\n");
		}

		std::cout << "\n\n" << ss.str() << "\n\n";

		file << ss.str();

		file.close();
	}
	catch (std::runtime_error e) {
		std::cerr << e.what() << std::endl;
	}
}