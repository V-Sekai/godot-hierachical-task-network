#pragma once

#include <any>
#include <functional>
#include <map>
#include <optional>
#include <vector>

#include "core/string/string_name.h"

struct State {
	StringName domain_name;
	std::any data;
};

struct Task {
	StringName task_name;
	std::vector<std::any> parameters;
};

typedef std::function<std::optional<State>(const State &, const std::vector<std::any> &)> OperatorFunction;
typedef std::function<std::optional<std::vector<Task>>(const State &, const std::vector<std::any> &)> MethodFunction;

struct OperatorWithParams {
	OperatorWithParams(const Task &p_task, const OperatorFunction &p_func) :
			task(p_task),
			func(p_func) {}
	OperatorWithParams() {}

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

class PlanningDomain {
public:
	void set_domain_name(const StringName &p_domain_name) {
		domain_name = p_domain_name;
	}
	StringName get_domain_name() const {
		return domain_name;
	}

	void add_operator(const StringName &p_task_name, const OperatorFunction &p_operator_func);
	void add_method(const StringName &p_task_name, const MethodFunction &p_method_func);

	void set_name(const StringName &p_domain) {
		domain_name = p_domain;
	}

	std::vector<OperatorWithParams> get_applicable_operators(const State &p_current_state, const Task &p_task) const;
	std::optional<std::vector<MethodWithParams>> get_relevant_methods(const State &p_current_state, const Task &p_task) const;

	bool task_is_operator(const StringName &p_task_name) const;
	bool task_is_method(const StringName &p_task_name) const;

private:
	StringName domain_name;
	std::map<StringName, std::vector<OperatorFunction>> operator_table;
	std::map<StringName, std::vector<MethodFunction>> method_table;
};
