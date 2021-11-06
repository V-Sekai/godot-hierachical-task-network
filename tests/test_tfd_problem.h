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

#ifndef TEST_TFD_PROBLEM_H
#define TEST_TFD_PROBLEM_H

#include "tests/test_macros.h"
#include <any>
#include <functional>
#include <optional>

#include "tfd_common.h"

#include "../planning.h"
#include "../planning_problem.h"
#include <cassert>
#include <sstream>

std::optional<State> problem_test_operator(const State &state, const Parameters &parameters) {
	State newState(state);
	bool status = !std::any_cast<bool>(state.data);
	newState.data = status;

	return newState;
}

std::optional<std::vector<Task>> problem_test_method(const State &state, const Parameters &parameters) {
	Task task;
	task.task_name = "test_operator";
	std::vector<Task> subtasks{ task };

	return subtasks;
}

TEST_CASE("[Modules][TaskPlanner] Planning problem is task method") {
	planning_domain.add_operator("test_operator", problem_test_operator);
	planning_domain.add_method("test_method", problem_test_method);

	PlanningProblem planningProblem;
	planningProblem.set_planning_domain(planning_domain);
	planningProblem.set_initial_state(initial_state);
	planningProblem.set_top_level_task(top_level_task);
	bool isMethod = planningProblem.task_is_method("test_method");
	REQUIRE(isMethod);
	isMethod = planningProblem.task_is_method("test_operator");
	REQUIRE_FALSE(isMethod);
}

#endif // TEST_TFD_PROBLEM_H
