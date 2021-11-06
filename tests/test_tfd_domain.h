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

#include "tfd_common.h"

#include "../planning.h"
#include "../planning_problem.h"
#include <cassert>
#include <sstream>

TEST_CASE("[Modules][TaskPlanner] Get domain empty operator table") {
	State state = initial_state;
	Task task;
	PlanningProblem problem;
	problem.set_planning_domain(planning_domain);
	problem.set_initial_state(state);
	problem.set_top_level_task(task);
	std::vector<OperatorWithParams> applicable_operators = planning_domain.get_applicable_operators(state, task);
	REQUIRE_FALSE(applicable_operators.empty());
}

TEST_CASE("[Modules][TaskPlanner] Get domain empty method table") {
	State state;
	Task task;
	task.task_name = "test_method";
	std::optional<std::vector<MethodWithParams>> relevantMethods = planning_domain.get_relevant_methods(state, task);

	REQUIRE_FALSE(relevantMethods.has_value());
}

TEST_CASE("[Modules][TaskPlanner] Get domain operator succeed") {
	State state;
	state.domain_name = "test_domain";
	state.data = false;

	Task task;
	task.task_name = "test_operator";
	planning_domain.add_operator("test_operator", test_operator);
	std::vector<OperatorWithParams> applicableOperators = planning_domain.get_applicable_operators(state, task);

	REQUIRE(applicableOperators.size());
	REQUIRE(1 == applicableOperators.size());
	REQUIRE(task.task_name == applicableOperators[0].task.task_name);
}

TEST_CASE("[Modules][TaskPlanner] Get domain operator wrong task name]") {
	State state;
	state.domain_name = "test_domain";
	state.data = false;

	Task task;
	task.task_name = "TestOperatorFail";

	planning_domain.add_operator("test_operator", test_operator);
	std::vector<OperatorWithParams> applicableOperators = planning_domain.get_applicable_operators(state, task);

	REQUIRE(applicableOperators.size());
	REQUIRE(true == applicableOperators.empty());
}

TEST_CASE("[Modules][TaskPlanner] Get domain methods succeed") {
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

TEST_CASE("[Modules][TaskPlanner] Get domain get method wrong task name") {
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

TEST_CASE("[Modules][TaskPlanner] Get domain method task is operator") {
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

TEST_CASE("[Modules][TaskPlanner] Get domain task is method") {
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
