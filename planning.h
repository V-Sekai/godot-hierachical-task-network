#pragma once

#include "planning_problem.h"

#include <utility>
#include <vector>

#include "core/object/object.h"
#include "core/object/ref_counted.h"

class TaskPlanner : public RefCounted {
	GDCLASS(TaskPlanner, RefCounted);

public:
	void set_planning_problem(PlanningProblem p_planning_problem);
	PlanningProblem get_planning_problem() const;
	Vector<OperatorWithParams> plan();

private:
	std::vector<OperatorWithParams> try_to_plan();
	std::vector<OperatorWithParams> seek_plan(const std::vector<Task> &p_tasks, const State &p_current_state, std::vector<OperatorWithParams> &p_current_plan);
	std::vector<OperatorWithParams> search_methods(const std::vector<Task> &p_tasks, const State &p_current_state, std::vector<OperatorWithParams> &p_current_plan);
	std::vector<OperatorWithParams> search_operators(const std::vector<Task> &p_tasks, const State &p_current_state, std::vector<OperatorWithParams> &p_current_plan);

	PlanningProblem planning_problem;
};
