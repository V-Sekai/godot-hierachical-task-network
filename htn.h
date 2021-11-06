#pragma once

#include "planning_problem.h"

#include <utility>
#include <vector>

class TaskPlanner {
public:
	using Plan = OperatorsWithParams;
	using RelevantMethods = PlanningProblem::RelevantMethods;
	using ApplicableOperators = PlanningProblem::ApplicableOperators;

	void set_planning_problem(PlanningProblem p_planning_problem) {
		planning_problem = p_planning_problem;
	}
	PlanningProblem get_planning_problem() const {
		return planning_problem;
	}
	Vector<OperatorWithParams> plan() {
		Vector<OperatorWithParams> result;
		const Plan plan = try_to_plan();
		for(const OperatorWithParams& op : plan) {
			result.push_back(op);
		}
		return result;
	}

private:

	Plan try_to_plan();
	Plan seek_plan(const std::vector<Task> &p_tasks, const State &p_current_state, Plan &p_current_plan);
	Plan search_methods(const std::vector<Task> &p_tasks, const State &p_current_state, Plan &p_current_plan);
	Plan search_operators(const std::vector<Task> &p_tasks, const State &p_current_state, Plan &p_current_plan);

	PlanningProblem planning_problem;
};
