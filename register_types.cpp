#include "register_types.h"

#include "planner.h"

std::optional<State> new_operator(const State &state, const Parameters &parameters) {
	State newState(state);
	bool status = !std::any_cast<bool>(state.data);
	newState.data = status;

	return newState;
}

std::optional<std::vector<Task>> new_method(const State &state, const Parameters &parameters) {
	Task task;
	task.task_name = "operator";
	task.parameters.push_back(true);
	std::vector<Task> subtasks{ task };

	return subtasks;
}

void register_tfd_types() {
	ClassDB::register_class<TotalOrderForwardDecomposition>();
	ClassDB::register_class<PlanningDomain>();
	ClassDB::register_class<State>();
	ClassDB::register_class<Task>();
	PlanningDomain planning_domain_plan = PlanningDomain("domain");
	State initial_state_plan = { "domain", false };
	Task top_level_task_plan = { "method", {} };

	planning_domain_plan.add_operator("operator", new_operator);
	planning_domain_plan.add_method("method", new_method);

	PlanningProblem planning_problem(planning_domain_plan, initial_state_plan, top_level_task_plan);
	Ref<TotalOrderForwardDecomposition> planner = memnew(TotalOrderForwardDecomposition(planning_problem));
	planner->try_to_plan();
}

void unregister_tfd_types() {
}