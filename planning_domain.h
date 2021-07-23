#pragma once

#include <any>
#include <functional>
#include <iostream>
#include <map>
#include <optional>
#include <string>
#include <vector>

#include "core/string/string_name.h"

using Parameters = std::vector<std::any>;

struct State {
	std::string domain_name;
	std::any data;
};

struct Task {
	std::string task_name;
	Parameters parameters;
};

typedef std::function<std::optional<State>(const State &, const Parameters &)> OperatorFunction;
typedef std::function<std::optional<std::vector<Task>>(const State &, const Parameters &)> MethodFunction;

struct OperatorWithParams {
	OperatorWithParams(const Task &p_task, const OperatorFunction &p_func) :
			task(p_task),
			func(p_func) {}

	Task task;
	OperatorFunction func;
};

struct MethodWithParams {
	MethodWithParams(const Task &p_task, const MethodFunction &p_func) :
			task(p_task),
			func(p_func) {}

	Task task;
	MethodFunction func;
};

using Operators = std::vector<OperatorFunction>;
using Methods = std::vector<MethodFunction>;
using OperatorsWithParams = std::vector<OperatorWithParams>;
using MethodsWithParams = std::vector<MethodWithParams>;

class PlanningDomain {
public:
	PlanningDomain(const std::string &p_domain_name);
	~PlanningDomain();

	void add_operator(const std::string &p_task_name, const OperatorFunction &p_operator_func);
	void add_method(const std::string &p_task_name, const MethodFunction &p_method_func);

	void set_name(const std::string &p_domain){
		domain_name = p_domain;
	}

	std::optional<OperatorsWithParams> get_applicable_operators(const State &p_current_state, const Task &p_task) const;
	std::optional<MethodsWithParams> get_relevant_methods(const State &p_current_state, const Task &p_task) const;

	bool task_is_operator(const std::string &p_task_name) const;
	bool task_is_method(const std::string &p_task_name) const;

private:
	std::string domain_name;
	std::map<std::string, Operators> operator_table;
	std::map<std::string, Methods> method_table;
};
