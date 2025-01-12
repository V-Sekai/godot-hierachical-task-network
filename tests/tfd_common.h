#pragma once

#include "../planning.h"
#include "../planning_problem.h"

PlanningDomain planning_domain;
State initial_state = { "test_domain", false };
Task top_level_task = { "test_method", {} };

std::optional<State> test_operator(const State &state, const std::vector<Variant> &parameters) {
	State newState(state);
	bool status = state.data;
	newState.data = !status;

	return newState;
}

std::optional<std::vector<Task>> test_method(const State &state, const std::vector<Variant> &parameters) {
	Task task;
	task.task_name = "test_operator";
	std::vector<Task> subtasks{ task };

	return subtasks;
}
