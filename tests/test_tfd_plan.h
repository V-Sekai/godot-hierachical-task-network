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

#ifndef TEST_TFD_H
#define TEST_TFD_H

#include "tests/test_macros.h"
#include <any>
#include <functional>
#include <optional>

#include <modules/tfd/planning_problem.h>
#include <modules/tfd/tfd.h>
#include <cassert>
#include <sstream>

namespace TestTFD {
PlanningDomain planningDomain = PlanningDomain("test_domain");
State initialState = { "test_domain", false };
Task topLevelTask = { "test_method", {} };
std::optional<State> problem_test_operator(const State &state, const Parameters &parameters) {
	State newState(state);
	bool status = !std::any_cast<bool>(state.data);
	newState.data = status;

	return newState;
}

std::optional<std::vector<Task>> problem_test_method(const State &state, const Parameters &parameters) {
	Task task;
	task.taskName = "test_operator";
	std::vector<Task> subtasks{ task };

	return subtasks;
}
TEST_CASE("[Modules][TotalOrderForwardDecomposition][PlanningTest][TryToPlanSucceed]") {
	planningDomain.AddOperator("test_operator", plan_test_operator);
	planningDomain.AddMethod("test_method", plan_test_method);

	PlanningProblem planningProblem(planningDomain, initialState, topLevelTask);
	TotalOrderForwardDecomposition tfd(planningProblem);

	std::vector<OperatorWithParams> solutionPlan = tfd.try_to_plan();

	// check operator
	REQUIRE_FALSE(solutionPlan.empty());
	REQUIRE(1 == solutionPlan.size());
	REQUIRE("test_operator" == solutionPlan[0].task.taskName);

	// check parameter
	REQUIRE(1 == solutionPlan[0].task.parameters.size());
	REQUIRE(std::any_cast<bool>(solutionPlan[0].task.parameters[0]));
	REQUIRE(true == std::any_cast<bool>(solutionPlan[0].task.parameters[0]));
}

TEST_CASE("[Modules][TotalOrderForwardDecomposition][PlanningTest][TryToPlanFail]") {
	planningDomain.AddOperator("test_operator", plan_test_operator);
	planningDomain.AddMethod("test_method", plan_test_method);

	Task task;
	task.taskName = "Random";

	PlanningProblem planningProblem(planningDomain, initialState, task);
	TotalOrderForwardDecomposition tfd(planningProblem);

	std::vector<OperatorWithParams> solutionPlan = tfd.try_to_plan();
	REQUIRE(solutionPlan.empty());
}
} // namespace TestTFD

#endif
