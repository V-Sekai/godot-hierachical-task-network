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

// Simple Travel Planning Domain
#pragma once

#include <functional>

#include <modules/tfd/planning_domain.h>

#define DOMAIN_NAME "simple_travel"
#define WALKING_DISTANCE 2

// Tasks
#define TRAVEL "Travel"
#define WALK "Walk"
#define CALL_TAXI "CallTaxi"
#define RIDE_TAXI "RideTaxi"
#define PAY_DRIVER "PayDriver"

class SimpleTravelState {
public:
	using Object = std::string;
	using Location = std::string;
	using Cash = std::size_t;
	using Distance = std::size_t;

	using PersonLocationTable = std::map<Object, Location>;
	using PersonCashTable = std::map<Object, Cash>;
	using PersonOweTable = std::map<Object, Cash>;
	using DistanceTable = std::map<Location, std::map<Location, Distance>>;

	SimpleTravelState(const PersonLocationTable &personLocationTable,
			const PersonCashTable &personCashTable,
			const PersonOweTable &personOweTable,
			const DistanceTable &distanceTable);
	virtual ~SimpleTravelState();

	std::optional<Location> LocationOf(const Object &person) const;
	std::optional<Cash> CashOwnedBy(const Object &person) const;
	std::optional<Cash> Owe(const Object &person) const;
	std::optional<Distance> DistanceBetween(const Location &location1, const Location &location2) const;

	void SetLocationOf(const Object &person, const Location &location);
	void SetCashOwnedBy(const Object &person, const Cash &cash);
	void SetOwe(const Object &person, const Cash &cash);

	Cash TaxiRate(const Distance &distance) const;

private:
	friend std::ostream &operator<<(std::ostream &os, const SimpleTravelState &state);

	PersonLocationTable m_personLocationTable;
	PersonCashTable m_personCashTable;
	PersonOweTable m_personOweTable;
	DistanceTable m_distanceTable;
};

// Operators
std::optional<State> Walk(const State &state, const Parameters &parameters);
std::optional<State> CallTaxi(const State &state, const Parameters &parameters);
std::optional<State> RideTaxi(const State &state, const Parameters &parameters);
std::optional<State> PayDriver(const State &state, const Parameters &parameters);

// Methods
std::optional<std::vector<Task>> TravelByFoot(const State &state, const Parameters &parameters);
std::optional<std::vector<Task>> TravelByTaxi(const State &state, const Parameters &parameters);

PlanningDomain CreatePlanningDomain();

std::ostream &operator<<(std::ostream &os, const SimpleTravelState &state);

// initial state
static const SimpleTravelState::PersonLocationTable s_initPersonLocationTable = { { "me", "home" }, { "taxi", "park" } };
static const SimpleTravelState::PersonCashTable s_initPersonCashTable = { { "me", 20 } };
static const SimpleTravelState::PersonOweTable s_initPersonOweTable = { { "me", 0 } };
static const SimpleTravelState::DistanceTable s_initDistanceTable = { { "home", { { "park", 8 } } }, { "park", { { "home", 8 } } } };

static const SimpleTravelState s_initialState(s_initPersonLocationTable,
		s_initPersonCashTable,
		s_initPersonOweTable,
		s_initDistanceTable);

PlanningProblem CreatePlanningProblem(const Task &topLevelTask) {
	auto planningDomain = CreatePlanningDomain();
	State state = { DOMAIN_NAME, s_initialState };

	return PlanningProblem(planningDomain, state, topLevelTask);
}
PlanningDomain CreatePlanningDomain() {
	PlanningDomain planningDomain(DOMAIN_NAME);

	planningDomain.AddOperator(WALK, Walk);
	planningDomain.AddOperator(CALL_TAXI, CallTaxi);
	planningDomain.AddOperator(RIDE_TAXI, RideTaxi);
	planningDomain.AddOperator(PAY_DRIVER, PayDriver);

	planningDomain.AddMethod(TRAVEL, TravelByFoot);
	planningDomain.AddMethod(TRAVEL, TravelByTaxi);

	return planningDomain;
}

