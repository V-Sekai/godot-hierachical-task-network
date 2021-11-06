#pragma once

#include "planning_problem.h"

#include <utility>
#include <vector>

class TaskPlanner {
public:
	using Plan = std::vector<OperatorWithParams>;
	using RelevantMethods = PlanningProblem::RelevantMethods;
	using ApplicableOperators = PlanningProblem::ApplicableOperators;

	void set_planning_problem(PlanningProblem p_planning_problem);
	PlanningProblem get_planning_problem() const;
	Vector<OperatorWithParams> plan();

private:

	Plan try_to_plan();
	Plan seek_plan(const std::vector<Task> &p_tasks, const State &p_current_state, Plan &p_current_plan);
	Plan search_methods(const std::vector<Task> &p_tasks, const State &p_current_state, Plan &p_current_plan);
	Plan search_operators(const std::vector<Task> &p_tasks, const State &p_current_state, Plan &p_current_plan);

	PlanningProblem planning_problem;
};
