#include "planner.h"
#include <iostream>
#include <ostream>

TotalOrderForwardDecomposition::TotalOrderForwardDecomposition(const PlanningProblem &planningProblem) :
		m_planningProblem(planningProblem) {
}

TotalOrderForwardDecomposition::~TotalOrderForwardDecomposition() {}

TotalOrderForwardDecomposition::Plan TotalOrderForwardDecomposition::try_to_plan() {
	std::vector<Task> tasks;
	Plan solutionPlan;
	tasks.push_back(m_planningProblem.get_top_level_task());

	std::cout << "Try To Plan for: " << tasks.back().task_name << "\n";

	return seek_plan(tasks, m_planningProblem.get_initial_state(), solutionPlan);
}

TotalOrderForwardDecomposition::Plan TotalOrderForwardDecomposition::seek_plan(const std::vector<Task> &tasks, const State &p_current_state, Plan &currentPlan) {
	if (tasks.empty()) {
		std::cout << "SeekPlan: No more tasks, returning current plan.\n";
		if (!currentPlan.empty()) {
			std::cout << "TFD found solution plan." << std::endl;
			for (const OperatorWithParams &operatorWithParams : currentPlan) {
				std::cout << operatorWithParams.task.task_name;
			}
		}
		return currentPlan;
	}

	if (m_planningProblem.task_is_operator(tasks.back().task_name)) {
		std::cout << "SeekPlan: Task is operator type.\n";
		return search_operators(tasks, p_current_state, currentPlan);
	}

	if (m_planningProblem.task_is_method(tasks.back().task_name)) {
		std::cout << "SeekPlan: Task is method type.\n";
		return search_methods(tasks, p_current_state, currentPlan);
	}

	return {};
}

TotalOrderForwardDecomposition::Plan TotalOrderForwardDecomposition::search_methods(const std::vector<Task> &tasks, const State &p_current_state, Plan &currentPlan) {
	std::cout << "SearchMethods for " << tasks.back().task_name;
	RelevantMethods relevantMethods = m_planningProblem.get_methods_for_task(tasks.back(), p_current_state);

	if (!relevantMethods.empty()) {
		std::cout << "SearchMethods: " << relevantMethods.size() << " relevant methods found.\n";

		for (const MethodWithParams &relevantMethod : relevantMethods) {
			std::optional<std::vector<Task>> subTasks = relevantMethod.func(p_current_state, relevantMethod.task.parameters);
			if (subTasks && !subTasks.value().empty()) {
				std::vector<Task> newTasks(tasks);
				newTasks.pop_back();
				newTasks.insert(newTasks.end(), subTasks.value().begin(), subTasks.value().end());

				std::vector<OperatorWithParams> solution = seek_plan(newTasks, p_current_state, currentPlan);
				if (!solution.empty()) {
					return solution;
				}
			}
		}
	} else {
		std::cout << "SearchMethods: No relevant methods found.\n";
	}

	std::cout << "SearchMethods: Failed to plan";
	return {};
}

TotalOrderForwardDecomposition::Plan TotalOrderForwardDecomposition::search_operators(const std::vector<Task> &tasks, const State &p_current_state, Plan &currentPlan) {
	std::cout << "SearchOperators for " << tasks.back().task_name;
	ApplicableOperators applicableOperators = m_planningProblem.get_operators_for_task(tasks.back(), p_current_state);

	if (!applicableOperators.empty()) {
		for (const OperatorWithParams &chosenOperator : applicableOperators) {
			const std::optional<State> newState = chosenOperator.func(p_current_state, chosenOperator.task.parameters);

			if (newState) {
				std::vector<Task> newTasks(tasks);
				newTasks.pop_back();
				currentPlan.push_back(chosenOperator);

				std::vector<OperatorWithParams> solution = seek_plan(newTasks, newState.value(), currentPlan);
				if (!solution.empty()) {
					return solution;
				}
			}
		}
	} else {
		std::cout << "SearchOperators: No applicable operator found.\n";
	}

	return {};
}