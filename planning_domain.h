#pragma once

#include <any>
#include <functional>
#include <iostream>
#include <map>
#include <optional>
#include <string>
#include <vector>

using Parameters = std::vector<std::any>;

struct State {
	std::string domain_name;
	std::any data;
};

struct Task {
	std::string task_name;
	Parameters parameters;
};

typedef std::function<std::optional<State>(const State &, const Parameters &)> p_operator_function;
typedef std::function<std::optional<std::vector<Task>>(const State &, const Parameters &)> p_method_function;

struct OperatorWithParams {
	OperatorWithParams(const Task &p_task, const p_operator_function &func) :
			task(p_task),
			func(func) {}

	Task task;
	p_operator_function func;
};

struct MethodWithParams {
	MethodWithParams(const Task &p_task, const p_method_function &func) :
			task(p_task),
			func(func) {}

	Task task;
	p_method_function func;
};

using Operators = std::vector<p_operator_function>;
using Methods = std::vector<p_method_function>;
using OperatorsWithParams = std::vector<OperatorWithParams>;
using MethodsWithParams = std::vector<MethodWithParams>;

class PlanningDomain {
public:
	PlanningDomain(const std::string &domain_name);
	~PlanningDomain();

	void add_operator(const std::string &p_task_name, const p_operator_function &p_operator_func);
	void add_method(const std::string &p_task_name, const p_method_function &p_method_func);

	std::optional<OperatorsWithParams> get_applicable_operators(const State &p_current_state, const Task &p_task) const;
	std::optional<MethodsWithParams> get_relevant_methods(const State &p_current_state, const Task &p_task) const;

	bool task_is_operator(const std::string &p_task_name) const;
	bool task_is_method(const std::string &p_task_name) const;

private:
	std::string DOMAIN_NAME;
	std::map<std::string, Operators> OPERATOR_TABLE;
	std::map<std::string, Methods> METHOD_TABLE;
};

std::ostream &operator<<(std::ostream &os, const State &p_state);
std::ostream &operator<<(std::ostream &os, const Task &p_task);
