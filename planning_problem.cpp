#include "planning_problem.h"

bool PlanningProblem::task_is_operator(const StringName &p_task_name) const {
	return planning_domain.task_is_operator(p_task_name);
}

bool PlanningProblem::task_is_method(const StringName &p_task_name) const {
	return planning_domain.task_is_method(p_task_name);
}

PlanningProblem::RelevantMethods PlanningProblem::get_methods_for_task(const Task &p_task, const State &p_current_state) const {
	const auto relevantMethods = planning_domain.get_relevant_methods(p_current_state, p_task);
	if (relevantMethods) {
		return *relevantMethods;
	} else {
		return {};
	}
}

PlanningProblem::ApplicableOperators PlanningProblem::get_operators_for_task(const Task &p_task, const State &p_current_state) const {
	const auto applicableOperators = planning_domain.get_applicable_operators(p_current_state, p_task);
	if (applicableOperators) {
		return *applicableOperators;
	} else {
		return {};
	}
}

State PlanningProblem::get_initial_state() const {
	return initial_state;
}

Task PlanningProblem::get_top_level_task() const {
	return top_level_task;
}
