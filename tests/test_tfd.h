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

static const SimpleTravelState INITIAL_STATE(s_initPersonLocationTable,
		s_initPersonCashTable,
		s_initPersonOweTable,
		s_initDistanceTable);

PlanningProblem CreatePlanningProblem(const Task &topLevelTask) {
	PlanningDomain planningDomain = CreatePlanningDomain();
	State state = { DOMAIN_NAME, INITIAL_STATE };

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
		const std::map<std::basic_string<char>, unsigned long long> &destination_table = m_distanceTable.at(location1);
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
		const std::basic_string<char> &person = std::any_cast<SimpleTravelState::Object>(parameters[0]);
		const std::basic_string<char> &src = std::any_cast<SimpleTravelState::Location>(parameters[1]);
		const std::basic_string<char> &dst = std::any_cast<SimpleTravelState::Location>(parameters[2]);
		SimpleTravelState simpleTravelState = std::any_cast<SimpleTravelState>(state.data);

		std::optional<std::basic_string<char>> currentLocation = simpleTravelState.LocationOf(person);
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
		SimpleTravelState simpleTravelState = std::any_cast<SimpleTravelState>(newState.data);
		const std::basic_string<char> &person = std::any_cast<SimpleTravelState::Object>(parameters[0]);
		const std::basic_string<char> &taxi = std::any_cast<SimpleTravelState::Object>(parameters[1]);

		std::optional<std::basic_string<char>> personLocation = simpleTravelState.LocationOf(person);
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
		const std::basic_string<char> &person = std::any_cast<SimpleTravelState::Object>(parameters[0]);
		const std::basic_string<char> &taxi = std::any_cast<SimpleTravelState::Object>(parameters[1]);
		const std::basic_string<char> &src = std::any_cast<SimpleTravelState::Location>(parameters[2]);
		const std::basic_string<char> &dst = std::any_cast<SimpleTravelState::Location>(parameters[3]);
		SimpleTravelState simpleTravelState = std::any_cast<SimpleTravelState>(state.data);

		std::optional<std::basic_string<char>> currentPersonLocation = simpleTravelState.LocationOf(person);
		std::optional<std::basic_string<char>> currentTaxiLocation = simpleTravelState.LocationOf(taxi);

		if (currentPersonLocation and currentTaxiLocation) {
			if ((currentPersonLocation.value() == src) and (currentTaxiLocation.value() == src)) {
				State newState(state);
				std::optional<unsigned long long> distance = simpleTravelState.DistanceBetween(src, dst);

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
		const std::basic_string<char> &person = std::any_cast<SimpleTravelState::Object>(parameters[0]);
		SimpleTravelState simpleTravelState = std::any_cast<SimpleTravelState>(state.data);

		std::optional<unsigned long long> cashOwned = simpleTravelState.CashOwnedBy(person);
		std::optional<unsigned long long> owe = simpleTravelState.Owe(person);

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
		const std::basic_string<char> &person = std::any_cast<SimpleTravelState::Object>(parameters[0]);
		const std::basic_string<char> &taxi = std::any_cast<SimpleTravelState::Object>(parameters[1]);
		const std::basic_string<char> &src = std::any_cast<SimpleTravelState::Location>(parameters[2]);
		const std::basic_string<char> &dst = std::any_cast<SimpleTravelState::Location>(parameters[3]);
		SimpleTravelState simpleTravelState = std::any_cast<SimpleTravelState>(state.data);

		std::optional<unsigned long long> distance = simpleTravelState.DistanceBetween(src, dst);
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
		const std::basic_string<char> &person = std::any_cast<SimpleTravelState::Object>(parameters[0]);
		const std::basic_string<char> &taxi = std::any_cast<SimpleTravelState::Object>(parameters[1]);
		const std::basic_string<char> &src = std::any_cast<SimpleTravelState::Location>(parameters[2]);
		const std::basic_string<char> &dst = std::any_cast<SimpleTravelState::Location>(parameters[3]);
		SimpleTravelState simpleTravelState = std::any_cast<SimpleTravelState>(state.data);

		std::optional<unsigned long long> cash = simpleTravelState.CashOwnedBy(person);
		std::optional<unsigned long long> distance = simpleTravelState.DistanceBetween(src, dst);
		std::optional<std::basic_string<char>> currentPersonLocation = simpleTravelState.LocationOf(person);
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
	TotalOrderForwardDecomposition::Plan solutionPlan = tfd.try_to_plan();
	if (solutionPlan.empty()) {
		std::cout << "Failed to plan" << std::endl;
		return;
	}
	std::cout << "Found solution plan" << std::endl;
	for (const OperatorWithParams &_operator : solutionPlan) {
		std::cout << _operator.task.taskName << std::endl;
	}
}

TEST_CASE("[Modules][TotalOrderForwardDecomposition][DomainTest][EmptyOperatorTable]") {
	State state = { DOMAIN_NAME, INITIAL_STATE };
	Task task;
	PlanningDomain planning_domain = PlanningDomain(DOMAIN_NAME);
	PlanningProblem problem = PlanningProblem(planning_domain, state, task);
	std::optional<std::vector<OperatorWithParams>> applicable_operators = planning_domain.GetApplicableOperators(state, task);
	REQUIRE_FALSE(applicable_operators.has_value());
}

TEST_CASE("[Modules][TotalOrderForwardDecomposition][DomainTest][EmptyMethodTable]") {
	State state;
	Task task;
	task.taskName = "test_method";
	PlanningDomain planning_domain = PlanningDomain(DOMAIN_NAME);
	std::optional<std::vector<MethodWithParams>> relevantMethods = planning_domain.GetRelevantMethods(state, task);

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
	task.taskName = "test_operator";
	std::vector<Task> subtasks{ task };

	return subtasks;
}

TEST_CASE("[Modules][TotalOrderForwardDecomposition][DomainTest][GetOperatorSucceed]") {
	State state;
	state.domainName = "test_domain";
	state.data = INITIAL_STATE;

	Task task;
	task.taskName = "test_operator";
	PlanningDomain planning_domain = PlanningDomain(DOMAIN_NAME);
	planning_domain.AddOperator("test_operator", test_operator);
	std::optional<std::vector<OperatorWithParams>> applicableOperators = planning_domain.GetApplicableOperators(state, task);

	REQUIRE(applicableOperators);
	REQUIRE(false == applicableOperators.value().empty());
	REQUIRE(1 == applicableOperators.value().size());
	REQUIRE(task.taskName == applicableOperators.value()[0].task.taskName);
}

TEST_CASE("[Modules][TotalOrderForwardDecomposition][DomainTest][GetOperatorWrongTaskName]") {
	State state;
	state.domainName = "test_domain";
	state.data = false;

	Task task;
	task.taskName = "TestOperatorFail";

	PlanningDomain planning_domain = PlanningDomain(DOMAIN_NAME);
	planning_domain.AddOperator("test_operator", test_operator);
	std::optional<std::vector<OperatorWithParams>> applicableOperators = planning_domain.GetApplicableOperators(state, task);

	REQUIRE(applicableOperators);
	REQUIRE(true == applicableOperators.value().empty());
}

TEST_CASE("[Modules][TotalOrderForwardDecomposition][DomainTest][GetMethodSucceed]") {
	State state;
	state.domainName = "test_domain";
	state.data = false;

	Task task;
	task.taskName = "test_method";

	PlanningDomain planning_domain = PlanningDomain(DOMAIN_NAME);
	planning_domain.AddMethod("test_method", test_method);
	std::optional<std::vector<MethodWithParams>> relevantMethods = planning_domain.GetRelevantMethods(state, task);

	REQUIRE(relevantMethods);
	REQUIRE(false == relevantMethods.value().empty());
	REQUIRE(1 == relevantMethods.value().size());
	REQUIRE(task.taskName == relevantMethods.value()[0].task.taskName);
}

TEST_CASE("[Modules][TotalOrderForwardDecomposition][DomainTest][GetMethodWrongTaskName]") {
	State state;
	state.domainName = "test_domain";
	state.data = false;

	Task task;
	task.taskName = "test_method_fail";

	PlanningDomain planning_domain = PlanningDomain(DOMAIN_NAME);
	planning_domain.AddMethod("test_method", test_method);
	std::optional<std::vector<MethodWithParams>> relevantMethods = planning_domain.GetRelevantMethods(state, task);

	REQUIRE(relevantMethods);
	REQUIRE(true == relevantMethods.value().empty());
}

TEST_CASE("[Modules][TotalOrderForwardDecomposition][DomainTest][TaskIsOperator]") {
	State state;
	state.domainName = "test_domain";
	state.data = false;

	Task task;
	task.taskName = "test_operator";

	PlanningDomain planning_domain = PlanningDomain(DOMAIN_NAME);
	planning_domain.AddOperator("test_operator", test_operator);
	planning_domain.AddMethod("test_method", test_method);
	bool isOperator = planning_domain.TaskIsOperator("test_operator");
	REQUIRE(isOperator);
	isOperator = planning_domain.TaskIsOperator("test_method");
	REQUIRE_FALSE(isOperator);
}

TEST_CASE("[Modules][TotalOrderForwardDecomposition][DomainTest][TaskIsMethod]") {
	State state;
	state.domainName = "test_domain";
	state.data = false;

	Task task;
	task.taskName = "test_operator";

	PlanningDomain planning_domain = PlanningDomain(DOMAIN_NAME);
	planning_domain.AddOperator("test_operator", test_operator);
	planning_domain.AddMethod("test_method", test_method);
	bool isMethod = planning_domain.TaskIsMethod("test_method");
	REQUIRE(isMethod);
	isMethod = planning_domain.TaskIsMethod("test_operator");
	REQUIRE_FALSE(isMethod);
}
std::optional<State> ProblemTestOperator(const State &state, const Parameters &parameters) {
	State newState(state);
	bool status = !std::any_cast<bool>(state.data);
	newState.data = status;

	return newState;
}

std::optional<std::vector<Task>> ProblemTestMethod(const State &state, const Parameters &parameters) {
	Task task;
	task.taskName = "test_operator";
	std::vector<Task> subtasks{ task };

	return subtasks;
}

PlanningDomain planningDomain = PlanningDomain("test_domain");
State initialState = { "test_domain", false };
Task topLevelTask = { "test_method", {} };

TEST_CASE("[Modules][TotalOrderForwardDecomposition][PlanningProblemTest][TaskIsMethod]") {
	planningDomain.AddOperator("test_operator", ProblemTestOperator);
	planningDomain.AddMethod("test_method", ProblemTestMethod);

	PlanningProblem planningProblem(planningDomain, initialState, topLevelTask);

	bool isMethod = planningProblem.TaskIsMethod("test_method");
	REQUIRE(isMethod);
	isMethod = planningProblem.TaskIsMethod("test_operator");
	REQUIRE_FALSE(isMethod);
}

TEST_CASE("[Modules][TotalOrderForwardDecomposition][PlanningProblemTest][TaskIsOperator]") {
	PlanningDomain planning_domain = PlanningDomain(DOMAIN_NAME);
	planning_domain.AddOperator("test_operator", ProblemTestOperator);
	planning_domain.AddMethod("test_method", ProblemTestMethod);

	PlanningProblem planningProblem(planning_domain, initialState, topLevelTask);

	bool isOperator = planningProblem.TaskIsOperator("test_operator");
	REQUIRE(isOperator);
	isOperator = planningProblem.TaskIsOperator("test_method");
	REQUIRE_FALSE(isOperator);
}

TEST_CASE("[Modules][TotalOrderForwardDecomposition][PlanningProblemTest][GetMethodsForTask]") {
	planningDomain.AddOperator("test_operator", ProblemTestOperator);
	planningDomain.AddMethod("test_method", ProblemTestMethod);

	PlanningProblem planningProblem(planningDomain, initialState, topLevelTask);

	std::vector<MethodWithParams> relevantMethods = planningProblem.GetMethodsForTask(topLevelTask, initialState);
	REQUIRE(false == relevantMethods.empty());
	REQUIRE(1 == relevantMethods.size());
	REQUIRE(topLevelTask.taskName == relevantMethods[0].task.taskName);

	Task task;
	task.taskName = "test_method_fail";

	relevantMethods = planningProblem.GetMethodsForTask(task, initialState);
	REQUIRE(true == relevantMethods.empty());
}

TEST_CASE("[Modules][TotalOrderForwardDecomposition][PlanningProblemTest][GetOperatorsForTask]") {
	planningDomain.AddOperator("test_operator", ProblemTestOperator);
	planningDomain.AddMethod("test_method", ProblemTestMethod);

	PlanningProblem planningProblem(planningDomain, initialState, topLevelTask);

	std::vector<OperatorWithParams> applicableOperators = planningProblem.GetOperatorsForTask(topLevelTask, initialState);
	REQUIRE(true == applicableOperators.empty());

	Task task;
	task.taskName = "test_operator";
	applicableOperators = planningProblem.GetOperatorsForTask(task, initialState);
	REQUIRE(false == applicableOperators.empty());
	REQUIRE(1 == applicableOperators.size());
	REQUIRE(task.taskName == applicableOperators[0].task.taskName);
}

TEST_CASE("[Modules][TotalOrderForwardDecomposition][PlanningProblemTest][GetInitialState]") {
	planningDomain.AddOperator("test_operator", ProblemTestOperator);
	planningDomain.AddMethod("test_method", ProblemTestMethod);

	PlanningProblem planningProblem(planningDomain, initialState, topLevelTask);
	REQUIRE(initialState.domainName == planningProblem.GetInitialState().domainName);
	REQUIRE(std::any_cast<bool>(initialState.data) == std::any_cast<bool>(planningProblem.GetInitialState().data));
}

TEST_CASE("[Modules][TotalOrderForwardDecomposition][PlanningProblemTest][GetTopLevelTask]") {
	planningDomain.AddOperator("test_operator", ProblemTestOperator);
	planningDomain.AddMethod("test_method", ProblemTestMethod);

	PlanningProblem planningProblem(planningDomain, initialState, topLevelTask);
	REQUIRE(topLevelTask.taskName == planningProblem.GetTopLevelTask().taskName);
	REQUIRE(topLevelTask.parameters.size() == planningProblem.GetTopLevelTask().parameters.size());
}

std::optional<State> PlanTestOperator(const State &state, const Parameters &parameters) {
	State newState(state);
	bool status = !std::any_cast<bool>(state.data);
	newState.data = status;

	return newState;
}

std::optional<std::vector<Task>> PlanTestMethod(const State &state, const Parameters &parameters) {
	Task task;
	task.taskName = "test_operator";
	task.parameters.push_back(true);
	std::vector<Task> subtasks{ task };

	return subtasks;
}

TEST_CASE("[Modules][TotalOrderForwardDecomposition][PlanningTest][TryToPlanSucceed]") {
	planningDomain.AddOperator("test_operator", PlanTestOperator);
	planningDomain.AddMethod("test_method", PlanTestMethod);

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
	planningDomain.AddOperator("test_operator", PlanTestOperator);
	planningDomain.AddMethod("test_method", PlanTestMethod);

	Task task;
	task.taskName = "Random";

	PlanningProblem planningProblem(planningDomain, initialState, task);
	TotalOrderForwardDecomposition tfd(planningProblem);

	std::vector<OperatorWithParams> solutionPlan = tfd.try_to_plan();
	REQUIRE(solutionPlan.empty());
}
} // namespace TestTFD

#endif
