#pragma once

#include "core/string/string_name.h"
#include <any>
#include <functional>
#include <iostream>
#include <map>
#include <optional>
#include <string>
#include <vector>

using Parameters = std::vector<std::any>;

struct State {
	StringName domain_name;
	std::any data;
};

struct Task {
	StringName task_name;
	Parameters parameters;
};

typedef std::function<std::optional<State>(const State &, const Parameters &)> p_operator_function;
typedef std::function<std::optional<std::vector<Task>>(const State &, const Parameters &)> p_method_function;

struct OperatorWithParams {
	OperatorWithParams(const Task &p_task, const p_operator_function &p_func) :
			task(p_task),
			func(p_func) {}

	Task task;
	p_operator_function func;
};

struct MethodWithParams {
	MethodWithParams(const Task &p_task, const p_method_function &p_func) :
			task(p_task),
			func(p_func) {}

	Task task;
	p_method_function func;
};

using Operators = std::vector<p_operator_function>;
using Methods = std::vector<p_method_function>;
using OperatorsWithParams = std::vector<OperatorWithParams>;
using MethodsWithParams = std::vector<MethodWithParams>;

class PlanningDomain {
public:
	PlanningDomain(const StringName &p_domain_name);
	~PlanningDomain();

	void add_operator(const StringName &p_task_name, const p_operator_function &p_operator_func);
	void add_method(const StringName &p_task_name, const p_method_function &p_method_func);

	std::optional<OperatorsWithParams> get_applicable_operators(const State &p_current_state, const Task &p_task) const;
	std::optional<MethodsWithParams> get_relevant_methods(const State &p_current_state, const Task &p_task) const;

	bool task_is_operator(const StringName &p_task_name) const;
	bool task_is_method(const StringName &p_task_name) const;

private:
	StringName DOMAIN_NAME;
	std::map<StringName, Operators> OPERATOR_TABLE;
	std::map<StringName, Methods> METHOD_TABLE;
};

std::ostream &operator<<(std::ostream &os, const State &p_state);
std::ostream &operator<<(std::ostream &os, const Task &p_task);
