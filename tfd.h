// Total-order Forward Decomposition Algorithm
#pragma once

#include "planning_problem.h"

#include <utility>
#include <vector>

class TotalOrderForwardDecomposition {
public:
	using Plan = OperatorsWithParams;
	using RelevantMethods = PlanningProblem::RelevantMethods;
	using ApplicableOperators = PlanningProblem::ApplicableOperators;

	TotalOrderForwardDecomposition(const PlanningProblem &planningProblem);
	~TotalOrderForwardDecomposition();

	Plan try_to_plan();

private:
	Plan seek_plan(const std::vector<Task> &tasks, const State &currentState, Plan &currentPlan);
	Plan search_methods(const std::vector<Task> &tasks, const State &currentState, Plan &currentPlan);
	Plan search_operators(const std::vector<Task> &tasks, const State &currentState, Plan &currentPlan);

	const PlanningProblem m_planningProblem;
};
