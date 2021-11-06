#include "planning_domain.h"

void PlanningDomain::add_operator(const StringName &p_task_name, const OperatorFunction &p_operatorFunc) {
	auto operators = operator_table.find(p_task_name);

	if (operators == operator_table.end()) {
		operator_table[p_task_name] = Operators{ p_operatorFunc };
		return;
	}
	operators->second.push_back(p_operatorFunc);
}

void PlanningDomain::add_method(const StringName &p_task_name, const MethodFunction &p_method_func) {
	auto methods = method_table.find(p_task_name);

	if (methods == method_table.end()) {
		method_table[p_task_name] = Methods{ p_method_func };
		return;
	}
	methods->second.push_back(p_method_func);
}

std::optional<OperatorsWithParams> PlanningDomain::get_applicable_operators(const State &p_current_state, const Task &p_task) const {
	OperatorsWithParams operatorsWithParams;

	if (operator_table.empty()) {
		return std::nullopt;
	}

	for (const auto &element : operator_table) {
		if (element.first == p_task.task_name) {
			for (const auto &_operator : element.second) {
				if (_operator(p_current_state, p_task.parameters)) {
					operatorsWithParams.emplace_back(p_task, _operator);
				}
			}
			break;
		}
	}

	return operatorsWithParams;
}

std::optional<MethodsWithParams> PlanningDomain::get_relevant_methods(const State &p_current_state, const Task &p_task) const {
	MethodsWithParams methodsWithParams;

	if (method_table.empty()) {
		return std::nullopt;
	}

	for (const auto &element : method_table) {
		if (element.first == p_task.task_name) {
			for (const auto &method : element.second) {
				if (method(p_current_state, p_task.parameters)) {
					methodsWithParams.emplace_back(p_task, method);
				}
			}
			break;
		}
	}

	return methodsWithParams;
}

bool PlanningDomain::task_is_operator(const StringName &p_task_name) const {
	return (operator_table.find(p_task_name) != operator_table.end());
}

bool PlanningDomain::task_is_method(const StringName &p_task_name) const {
	return (method_table.find(p_task_name) != method_table.end());
}