SimpleTravelState::SimpleTravelState(const PersonLocationTable &personLocationTable,
		const PersonCashTable &personCashTable,
		const PersonOweTable &personOweTable,
		const DistanceTable &distanceTable) :
		m_personLocationTable(personLocationTable),
		m_personCashTable(personCashTable),
		m_personOweTable(personOweTable),
		m_distanceTable(distanceTable) {
}

SimpleTravelState::~SimpleTravelState() {
}

std::optional<SimpleTravelState::Location> SimpleTravelState::LocationOf(const Object &person) const {
	if (m_personLocationTable.find(person) == m_personLocationTable.end()) {
		return std::nullopt;
	} else {
		return m_personLocationTable.at(person);
	}
}

void SimpleTravelState::SetLocationOf(const Object &person, const Location &location) {
	if (m_personLocationTable.find(person) != m_personLocationTable.end()) {
		m_personLocationTable[person] = location;
	}
}

std::optional<SimpleTravelState::Cash> SimpleTravelState::CashOwnedBy(const Object &person) const {
	if (m_personCashTable.find(person) == m_personCashTable.end()) {
		return std::nullopt;
	} else {
		return m_personCashTable.at(person);
	}
}

void SimpleTravelState::SetCashOwnedBy(const Object &person, const Cash &cash) {
	if (m_personCashTable.find(person) != m_personCashTable.end()) {
		m_personCashTable[person] = cash;
	}
}

std::optional<SimpleTravelState::Cash> SimpleTravelState::Owe(const Object &person) const {
	if (m_personOweTable.find(person) == m_personOweTable.end()) {
		return std::nullopt;
	} else {
		return m_personOweTable.at(person);
	}
}

void SimpleTravelState::SetOwe(const Object &person, const Cash &cash) {
	if (m_personOweTable.find(person) != m_personOweTable.end()) {
		m_personOweTable[person] = cash;
	}
}

std::optional<SimpleTravelState::Distance> SimpleTravelState::DistanceBetween(const Location &location1, const Location &location2) const {
	if (m_distanceTable.find(location1) == m_distanceTable.end()) {
		return std::nullopt;
	} else {
		const auto &destination_table = m_distanceTable.at(location1);
		if (destination_table.find(location2) == destination_table.end()) {
			return std::nullopt;
		} else {
			return destination_table.at(location2);
		}
	}
}

SimpleTravelState::Cash SimpleTravelState::TaxiRate(const Distance &distance) const {
	return (1.5 + 0.5 * distance);
}

// Operators
std::optional<State> Walk(const State &state, const Parameters &parameters) {
	assert(parameters.size() == 3);
	assert(state.domainName == DOMAIN_NAME);

	try {
		const auto &person = std::any_cast<SimpleTravelState::Object>(parameters[0]);
		const auto &src = std::any_cast<SimpleTravelState::Location>(parameters[1]);
		const auto &dst = std::any_cast<SimpleTravelState::Location>(parameters[2]);
		auto simpleTravelState = std::any_cast<SimpleTravelState>(state.data);

		auto currentLocation = simpleTravelState.LocationOf(person);
		if (currentLocation && currentLocation.value() == src) {
			State newState(state);
			simpleTravelState.SetLocationOf(person, dst);
			newState.data = simpleTravelState;

			return newState;
		}
	} catch (const std::bad_any_cast &e) {
		std::cerr << e.what() << " in " << __PRETTY_FUNCTION__ << '\n';
	}

	return std::nullopt;
}

