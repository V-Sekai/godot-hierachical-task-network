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

#include <modules/tfd/tests/simple_travel_domain.h>
#include <modules/tfd/tests/simple_travel_problem.h>
#include <modules/tfd/tfd.h>


namespace TestTFD {
TEST_CASE("[Modules][TotalOrderForwardDecomposition] Simple Travel Problem") {
	Task task;
	task.taskName = TRAVEL;
	task.parameters.push_back(SimpleTravelState::Object("me"));
	task.parameters.push_back(SimpleTravelState::Object("taxi"));
	task.parameters.push_back(SimpleTravelState::Location("home"));
	task.parameters.push_back(SimpleTravelState::Location("park"));

	TotalOrderForwardDecomposition tfd(CreatePlanningProblem(task));
	TotalOrderForwardDecomposition::Plan solutionPlan = tfd.TryToPlan();

	if (!solutionPlan.empty()) {
		std::cout << "Found solution plan" << std::endl;
		for (const auto &_operator : solutionPlan) {
			std::cout << _operator.task.taskName << std::endl;
		}
	} else {
		std::cout << "Failed to plan" << std::endl;
	}
}
} // namespace TestTFD

// #include "planning_domain.h"
// #include <optional>
// #include <any>
// #include <functional>

// struct PlanningDomainTest : public ::testing::Test
// {
//     PlanningDomainTest() :
//         internalState(false),
//         planningDomain("TestDomain") {}

//     ~PlanningDomainTest() {}

//     bool internalState;
//     PlanningDomain planningDomain;

//     std::optional<State> Operator(const State& state, const Parameters& parameters)
//     {
//         State newState(state);
//         bool status = ! std::any_cast<bool>(state.data);
//         newState.data = status;

//         return newState;
//     }

//     std::optional<std::vector<Task>> Method(const State& state, const Parameters& parameters)
//     {
//         Task task;
//         task.taskName = "TestOperator";
//         std::vector<Task> subtasks{task};

//         return subtasks;
//     }
// };

// TEST_F(PlanningDomainTest, EmptyOperatorTable)
// {
//     State state;
//     Task task;
//     task.taskName = "TestOperator";
//     auto applicableOperators = planningDomain.GetApplicableOperators(state, task);

//     ASSERT_EQ(std::nullopt, applicableOperators);
// }

// TEST_F(PlanningDomainTest, EmptyMethodTable)
// {
//     State state;
//     Task task;
//     task.taskName = "TestMethod";
//     auto relevantMethods = planningDomain.GetRelevantMethods(state, task);

//     ASSERT_EQ(std::nullopt, relevantMethods);
// }

// TEST_F(PlanningDomainTest, GetOperatorSucceed)
// {
//     State state;
//     state.domainName = "TestDomain";
//     state.data = internalState;

//     Task task;
//     task.taskName = "TestOperator";

//     planningDomain.AddOperator("TestOperator", std::bind(&PlanningDomainTest::Operator, this, std::placeholders::_1, std::placeholders::_2));
//     auto applicableOperators = planningDomain.GetApplicableOperators(state, task);

//     ASSERT_TRUE(applicableOperators);
//     ASSERT_EQ(false, applicableOperators.value().empty());
//     ASSERT_EQ(1, applicableOperators.value().size());
//     ASSERT_EQ(task.taskName, applicableOperators.value()[0].task.taskName);
// }

// TEST_F(PlanningDomainTest, GetOperatorWrongTaskName)
// {
//     State state;
//     state.domainName = "TestDomain";
//     state.data = internalState;

//     Task task;
//     task.taskName = "TestOperatorFail";

//     planningDomain.AddOperator("TestOperator", std::bind(&PlanningDomainTest::Operator, this, std::placeholders::_1, std::placeholders::_2));
//     auto applicableOperators = planningDomain.GetApplicableOperators(state, task);

//     ASSERT_TRUE(applicableOperators);
//     ASSERT_EQ(true, applicableOperators.value().empty());
// }

// TEST_F(PlanningDomainTest, GetMethodSucceed)
// {
//     State state;
//     state.domainName = "TestDomain";
//     state.data = internalState;

//     Task task;
//     task.taskName = "TestMethod";

