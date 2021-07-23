#pragma once

#include "planning_problem.h"

#include <utility>
#include <vector>

class TotalOrderForwardDecomposition {
public:
	using Plan = OperatorsWithParams;
	using RelevantMethods = PlanningProblem::RelevantMethods;
	using ApplicableOperators = PlanningProblem::ApplicableOperators;

	TotalOrderForwardDecomposition(const PlanningProblem &p_planning_problem);
	~TotalOrderForwardDecomposition();

	Plan try_to_plan();

private:
	Plan seek_plan(const std::vector<Task> &p_tasks, const State &p_current_state, Plan &p_current_plan);
	Plan search_methods(const std::vector<Task> &p_tasks, const State &p_current_state, Plan &p_current_plan);
	Plan search_operators(const std::vector<Task> &p_tasks, const State &p_current_state, Plan &p_current_plan);

	const PlanningProblem planning_problem;
};