std::optional<State> CallTaxi(const State &state, const Parameters &parameters) {
	assert(parameters.size() == 2);
	assert(state.domainName == DOMAIN_NAME);

	try {
		State newState(state);
		auto simpleTravelState = std::any_cast<SimpleTravelState>(newState.data);
		const auto &person = std::any_cast<SimpleTravelState::Object>(parameters[0]);
		const auto &taxi = std::any_cast<SimpleTravelState::Object>(parameters[1]);

		auto personLocation = simpleTravelState.LocationOf(person);
		if (personLocation) {
			simpleTravelState.SetLocationOf(taxi, personLocation.value());
		}
		newState.data = simpleTravelState;

		return newState;
	} catch (const std::exception &e) {
		std::cerr << e.what() << " in " << __PRETTY_FUNCTION__ << '\n';
	}

	return std::nullopt;
}

std::optional<State> RideTaxi(const State &state, const Parameters &parameters) {
	assert(parameters.size() == 4);
	assert(state.domainName == DOMAIN_NAME);

	try {
		const auto &person = std::any_cast<SimpleTravelState::Object>(parameters[0]);
		const auto &taxi = std::any_cast<SimpleTravelState::Object>(parameters[1]);
		const auto &src = std::any_cast<SimpleTravelState::Location>(parameters[2]);
		const auto &dst = std::any_cast<SimpleTravelState::Location>(parameters[3]);
		auto simpleTravelState = std::any_cast<SimpleTravelState>(state.data);

		auto currentPersonLocation = simpleTravelState.LocationOf(person);
		auto currentTaxiLocation = simpleTravelState.LocationOf(taxi);

		if (currentPersonLocation and currentTaxiLocation) {
			if ((currentPersonLocation.value() == src) and (currentTaxiLocation.value() == src)) {
				State newState(state);
				auto distance = simpleTravelState.DistanceBetween(src, dst);

				if (distance) {
					simpleTravelState.SetLocationOf(person, dst);
					simpleTravelState.SetLocationOf(taxi, dst);
					simpleTravelState.SetOwe(person, simpleTravelState.TaxiRate(distance.value()));
					newState.data = simpleTravelState;
				}

				return newState;
			}
		}
	} catch (const std::exception &e) {
		std::cerr << e.what() << " in " << __PRETTY_FUNCTION__ << '\n';
	}

	return std::nullopt;
}

std::optional<State> PayDriver(const State &state, const Parameters &parameters) {
	assert(parameters.size() == 1);
	assert(state.domainName == DOMAIN_NAME);

	try {
		const auto &person = std::any_cast<SimpleTravelState::Object>(parameters[0]);
		auto simpleTravelState = std::any_cast<SimpleTravelState>(state.data);

		auto cashOwned = simpleTravelState.CashOwnedBy(person);
		auto owe = simpleTravelState.Owe(person);

		if (cashOwned and owe) {
			if (cashOwned.value() >= owe.value()) {
				State newState(state);

				simpleTravelState.SetCashOwnedBy(person, cashOwned.value() - owe.value());
				simpleTravelState.SetOwe(person, 0);
				newState.data = state;

				return newState;
			}
		}
	} catch (const std::exception &e) {
		std::cerr << e.what() << " in " << __PRETTY_FUNCTION__ << '\n';
	}

	return std::nullopt;
}

// Methods
std::optional<std::vector<Task>> TravelByFoot(const State &state, const Parameters &parameters) {
	assert(parameters.size() == 4);
	assert(state.domainName == DOMAIN_NAME);

	try {
		const auto &person = std::any_cast<SimpleTravelState::Object>(parameters[0]);
		const auto &taxi = std::any_cast<SimpleTravelState::Object>(parameters[1]);
		const auto &src = std::any_cast<SimpleTravelState::Location>(parameters[2]);
		const auto &dst = std::any_cast<SimpleTravelState::Location>(parameters[3]);
		auto simpleTravelState = std::any_cast<SimpleTravelState>(state.data);

		auto distance = simpleTravelState.DistanceBetween(src, dst);
		if (distance && distance.value() <= WALKING_DISTANCE) {
			Task task;
			task.taskName = WALK;
			task.parameters.push_back(person);
			task.parameters.push_back(src);
			task.parameters.push_back(dst);
			std::vector<Task> subtasks{ task };

			return subtasks;
		}
	} catch (const std::exception &e) {
		std::cerr << e.what() << " in " << __PRETTY_FUNCTION__ << '\n';
	}

	return std::nullopt;
}

