#pragma once

#include "planning_domain.h"
#include <memory>
#include <vector>

class PlanningProblem {
public:
	using RelevantMethods = MethodsWithParams;
	using ApplicableOperators = OperatorsWithParams;
	void set_planning_domain(PlanningDomain p_domain);
	PlanningDomain get_planning_domain() const;
	void set_initial_state(State p_state);
	void set_top_level_task(Task p_top_level_task);
	bool task_is_operator(const StringName &p_task_name) const;
	bool task_is_method(const StringName &p_task_name) const;
	RelevantMethods get_methods_for_task(const Task &p_task, const State &p_current_state) const;
	ApplicableOperators get_operators_for_task(const Task &p_task, const State &p_current_state) const;
	State get_initial_state() const;
	Task get_top_level_task() const;

private:
	PlanningDomain planning_domain;
	State initial_state;
	Task top_level_task;
};
