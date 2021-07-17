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

#include "tfd.h"
#include <optional>
#include <any>
#include <functional>
#include "tests/test_macros.h"

namespace TestTFD {
TEST_CASE("[Modules][TFD] tfd") {
}

// namespace {
//     std::optional<tfd_cpp::State> Operator(const tfd_cpp::State& state, const tfd_cpp::Parameters& parameters)
//     {
//         tfd_cpp::State newState(state);
//         bool status = ! std::any_cast<bool>(state.data);
//         newState.data = status;
        
//         return newState;
//     }

//     std::optional<std::vector<tfd_cpp::Task>> Method(const tfd_cpp::State& state, const tfd_cpp::Parameters& parameters)
//     {
//         tfd_cpp::Task task;
//         task.taskName = "TestOperator";
//         task.parameters.push_back(true);
//         std::vector<tfd_cpp::Task> subtasks{task};

//         return subtasks;
//     }
// }

// struct TFDTest : public ::testing::Test
// {
//     TFDTest() : 
//         planningDomain("TestDomain"),
//         initialState{"TestDomain", false},
//         topLevelTask{"TestMethod", {}}
//     {
//     }

//     ~TFDTest() {}

//     tfd_cpp::PlanningDomain planningDomain;
//     tfd_cpp::State initialState; 
//     tfd_cpp::Task topLevelTask;
// };

// TEST_F(TFDTest, TryToPlanSucceed)
// {
//     planningDomain.AddOperator("TestOperator", Operator);
//     planningDomain.AddMethod("TestMethod", Method);

//     tfd_cpp::PlanningProblem planningProblem(planningDomain, initialState, topLevelTask);
//     tfd_cpp::TFD tfd(planningProblem);

//     auto solutionPlan = tfd.TryToPlan();

//     // check operator
//     ASSERT_FALSE(solutionPlan.empty());
//     ASSERT_EQ(1, solutionPlan.size());
//     ASSERT_EQ("TestOperator", solutionPlan[0].task.taskName);

//     // check parameter
//     ASSERT_EQ(1, solutionPlan[0].task.parameters.size());
//     ASSERT_NO_THROW(std::any_cast<bool>(solutionPlan[0].task.parameters[0]));
//     ASSERT_EQ(true, std::any_cast<bool>(solutionPlan[0].task.parameters[0]));
// }

// TEST_F(TFDTest, TryToPlanFail)
// {
//     planningDomain.AddOperator("TestOperator", Operator);
//     planningDomain.AddMethod("TestMethod", Method);

//     tfd_cpp::Task task;
//     task.taskName = "Random";

//     tfd_cpp::PlanningProblem planningProblem(planningDomain, initialState, task);
//     tfd_cpp::TFD tfd(planningProblem);

//     auto solutionPlan = tfd.TryToPlan();
//     ASSERT_TRUE(solutionPlan.empty());
// }


} // namespace TEST_TFD_H

#endif
