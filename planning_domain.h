#pragma once

#include "core/string/string_name.h"
#include "core/variant/variant.h"

#include <functional>
#include <map>
#include <optional>
#include <vector>

struct State {
	String domain_name;
	Variant data;
};

struct Task {
	String task_name;
	std::vector<Variant> parameters;
};

typedef std::function<std::optional<State>(const State &, const std::vector<Variant> &)> OperatorFunction;
typedef std::function<std::optional<std::vector<Task>>(const State &, const std::vector<Variant> &)> MethodFunction;

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
	void set_domain_name(const String &p_domain_name) {
		domain_name = p_domain_name;
	}
	String get_domain_name() const {
		return domain_name;
	}

	void add_operator(const String &p_task_name, const OperatorFunction &p_operator_func);
	void add_method(const String &p_task_name, const MethodFunction &p_method_func);

	void set_name(const String &p_domain) {
		domain_name = p_domain;
	}

	std::vector<OperatorWithParams> get_applicable_operators(const State &p_current_state, const Task &p_task) const;
	std::optional<std::vector<MethodWithParams>> get_relevant_methods(const State &p_current_state, const Task &p_task) const;

	bool task_is_operator(const String &p_task_name) const;
	bool task_is_method(const String &p_task_name) const;

private:
	String domain_name;
	std::map<String, std::vector<OperatorFunction>> operator_table;
	std::map<String, std::vector<MethodFunction>> method_table;
};
