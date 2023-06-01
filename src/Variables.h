#pragma once

#include <string>

class VariableTwo
{

public:

	VariableTwo() : m_name("") {}

	VariableTwo(std::string name) : m_name(name) {}

	/**
	* Set method for variable name
	*/
	void setName(std::string name) { m_name = name; }

	/**
	* Get method for variable name
	*/
	std::string getName() { return m_name; }

private:

	/**
	* Name of the variable
	*/
	std::string m_name;
};