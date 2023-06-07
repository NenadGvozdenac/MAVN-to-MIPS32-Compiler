// Autor: Nenad Gvozdenac, Datum: 07.06.2023

#include "FileWriter.h"
#include <sstream>
#include <exception>

FileWriter::FileWriter(std::string fileName, Instructions& instructions, Variables& variables, Labels& labels) :
	fileName(fileName),
	instructions(instructions),
	variables(variables),
	labels(labels)
{}

void FileWriter::write() 
{
	try 
	{
		std::ofstream file(this->fileName);											// Create ofstream file

		if (!file.is_open()) {														// Throw error in case the file couldn't be open
			throw new std::runtime_error("\nException! Output file couldn't be open!\n");
		}

		std::vector<std::string> registers = { "$t0", "$t1", "$t2", "$t3", "$t4" }; // Register values
		std::stringstream ss;														// Stringstream object, used for easy and memory-efficient addition of elements into memory

		ss << ".globl " << (*(labels.begin()))->getNameOfLabel() << "\n\n";			// We add the entry point into the program (first label)
		ss << ".data\n";															// Add data section

		for (Variable* variable : variables) 
		{																			// We add all the memory variables into the memory section (under .data)
			if (variable->getVariableType() == Variable::MEM_VAR)
				ss << variable->getName() << ":\t.word " << variable->getValue() << "\n";
		}

		ss << "\n.text\n";															// Add text section

		for (Instruction* instruction : instructions)
		{
			for (Label* label : labels)
			{
				if (label->getPosition() == instruction->m_position)				// First output the label name. It should have the same position as the instruction position
				{
					ss << label->getNameOfLabel() << ":\n";
					break;
				}
			}

			std::string tempString = instruction->m_instructionTemplate + "\n";		// We create an object that has the template [<instruction> <dest. reg> <src. reg>]. 
																					// Later we replace dest and src registers with actual registers

			ss << "\t";
			while (tempString.find("`d") != std::string::npos)						// While tempString has `d (destination registers)
			{
				int commandPos = tempString.find("`d");								// Position of the register
				tempString.replace(commandPos, 2, registers[instruction->getDst().front()->getVariableAssignment() - 1]);
			}

			while (tempString.find("`s") != std::string::npos)						// While tempString has `s (source registers)
			{
				int commandPos = tempString.find("`s");								// Position of the source register
																					// Replace it (if its memory register, switch it with the name (for example m1), 
																					// or alternatevely, switch with the register from the registers vector
				tempString.replace(commandPos, 2, instruction->getSrc().front()->getVariableType() == Variable::MEM_VAR ? instruction->getSrc().front()->getName() : registers[instruction->getSrc().front()->getVariableAssignment() - 1]);
				
				instruction->m_src.remove(instruction->getSrc().front());			// Pop the register, so we don't read it twice
			}

			while (tempString.find("`i") != std::string::npos)						// While tempString has `i (literal number)
			{
				int commandPos = tempString.find("`i");								// Position of the number
																					// Replace with the number of number of the instruction
				tempString.replace(commandPos, 2, std::to_string(instruction->m_number));
			}

			while (tempString.find("`n") != std::string::npos)						// While tempString has `n (label)
			{
				int commandPos = tempString.find("`n");								// Position of the label
																					// Replace with the name of the label
				tempString.replace(commandPos, 2, instruction->m_label->getNameOfLabel());
			}

			ss << tempString;														// Write into the stringstream
		}

		std::cout << "\n\n" << ss.str() << "\n\n";									// Output the instruction to console

		file << ss.str();															// Output the instruction to file

		file.close();																// Close the output file
	}
	catch (std::runtime_error e) {
		std::cerr << e.what() << std::endl;
	}
}