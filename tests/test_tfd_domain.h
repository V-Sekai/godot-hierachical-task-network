/*************************************************************************/
/*  test_tfd.h                                                           */
/*************************************************************************/
/*                       This file is part of:                           */
/*                           GODOT ENGINE                                */
/*                      https://godotengine.org                          */
/*************************************************************************/
/* Copyright (c) 2007-2020 Juan Linietsky, Ariel Manzur.                 */
/* Copyright (c) 2014-2020 Godot Engine contributors (cf. AUTHORS.md).   */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/

#ifndef TEST_TFD_DOMAIN_H
#define TEST_TFD_DOMAIN_H

#include "tests/test_macros.h"
#include <any>
#include <functional>
#include <optional>

#include <modules/tfd/planning_problem.h>
#include <modules/tfd/tfd.h>
#include <cassert>
#include <sstream>

PlanningDomain planning_domain = PlanningDomain("test_domain");
State initial_state = { "test_domain", false };
Task topLevelTask = { "test_method", {} };
TEST_CASE("[Modules][TotalOrderForwardDecomposition][DomainTest][EmptyOperatorTable]") {
	State state = initial_state;
	Task task;
	PlanningProblem problem = PlanningProblem(planning_domain, state, task);
	std::optional<std::vector<OperatorWithParams>> applicable_operators = planning_domain.get_applicable_operators(state, task);
	REQUIRE_FALSE(applicable_operators.has_value());
}

TEST_CASE("[Modules][TotalOrderForwardDecomposition][DomainTest][EmptyMethodTable]") {
	State state;
	Task task;
	task.task_name = "test_method";
	std::optional<std::vector<MethodWithParams>> relevantMethods = planning_domain.get_relevant_methods(state, task);

	REQUIRE_FALSE(relevantMethods.has_value());
}

std::optional<State> test_operator(const State &state, const Parameters &parameters) {
	State newState(state);
	bool status = !std::any_cast<bool>(state.data);
	newState.data = status;

	return newState;
}

std::optional<std::vector<Task>> test_method(const State &state, const Parameters &parameters) {
	Task task;
	task.task_name = "test_operator";
	std::vector<Task> subtasks{ task };

	return subtasks;
}

TEST_CASE("[Modules][TotalOrderForwardDecomposition][DomainTest][GetOperatorSucceed]") {
	State state;
	state.domain_name = "test_domain";
	state.data = false;

	Task task;
	task.task_name = "test_operator";
	planning_domain.add_operator("test_operator", test_operator);
	std::optional<std::vector<OperatorWithParams>> applicableOperators = planning_domain.get_applicable_operators(state, task);

	REQUIRE(applicableOperators);
	REQUIRE(false == applicableOperators.value().empty());
	REQUIRE(1 == applicableOperators.value().size());
	REQUIRE(task.task_name == applicableOperators.value()[0].task.task_name);
}

TEST_CASE("[Modules][TotalOrderForwardDecomposition][DomainTest][GetOperatorWrongp_task_name]") {
	State state;
	state.domain_name = "test_domain";
	state.data = false;

	Task task;
	task.task_name = "TestOperatorFail";

	planning_domain.add_operator("test_operator", test_operator);
	std::optional<std::vector<OperatorWithParams>> applicableOperators = planning_domain.get_applicable_operators(state, task);

	REQUIRE(applicableOperators);
	REQUIRE(true == applicableOperators.value().empty());
}

TEST_CASE("[Modules][TotalOrderForwardDecomposition][DomainTest][GetMethodSucceed]") {
	State state;
	state.domain_name = "test_domain";
	state.data = false;

	Task task;
	task.task_name = "test_method";
	planning_domain.add_method("test_method", test_method);
	std::optional<std::vector<MethodWithParams>> relevantMethods = planning_domain.get_relevant_methods(state, task);

	REQUIRE(relevantMethods);
	REQUIRE(false == relevantMethods.value().empty());
	REQUIRE(1 == relevantMethods.value().size());
	REQUIRE(task.task_name == relevantMethods.value()[0].task.task_name);
}

TEST_CASE("[Modules][TotalOrderForwardDecomposition][DomainTest][GetMethodWrongp_task_name]") {
	State state;
	state.domain_name = "test_domain";
	state.data = false;

	Task task;
	task.task_name = "test_method_fail";
	planning_domain.add_method("test_method", test_method);
	std::optional<std::vector<MethodWithParams>> relevantMethods = planning_domain.get_relevant_methods(state, task);

	REQUIRE(relevantMethods);
	REQUIRE(true == relevantMethods.value().empty());
}

TEST_CASE("[Modules][TotalOrderForwardDecomposition][DomainTest][TaskIsOperator]") {
	State state;
	state.domain_name = "test_domain";
	state.data = false;

	Task task;
	task.task_name = "test_operator";
	planning_domain.add_operator("test_operator", test_operator);
	planning_domain.add_method("test_method", test_method);
	bool isOperator = planning_domain.task_is_operator("test_operator");
	REQUIRE(isOperator);
	isOperator = planning_domain.task_is_operator("test_method");
	REQUIRE_FALSE(isOperator);
}

TEST_CASE("[Modules][TotalOrderForwardDecomposition][DomainTest][TaskIsMethod]") {
	State state;
	state.domain_name = "test_domain";
	state.data = false;

	Task task;
	task.task_name = "test_operator";
	planning_domain.add_operator("test_operator", test_operator);
	planning_domain.add_method("test_method", test_method);
	bool isMethod = planning_domain.task_is_method("test_method");
	REQUIRE(isMethod);
	isMethod = planning_domain.task_is_method("test_operator");
	REQUIRE_FALSE(isMethod);
}
#endif