//     planningDomain.AddMethod("TestMethod", std::bind(&PlanningDomainTest::Method, this, std::placeholders::_1, std::placeholders::_2));
//     auto relevantMethods = planningDomain.GetRelevantMethods(state, task);

//     ASSERT_TRUE(relevantMethods);
//     ASSERT_EQ(false, relevantMethods.value().empty());
//     ASSERT_EQ(1, relevantMethods.value().size());
//     ASSERT_EQ(task.taskName, relevantMethods.value()[0].task.taskName);
// }

// TEST_F(PlanningDomainTest, GetMethodWrongTaskName)
// {
//     State state;
//     state.domainName = "TestDomain";
//     state.data = internalState;

//     Task task;
//     task.taskName = "TestMethodFail";

//     planningDomain.AddMethod("TestMethod", std::bind(&PlanningDomainTest::Method, this, std::placeholders::_1, std::placeholders::_2));
//     auto relevantMethods = planningDomain.GetRelevantMethods(state, task);

//     ASSERT_TRUE(relevantMethods);
//     ASSERT_EQ(true, relevantMethods.value().empty());
// }

// TEST_F(PlanningDomainTest, TaskIsOperator)
// {
//     State state;
//     state.domainName = "TestDomain";
//     state.data = internalState;

//     Task task;
//     task.taskName = "TestOperator";

//     planningDomain.AddOperator("TestOperator", std::bind(&PlanningDomainTest::Operator, this, std::placeholders::_1, std::placeholders::_2));
//     planningDomain.AddMethod("TestMethod", std::bind(&PlanningDomainTest::Method, this, std::placeholders::_1, std::placeholders::_2));
//     auto isOperator = planningDomain.TaskIsOperator("TestOperator");
//     ASSERT_TRUE(isOperator);
//     isOperator = planningDomain.TaskIsOperator("TestMethod");
//     ASSERT_FALSE(isOperator);
// }

// TEST_F(PlanningDomainTest, TaskIsMethod)
// {
//     State state;
//     state.domainName = "TestDomain";
//     state.data = internalState;

//     Task task;
//     task.taskName = "TestOperator";

//     planningDomain.AddOperator("TestOperator", std::bind(&PlanningDomainTest::Operator, this, std::placeholders::_1, std::placeholders::_2));
//     planningDomain.AddMethod("TestMethod", std::bind(&PlanningDomainTest::Method, this, std::placeholders::_1, std::placeholders::_2));
//     auto isMethod = planningDomain.TaskIsMethod("TestMethod");
//     ASSERT_TRUE(isMethod);
//     isMethod = planningDomain.TaskIsMethod("TestOperator");
//     ASSERT_FALSE(isMethod);
// }

// #include "planning_problem.h"
// #include "gtest/gtest.h"
// #include <optional>
// #include <any>
// #include <functional>

// namespace {
//     std::optional<State> Operator(const State& state, const Parameters& parameters)
//     {
//         State newState(state);
//         bool status = ! std::any_cast<bool>(state.data);
//         newState.data = status;

//         return newState;
//     }

//     std::optional<std::vector<Task>> Method(const State& state, const Parameters& parameters)
//     {
//         Task task;
//         task.taskName = "TestOperator";
//         std::vector<Task> subtasks{task};

//         return subtasks;
//     }
// }

// struct PlanningProblemTest : public ::testing::Test
// {
//     PlanningProblemTest() :
//         planningDomain("TestDomain"),
//         initialState{"TestDomain", false},
//         topLevelTask{"TestMethod", {}}{}

//     ~PlanningProblemTest() {}

//     PlanningDomain planningDomain;
//     State initialState;
//     Task topLevelTask;
// };

// TEST_F(PlanningProblemTest, TaskIsMethod)
// {
//     planningDomain.AddOperator("TestOperator", Operator);
//     planningDomain.AddMethod("TestMethod", Method);

//     PlanningProblem planningProblem(planningDomain, initialState, topLevelTask);

//     auto isMethod = planningProblem.TaskIsMethod("TestMethod");
//     ASSERT_TRUE(isMethod);
//     isMethod = planningProblem.TaskIsMethod("TestOperator");
//     ASSERT_FALSE(isMethod);
// }

// TEST_F(PlanningProblemTest, TaskIsOperator)
// {
//     planningDomain.AddOperator("TestOperator", Operator);
//     planningDomain.AddMethod("TestMethod", Method);

