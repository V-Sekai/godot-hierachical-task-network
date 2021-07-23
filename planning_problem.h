#pragma once

#include "planning_domain.h"
#include <memory>
#include <vector>

class PlanningProblem {
public:
	using RelevantMethods = MethodsWithParams;
	using ApplicableOperators = OperatorsWithParams;

	PlanningProblem(const PlanningDomain &p_domain, const State &p_initial_state, const Task &p_top_level_task);
	~PlanningProblem();

	bool task_is_operator(const std::string &p_task_name) const;
	bool task_is_method(const std::string &p_task_name) const;
	RelevantMethods get_methods_for_task(const Task &p_task, const State &p_current_state) const;
	ApplicableOperators get_operators_for_task(const Task &p_task, const State &p_current_state) const;
	State get_initial_state() const;
	Task get_top_level_task() const;

private:
	const PlanningDomain planning_domain;
	const State initial_state;
	const Task top_level_task;
};