std::optional<std::vector<Task>> TravelByTaxi(const State &state, const Parameters &parameters) {
	assert(parameters.size() == 4);
	assert(state.domainName == DOMAIN_NAME);

	try {
		const auto &person = std::any_cast<SimpleTravelState::Object>(parameters[0]);
		const auto &taxi = std::any_cast<SimpleTravelState::Object>(parameters[1]);
		const auto &src = std::any_cast<SimpleTravelState::Location>(parameters[2]);
		const auto &dst = std::any_cast<SimpleTravelState::Location>(parameters[3]);
		auto simpleTravelState = std::any_cast<SimpleTravelState>(state.data);

		auto cash = simpleTravelState.CashOwnedBy(person);
		auto distance = simpleTravelState.DistanceBetween(src, dst);
		auto currentPersonLocation = simpleTravelState.LocationOf(person);
		if (cash and distance and currentPersonLocation) {
			if (currentPersonLocation.value() == src) {
				if (cash.value() >= simpleTravelState.TaxiRate(distance.value())) {
					Task callTaxi;
					callTaxi.taskName = CALL_TAXI;
					callTaxi.parameters.push_back(person);
					callTaxi.parameters.push_back(taxi);

					Task rideTaxi;
					rideTaxi.taskName = RIDE_TAXI;
					rideTaxi.parameters.push_back(person);
					rideTaxi.parameters.push_back(taxi);
					rideTaxi.parameters.push_back(src);
					rideTaxi.parameters.push_back(dst);

					Task payDriver;
					payDriver.taskName = PAY_DRIVER;
					payDriver.parameters.push_back(person);

					std::vector<Task> subtasks{ payDriver, rideTaxi, callTaxi };

					return subtasks;
				}
			}
		}
	} catch (const std::exception &e) {
		std::cerr << e.what() << " in " << __PRETTY_FUNCTION__ << '\n';
	}

	return std::nullopt;
}

std::ostream &operator<<(std::ostream &os, const SimpleTravelState &state) {
	os << "SimpleTravelState";
	return os;
}

TEST_CASE("[Modules][TotalOrderForwardDecomposition][Simple Travel Problem]") {
	Task task;
	task.taskName = TRAVEL;
	task.parameters.push_back(SimpleTravelState::Object("me"));
	task.parameters.push_back(SimpleTravelState::Object("taxi"));
	task.parameters.push_back(SimpleTravelState::Location("home"));
	task.parameters.push_back(SimpleTravelState::Location("park"));
	TotalOrderForwardDecomposition tfd(CreatePlanningProblem(task));
	TotalOrderForwardDecomposition::Plan solutionPlan = tfd.TryToPlan();
	if (solutionPlan.empty()) {
		std::cout << "Failed to plan" << std::endl;
		return;
	}
	std::cout << "Found solution plan" << std::endl;
	for (const auto &_operator : solutionPlan) {
		std::cout << _operator.task.taskName << std::endl;
	}
}

TEST_CASE("[Modules][TotalOrderForwardDecomposition][EmptyOperatorTable]") {
	State state = { DOMAIN_NAME, s_initialState };
	Task task;
	task.taskName = "TestOperator";
	PlanningDomain planningDomain = CreatePlanningDomain();
	PlanningProblem problem = PlanningProblem(planningDomain, state, task);
	std::optional<std::vector<OperatorWithParams>> applicableOperators = planningDomain.GetApplicableOperators(state, task);
	REQUIRE(std::nullopt == applicableOperators);
}

} // namespace TestTFD

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
