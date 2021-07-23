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
/* a copy of this software && associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, && to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice && this permission notice shall be        */
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

#ifndef TEST_TFD_SIMPLE_H
#define TEST_TFD_SIMPLE_H

#include "tests/test_macros.h"
#include <any>
#include <functional>
#include <optional>

#include <modules/tfd/planning_problem.h>
#include <modules/tfd/tfd.h>
#include <cassert>
#include <sstream>

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

	planningDomain.add_operator(WALK, Walk);
	planningDomain.add_operator(CALL_TAXI, CallTaxi);
	planningDomain.add_operator(RIDE_TAXI, RideTaxi);
	planningDomain.add_operator(PAY_DRIVER, PayDriver);

	planningDomain.add_method(TRAVEL, TravelByFoot);
	planningDomain.add_method(TRAVEL, TravelByTaxi);

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

std::optional<State> Walk(const State &state, const Parameters &parameters) {
	assert(parameters.size() == 3);
	assert(state.domain_name == DOMAIN_NAME);

	const auto &person = std::any_cast<SimpleTravelState::Object>(parameters[0]);
	const auto &src = std::any_cast<SimpleTravelState::Location>(parameters[1]);
	const auto &dst = std::any_cast<SimpleTravelState::Location>(parameters[2]);
	SimpleTravelState simpleTravelState = std::any_cast<SimpleTravelState>(state.data);

	auto currentLocation = simpleTravelState.LocationOf(person);
	if (currentLocation && currentLocation.value() == src) {
		State newState(state);
		simpleTravelState.SetLocationOf(person, dst);
		newState.data = simpleTravelState;

		return newState;
	}
	return std::nullopt;
}

std::optional<State> CallTaxi(const State &state, const Parameters &parameters) {
	assert(parameters.size() == 2);
	assert(state.domain_name == DOMAIN_NAME);

	State newState(state);
	SimpleTravelState simpleTravelState = std::any_cast<SimpleTravelState>(newState.data);
	const auto &person = std::any_cast<SimpleTravelState::Object>(parameters[0]);
	const auto &taxi = std::any_cast<SimpleTravelState::Object>(parameters[1]);

	auto personLocation = simpleTravelState.LocationOf(person);
	if (personLocation) {
		simpleTravelState.SetLocationOf(taxi, personLocation.value());
	}
	newState.data = simpleTravelState;

	return newState;

	return std::nullopt;
}

