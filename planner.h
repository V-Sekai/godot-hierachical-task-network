#pragma once

#include <utility>
#include <vector>

#include "core/io/resource.h"

#include "planning_problem.h"

class TotalOrderForwardDecomposition : public Resource {
	GDCLASS(TotalOrderForwardDecomposition, Resource);
public:
	using Plan = OperatorsWithParams;
	using RelevantMethods = PlanningProblem::RelevantMethods;
	using ApplicableOperators = PlanningProblem::ApplicableOperators;

	TotalOrderForwardDecomposition(const PlanningProblem &planningProblem);
	~TotalOrderForwardDecomposition();

	Plan try_to_plan();

private:
	Plan seek_plan(const std::vector<Task> &tasks, const State &p_current_state, Plan &currentPlan);
	Plan search_methods(const std::vector<Task> &tasks, const State &p_current_state, Plan &currentPlan);
	Plan search_operators(const std::vector<Task> &tasks, const State &p_current_state, Plan &currentPlan);

	const PlanningProblem m_planningProblem;
};