//     PlanningProblem planningProblem(planningDomain, initialState, topLevelTask);

//     auto isOperator = planningProblem.TaskIsOperator("TestOperator");
//     ASSERT_TRUE(isOperator);
//     isOperator = planningProblem.TaskIsOperator("TestMethod");
//     ASSERT_FALSE(isOperator);
// }

// TEST_F(PlanningProblemTest, GetMethodsForTask)
// {
//     planningDomain.AddOperator("TestOperator", Operator);
//     planningDomain.AddMethod("TestMethod", Method);

//     PlanningProblem planningProblem(planningDomain, initialState, topLevelTask);

//     auto relevantMethods = planningProblem.GetMethodsForTask(topLevelTask, initialState);
//     ASSERT_EQ(false, relevantMethods.empty());
//     ASSERT_EQ(1, relevantMethods.size());
//     ASSERT_EQ(topLevelTask.taskName, relevantMethods[0].task.taskName);

//     Task task;
//     task.taskName = "TestMethodFail";

//     relevantMethods = planningProblem.GetMethodsForTask(task, initialState);
//     ASSERT_EQ(true, relevantMethods.empty());
// }

// TEST_F(PlanningProblemTest, GetOperatorsForTask)
// {
//     planningDomain.AddOperator("TestOperator", Operator);
//     planningDomain.AddMethod("TestMethod", Method);

//     PlanningProblem planningProblem(planningDomain, initialState, topLevelTask);

//     auto applicableOperators = planningProblem.GetOperatorsForTask(topLevelTask, initialState);
//     ASSERT_EQ(true, applicableOperators.empty());

//     Task task;
//     task.taskName = "TestOperator";
//     applicableOperators = planningProblem.GetOperatorsForTask(task, initialState);
//     ASSERT_EQ(false, applicableOperators.empty());
//     ASSERT_EQ(1, applicableOperators.size());
//     ASSERT_EQ(task.taskName, applicableOperators[0].task.taskName);
// }

// TEST_F(PlanningProblemTest, GetInitialState)
// {
//     planningDomain.AddOperator("TestOperator", Operator);
//     planningDomain.AddMethod("TestMethod", Method);

//     PlanningProblem planningProblem(planningDomain, initialState, topLevelTask);
//     ASSERT_EQ(initialState.domainName, planningProblem.GetInitialState().domainName);
//     ASSERT_EQ(std::any_cast<bool>(initialState.data), std::any_cast<bool>(planningProblem.GetInitialState().data));
// }

// TEST_F(PlanningProblemTest, GetTopLevelTask)
// {
//     planningDomain.AddOperator("TestOperator", Operator);
//     planningDomain.AddMethod("TestMethod", Method);

//     PlanningProblem planningProblem(planningDomain, initialState, topLevelTask);
//     ASSERT_EQ(topLevelTask.taskName, planningProblem.GetTopLevelTask().taskName);
//     ASSERT_EQ(topLevelTask.parameters.size(), planningProblem.GetTopLevelTask().parameters.size());
// }

// namespace {
//     std::optional<State> Operator(const State& state, const Parameters& parameters)
//     {
//         State newState(state);
//         bool status = ! std::any_cast<bool>(state.data);
//         newState.data = status;

//         return newState;
//     }

//     std::optional<std::vector<Task>> Method(const State& state, const Parameters& parameters)
//     {
//         Task task;
//         task.taskName = "TestOperator";
//         task.parameters.push_back(true);
//         std::vector<Task> subtasks{task};

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

//     PlanningDomain planningDomain;
//     State initialState;
//     Task topLevelTask;
// };

// TEST_F(TFDTest, TryToPlanSucceed)
// {
//     planningDomain.AddOperator("TestOperator", Operator);
//     planningDomain.AddMethod("TestMethod", Method);

//     PlanningProblem planningProblem(planningDomain, initialState, topLevelTask);
//     TotalOrderForwardDecomposition tfd(planningProblem);

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

//     Task task;
//     task.taskName = "Random";

//     PlanningProblem planningProblem(planningDomain, initialState, task);
//     TotalOrderForwardDecomposition tfd(planningProblem);

//     auto solutionPlan = tfd.TryToPlan();
//     ASSERT_TRUE(solutionPlan.empty());
// }

#endif