std::optional<State> RideTaxi(const State &state, const Parameters &parameters) {
	assert(parameters.size() == 4);
	assert(state.domain_name == DOMAIN_NAME);

	const auto &person = std::any_cast<SimpleTravelState::Object>(parameters[0]);
	const auto &taxi = std::any_cast<SimpleTravelState::Object>(parameters[1]);
	const auto &src = std::any_cast<SimpleTravelState::Location>(parameters[2]);
	const auto &dst = std::any_cast<SimpleTravelState::Location>(parameters[3]);
	SimpleTravelState simpleTravelState = std::any_cast<SimpleTravelState>(state.data);

	auto currentPersonLocation = simpleTravelState.LocationOf(person);
	auto currentTaxiLocation = simpleTravelState.LocationOf(taxi);

	if (currentPersonLocation && currentTaxiLocation) {
		if ((currentPersonLocation.value() == src) && (currentTaxiLocation.value() == src)) {
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

	return std::nullopt;
}

std::optional<State> PayDriver(const State &state, const Parameters &parameters) {
	assert(parameters.size() == 1);
	assert(state.domain_name == DOMAIN_NAME);

	const auto &person = std::any_cast<SimpleTravelState::Object>(parameters[0]);
	SimpleTravelState simpleTravelState = std::any_cast<SimpleTravelState>(state.data);

	std::optional<unsigned long long> cashOwned = simpleTravelState.CashOwnedBy(person);
	std::optional<unsigned long long> owe = simpleTravelState.Owe(person);

	if (cashOwned && owe) {
		if (cashOwned.value() >= owe.value()) {
			State newState(state);

			simpleTravelState.SetCashOwnedBy(person, cashOwned.value() - owe.value());
			simpleTravelState.SetOwe(person, 0);
			newState.data = state;

			return newState;
		}
	}

	return std::nullopt;
}

std::optional<std::vector<Task>> TravelByFoot(const State &state, const Parameters &parameters) {
	assert(parameters.size() == 4);
	assert(state.domain_name == DOMAIN_NAME);

	const auto &person = std::any_cast<SimpleTravelState::Object>(parameters[0]);
	const auto &src = std::any_cast<SimpleTravelState::Location>(parameters[2]);
	const auto &dst = std::any_cast<SimpleTravelState::Location>(parameters[3]);
	SimpleTravelState simpleTravelState = std::any_cast<SimpleTravelState>(state.data);

	std::optional<unsigned long long> distance = simpleTravelState.DistanceBetween(src, dst);
	if (distance && distance.value() <= WALKING_DISTANCE) {
		Task task;
		task.task_name = WALK;
		task.parameters.push_back(person);
		task.parameters.push_back(src);
		task.parameters.push_back(dst);
		std::vector<Task> subtasks{ task };

		return subtasks;
	}

	return std::nullopt;
}

std::optional<std::vector<Task>> TravelByTaxi(const State &state, const Parameters &parameters) {
	assert(parameters.size() == 4);
	assert(state.domain_name == DOMAIN_NAME);

	const auto &person = std::any_cast<SimpleTravelState::Object>(parameters[0]);
	const auto &taxi = std::any_cast<SimpleTravelState::Object>(parameters[1]);
	const auto &src = std::any_cast<SimpleTravelState::Location>(parameters[2]);
	const auto &dst = std::any_cast<SimpleTravelState::Location>(parameters[3]);
	SimpleTravelState simpleTravelState = std::any_cast<SimpleTravelState>(state.data);

	std::optional<unsigned long long> cash = simpleTravelState.CashOwnedBy(person);
	std::optional<unsigned long long> distance = simpleTravelState.DistanceBetween(src, dst);
	auto currentPersonLocation = simpleTravelState.LocationOf(person);
	if (cash && distance && currentPersonLocation) {
		if (currentPersonLocation.value() == src) {
			if (cash.value() >= simpleTravelState.TaxiRate(distance.value())) {
				Task callTaxi;
				callTaxi.task_name = CALL_TAXI;
				callTaxi.parameters.push_back(person);
				callTaxi.parameters.push_back(taxi);

				Task rideTaxi;
				rideTaxi.task_name = RIDE_TAXI;
				rideTaxi.parameters.push_back(person);
				rideTaxi.parameters.push_back(taxi);
				rideTaxi.parameters.push_back(src);
				rideTaxi.parameters.push_back(dst);

				Task payDriver;
				payDriver.task_name = PAY_DRIVER;
				payDriver.parameters.push_back(person);

				std::vector<Task> subtasks{ payDriver, rideTaxi, callTaxi };

				return subtasks;
			}
		}
	}

	return std::nullopt;
}

TEST_CASE("[Modules][TotalOrderForwardDecomposition] Simple travel problem") {
	Task task;
	task.task_name = TRAVEL;
	task.parameters.push_back(SimpleTravelState::Object("me"));
	task.parameters.push_back(SimpleTravelState::Object("taxi"));
	task.parameters.push_back(SimpleTravelState::Location("home"));
	task.parameters.push_back(SimpleTravelState::Location("park"));
	TotalOrderForwardDecomposition tfd(CreatePlanningProblem(task));
	TotalOrderForwardDecomposition::Plan solutionPlan = tfd.try_to_plan();
	if (solutionPlan.empty()) {
		FAIL("Failed to plan");
	}
	MESSAGE("Found solution plan");
	for (const OperatorWithParams &_operator : solutionPlan) {
		MESSAGE(_operator.task.task_name.c_str());
	}
}
#endif // TEST_TFD_SIMPLE_H
