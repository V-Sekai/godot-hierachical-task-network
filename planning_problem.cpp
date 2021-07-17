#include "planning_problem.h"

PlanningProblem::PlanningProblem(const PlanningDomain &p_domain,
		const State &initialState,
		const Task &topLevelTask) :
		PLANNING_DOMAIN(p_domain),
		INITIAL_STATE(initialState),
		TOP_LEVEL_TASK(topLevelTask) {
}

PlanningProblem::~PlanningProblem() {
}

bool PlanningProblem::task_is_operator(const std::string &p_task_name) const {
	return PLANNING_DOMAIN.task_is_operator(p_task_name);
}

bool PlanningProblem::task_is_method(const std::string &p_task_name) const {
	return PLANNING_DOMAIN.task_is_method(p_task_name);
}

PlanningProblem::RelevantMethods PlanningProblem::get_methods_for_task(const Task &p_task, const State &p_current_state) const {
	const auto relevantMethods = PLANNING_DOMAIN.get_relevant_methods(p_current_state, p_task);
	if (relevantMethods) {
		return relevantMethods.value();
	} else {
		return {};
	}
}

PlanningProblem::ApplicableOperators PlanningProblem::get_operators_for_task(const Task &p_task, const State &p_current_state) const {
	const auto applicableOperators = PLANNING_DOMAIN.get_applicable_operators(p_current_state, p_task);
	if (applicableOperators) {
		return applicableOperators.value();
	} else {
		return {};
	}
}

State PlanningProblem::get_initial_state() const {
	return INITIAL_STATE;
}

Task PlanningProblem::get_top_level_task() const {
	return TOP_LEVEL_TASK;
}