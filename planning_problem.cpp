#include "planning_problem.h"

bool PlanningProblem::task_is_operator(const String &p_task_name) const {
	return planning_domain.task_is_operator(p_task_name);
}

bool PlanningProblem::task_is_method(const String &p_task_name) const {
	return planning_domain.task_is_method(p_task_name);
}

std::vector<MethodWithParams> PlanningProblem::get_methods_for_task(const Task &p_task, const State &p_current_state) const {
	const auto relevantMethods = planning_domain.get_relevant_methods(p_current_state, p_task);
	if (relevantMethods) {
		return *relevantMethods;
	} else {
		return {};
	}
}

std::vector<OperatorWithParams> PlanningProblem::get_operators_for_task(const Task &p_task, const State &p_current_state) const {
	const auto applicableOperators = planning_domain.get_applicable_operators(p_current_state, p_task);
	if (applicableOperators.size()) {
		return applicableOperators;
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

void PlanningProblem::set_planning_domain(PlanningDomain p_domain) {
	planning_domain = p_domain;
}

PlanningDomain PlanningProblem::get_planning_domain() const {
	return planning_domain;
}

void PlanningProblem::set_initial_state(State p_state) {
	initial_state = p_state;
}

void PlanningProblem::set_top_level_task(Task p_top_level_task) {
	top_level_task = p_top_level_task;
}
