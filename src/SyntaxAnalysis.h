#pragma once

#include "LexicalAnalysis.h"
#include "Token.h"
#include "IR.h"
#include "Variables.h"

class SyntaxAnalysis
{
public:

	/**
	* Constructor
	*/
	SyntaxAnalysis(LexicalAnalysis& lex, Variables& vars, Instructions& ins, Labels& labs);

	/**
	* Method which performs lexical analysis
	*/
	bool doSyntaxAnalysis();

	/**
	* Method which finds predecessors and successors
	*/
	void findPredAndSucc(Instructions& instructions);

	/**
	* Method which prints the instructions to console
	*/
	void printInstructions(Instructions instruc);

private:

	/**
	* Reference to lexical analysis module
	*/
public:
	// References to the variables, instructions and labels
	Variables& variables;
	Instructions& instructions;
	Labels& labels;

	/**
	* Prints the error message, and token that caused the syntax error
	*/
	void printSyntaxError(Token token);

	/**
	* Prints the token info
	*/
	void printTokenInfo(Token token);

	/**
	* Eats the current token if its type is "t"
	* otherwise reports syntax error
	*
	* param[in] - t - the expected token type
	*/
	void eat(TokenType t);

	/**
	* Returns the next token from the token list
	*/
	Token getNextToken();

	/**
	 * Nonterminal Q
	 */
	void Q();

	/**
	 * Nonterminal S
	 */
	void S();

	/**
	 * Nonterminal L
	 */
	void L();

	/**
	 * Nonterminal E
	 */
	void E();

	/**
	* Reference to lexical analysis module
	*/
	LexicalAnalysis& lexicalAnalysis;

	/**
	* Syntax error indicator
	*/
	bool errorFound;

	/**
	* Iterator to the token list which represents the output of the lexical analysis
	*/
	TokenList::iterator tokenIterator;

	/**
	* Current token that is being analyzed
	*/
	Token currentToken;

	// Current reg position and instruction position
	int regPosition = 0;
	int instructionPosition = 0;
};
