#include "planning_domain.h"

PlanningDomain::PlanningDomain(const std::string &p_domain_name) :
		DOMAIN_NAME(p_domain_name) {}

PlanningDomain::~PlanningDomain() {}

void PlanningDomain::add_operator(const std::string &p_task_name, const p_operator_function &p_operator_func) {
	auto operators = OPERATOR_TABLE.find(p_task_name);

	if (operators == OPERATOR_TABLE.end()) {
		OPERATOR_TABLE[p_task_name] = Operators{ p_operator_func };
	} else {
		operators->second.push_back(p_operator_func);
	}
}

void PlanningDomain::add_method(const std::string &p_task_name, const p_method_function &p_method_func) {
	auto methods = METHOD_TABLE.find(p_task_name);

	if (methods == METHOD_TABLE.end()) {
		METHOD_TABLE[p_task_name] = Methods{ p_method_func };
	} else {
		methods->second.push_back(p_method_func);
	}
}

std::optional<OperatorsWithParams> PlanningDomain::get_applicable_operators(const State &p_current_state, const Task &p_task) const {
	OperatorsWithParams operatorsWithParams;

	if (OPERATOR_TABLE.empty()) {
		return std::nullopt;
	}

	for (const auto &element : OPERATOR_TABLE) {
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

	if (METHOD_TABLE.empty()) {
		return std::nullopt;
	}

	for (const auto &element : METHOD_TABLE) {
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

bool PlanningDomain::task_is_operator(const std::string &p_task_name) const {
	return (OPERATOR_TABLE.find(p_task_name) != OPERATOR_TABLE.end());
}

bool PlanningDomain::task_is_method(const std::string &p_task_name) const {
	return (METHOD_TABLE.find(p_task_name) != METHOD_TABLE.end());
}

std::ostream &operator<<(std::ostream &r_os, const Task &p_task) {
	r_os << p_task.task_name << " with " << p_task.parameters.size() << " parameters.";
	return r_os;
}

std::ostream &operator<<(std::ostream &r_os, const State &p_state) {
	r_os << "State variable for: " << p_state.domain_name << "\n";
	return r_os;
}
