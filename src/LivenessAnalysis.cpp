#include "LivenessAnalysis.h"
#include <set>

using std::list;

bool variableExists(Variable* variable, Variables variables)
{
	Variables::iterator v_it;

	for (v_it = variables.begin(); v_it != variables.end(); v_it++)
	{
		if ((*v_it)->getName() == variable->getName())
		{
			return true;
		}
	}
	return false;
}

/**
 * @brief Does Liveness Analysis of parameters.
 * @param Instructions - std::list<Instruction*> - instructions holding Variables needing to be checked.
 */
void doLivenessAnalysis(Instructions& instructions)
{

	const auto funcEquals = ([](Variable* A, Variable* B) -> bool {
		return A->getName() == B->getName();
	});

	const auto funcSort = [](Variable* A, Variable* B) -> bool {return A->getName() < B->getName(); };

	bool provera = true;

	for (auto i_r_it = instructions.rbegin(); i_r_it != instructions.rend(); i_r_it++)
	{
		(*i_r_it)->m_in.clear();
		(*i_r_it)->m_out.clear();
	}

	while (provera) {

		provera = false;

		for (auto i_r_it = instructions.rbegin(); i_r_it != instructions.rend(); i_r_it++) {

			Variables in_c = (*i_r_it)->m_in;
			in_c.sort(funcSort);
			in_c.unique(funcEquals);

			Variables out_c = (*i_r_it)->m_out;
			out_c.sort(funcSort);
			out_c.unique(funcEquals);

			for (auto i_it = (*i_r_it)->m_succ.begin(); i_it != (*i_r_it)->m_succ.end(); i_it++)
			{
				for (auto v_it = (*i_it)->m_in.begin(); v_it != (*i_it)->m_in.end(); v_it++)
				{
					(*i_r_it)->m_out.push_back((*v_it));
				}
			}

			(*i_r_it)->m_out.sort(funcSort);
			(*i_r_it)->m_out.unique(funcEquals);


			(*i_r_it)->m_in = (*i_r_it)->m_use;


			for (auto v_it = (*i_r_it)->m_out.begin(); v_it != (*i_r_it)->m_out.end(); v_it++)
			{
				if (!variableExists(*v_it, (*i_r_it)->m_def))
				{
					(*i_r_it)->m_in.push_back((*v_it));
				}
			}

			(*i_r_it)->m_in.sort(funcSort);
			(*i_r_it)->m_in.unique(funcEquals);

			if ((*i_r_it)->m_in != in_c || (*i_r_it)->m_out != out_c) {
				provera = true;
			}
		}

		std::cout << "------------------------------------------------------------------------" << std::endl;
		std::cout << "------------------------------------------------------------------------" << std::endl;
		std::cout << "-------------------- Liveness Analysis Lap Finished! -------------------" << std::endl;
		std::cout << "------------------------------------------------------------------------" << std::endl;
		std::cout << "------------------------------------------------------------------------" << std::endl << std::endl;

	}
}

