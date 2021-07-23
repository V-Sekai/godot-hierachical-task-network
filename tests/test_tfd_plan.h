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

#ifndef TEST_TFD_PLAN_H
#define TEST_TFD_PLAN_H

#include "tests/test_macros.h"
#include <any>
#include <functional>
#include <optional>

#include "tfd_common.h"

#include "../planning_problem.h"
#include "../htn.h"
#include <cassert>
#include <sstream>

std::optional<State> plan_operator(const State &state, const Parameters &parameters) {
	State newState(state);
	bool status = !std::any_cast<bool>(state.data);
	newState.data = status;

	return newState;
}

std::optional<std::vector<Task>> plan_method(const State &state, const Parameters &parameters) {
	Task task;
	task.task_name = "test_operator";
	task.parameters.push_back(true);
	std::vector<Task> subtasks{ task };

	return subtasks;
}

TEST_CASE("[Modules][TotalOrderForwardDecomposition] Try to plan succeed") {
	PlanningDomain planning_domain_plan = PlanningDomain("test_domain");
	State initial_state_plan = { "test_domain", false };
	Task top_level_task_plan = { "test_method", {} };

	planning_domain_plan.add_operator("test_operator", plan_operator);
	planning_domain_plan.add_method("test_method", plan_method);

	PlanningProblem planning_problem(planning_domain_plan, initial_state_plan, top_level_task_plan);
	TotalOrderForwardDecomposition tfd(planning_problem);

	std::vector<OperatorWithParams> solution_plan = tfd.try_to_plan();

	// check operator
	REQUIRE_FALSE(solution_plan.empty());
	REQUIRE(1 == solution_plan.size());
	REQUIRE("test_operator" == solution_plan[0].task.task_name);

	// check parameter
	REQUIRE(1 == solution_plan[0].task.parameters.size());
	REQUIRE(std::any_cast<bool>(solution_plan[0].task.parameters[0]));
	REQUIRE(true == std::any_cast<bool>(solution_plan[0].task.parameters[0]));
}

TEST_CASE("[Modules][TotalOrderForwardDecomposition] Try to plan fail") {
	planning_domain.add_operator("test_operator", plan_operator);
	planning_domain.add_method("test_method", plan_method);

	Task task;
	task.task_name = "Random";

	PlanningProblem planning_problem(planning_domain, initial_state, task);
	TotalOrderForwardDecomposition tfd(planning_problem);

	std::vector<OperatorWithParams> solutionPlan = tfd.try_to_plan();
	REQUIRE(solutionPlan.empty());
}
#endif
