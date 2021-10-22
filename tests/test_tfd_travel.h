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

#include "../htn.h"
#include "../planning_problem.h"
#include <cassert>
#include <sstream>

class SimpleTravelState {
public:
	using Object = StringName;
	using Location = StringName;
	using Cash = int64_t;
	using Distance = int64_t;

	using PersonLocationTable = std::map<Object, Location>;
	using PersonCashTable = std::map<Object, Cash>;
	using PersonOweTable = std::map<Object, Cash>;
	using DistanceTable = std::map<Location, std::map<Location, Distance>>;

	SimpleTravelState(const PersonLocationTable &personLocationTable,
			const PersonCashTable &personCashTable,
			const PersonOweTable &personOweTable,
			const DistanceTable &distanceTable);
	virtual ~SimpleTravelState();

	std::optional<Location> location_of(const Object &person) const;
	std::optional<Cash> cast_owned_by(const Object &p_person) const;
	std::optional<Cash> owe(const Object &p_person) const;
	std::optional<Distance> distance_between(const Location &p_from, const Location &p_to) const;

	void set_location_of(const Object &p_person, const Location &p_location);
	void set_cash_ownedBy(const Object &p_person, const Cash &p_cash);
	void set_owe(const Object &p_person, const Cash &p_cash);

	Cash taxi_rate(const Distance &p_distance) const;

	constexpr static StringName DOMAIN_NAME = "Simple Travel";
	constexpr static float WALKING_DISTANCE = 2;

	// Tasks
	constexpr static StringName TASK_TRAVEL = "Travel";
	constexpr static StringName TASK_WALK = "Walk";
	constexpr static StringName TASK_CALL_TAXI = "Call Taxi";
	constexpr static StringName TASK_RIDE_TAXI = "Ride Taxi";
	constexpr static StringName TASK_PAY_DRIVER = "Pay Driver";

private:
	PersonLocationTable person_location_table;
	PersonCashTable person_cash_table;
	PersonOweTable person_owe_table;
	DistanceTable distance_table;
};

// Operators
std::optional<State> operator_walk(const State &p_state, const Parameters &p_parameters);
std::optional<State> operator_call_taxi(const State &p_state, const Parameters &p_parameters);
std::optional<State> operator_ride_taxi(const State &state, const Parameters &parameters);
std::optional<State> operator_pay_driver(const State &state, const Parameters &parameters);

// Methods
std::optional<std::vector<Task>> method_travel_by_foot(const State &state, const Parameters &parameters);
std::optional<std::vector<Task>> method_travel_by_taxi(const State &state, const Parameters &parameters);

PlanningDomain create_planning_domain();

// initial state
constexpr static const SimpleTravelState::PersonLocationTable init_person_location_table = { { SNAME("me"), SNAME("home") }, { SNAME("taxi"), SNAME("park") } };
constexpr static const SimpleTravelState::PersonCashTable init_person_cash_table = { { SNAME("me"), 20 } };
constexpr static const SimpleTravelState::PersonOweTable init_person_owe_table = { { SNAME("me"), 0 } };
constexpr static const SimpleTravelState::DistanceTable init_distance_table = { { SNAME("home"), { { SNAME("park"), 8 } } }, { SNAME("park"), { { SNAME("home"), 8 } } } };

static const SimpleTravelState INITIAL_STATE(init_person_location_table,
		init_person_cash_table,
		init_person_owe_table,
		init_distance_table);

PlanningProblem create_planning_problem(const Task &p_top_level_task) {
	PlanningDomain planning_domain = create_planning_domain();
	State state = { DOMAIN_NAME, INITIAL_STATE };

	return PlanningProblem(planning_domain, state, p_top_level_task);
}
PlanningDomain create_planning_domain() {
	PlanningDomain planning_domain(DOMAIN_NAME);

	planning_domain.add_operator(WALK, operator_walk);
	planning_domain.add_operator(CALL_TAXI, operator_call_taxi);
	planning_domain.add_operator(RIDE_TAXI, operator_ride_taxi);
	planning_domain.add_operator(PAY_DRIVER, operator_pay_driver);

	planning_domain.add_method(TRAVEL, method_travel_by_foot);
	planning_domain.add_method(TRAVEL, method_travel_by_taxi);

	return planning_domain;
}

SimpleTravelState::SimpleTravelState(const PersonLocationTable &p_person_location_table,
		const PersonCashTable &p_person_cash_table,
		const PersonOweTable &p_person_owe_table,
		const DistanceTable &p_distance_table) :
		p_person_location_table(p_person_location_table),
		person_cash_table(p_person_cash_table),
		person_owe_table(p_person_owe_table),
		distance_table(p_distance_table) {
}

SimpleTravelState::~SimpleTravelState() {
}

std::optional<SimpleTravelState::Location> SimpleTravelState::location_of(const Object &p_person) const {
	if (person_location_table.find(p_person) == person_location_table.end()) {
		return std::nullopt;
	} else {
		return person_location_table.at(p_person);
	}
}

void SimpleTravelState::set_location_of(const Object &p_person, const Location &p_location) {
	if (person_location_table.find(p_person) != person_location_table.end()) {
		person_location_table[p_person] = p_location;
	}
}

std::optional<SimpleTravelState::Cash> SimpleTravelState::cast_owned_by(const Object &p_person) const {
	if (person_cash_table.find(p_person) == person_cash_table.end()) {
		return std::nullopt;
	} else {
		return person_cash_table.at(p_person);
	}
}

void SimpleTravelState::set_cash_ownedBy(const Object &p_person, const Cash &p_cash) {
	if (person_cash_table.find(p_person) != person_cash_table.end()) {
		person_cash_table[p_person] = p_cash;
	}
}

std::optional<SimpleTravelState::Cash> SimpleTravelState::owe(const Object &p_person) const {
	if (person_owe_table.find(p_person) == person_owe_table.end()) {
		return std::nullopt;
	} else {
		return person_owe_table.at(p_person);
	}
}

void SimpleTravelState::set_owe(const Object &p_person, const Cash &p_cash) {
	if (person_owe_table.find(p_person) != person_owe_table.end()) {
		person_owe_table[p_person] = p_cash;
	}
}

std::optional<SimpleTravelState::Distance> SimpleTravelState::distance_between(const Location &p_from, const Location &p_to) const {
	if (distance_table.find(p_from) == distance_table.end()) {
		return std::nullopt;
	} else {
		const template <class _Kty, class _Ty, class _Pr = less<_Kty>, class _Alloc = allocator<pair<const _Kty, _Ty>>>
		class map : public _Tree<_Tmap_traits<_Kty, _Ty, _Pr, _Alloc, false>> &destination_table = distance_table.at(p_from);
		if (destination_table.find(p_to) == destination_table.end()) {
			return std::nullopt;
		} else {
			return destination_table.at(p_to);
		}
	}
}

SimpleTravelState::Cash SimpleTravelState::taxi_rate(const Distance &p_distance) const {
	return (1.5 + 0.5 * p_distance);
}

std::optional<State> operator_walk(const State &p_state, const Parameters &p_parameters) {
	assert(p_parameters.size() == 3);
	assert(p_state.domain_name == DOMAIN_NAME);

	const SimpleTravelState &person = std::any_cast<SimpleTravelState::Object>(p_parameters[0]);
	const SimpleTravelState &src = std::any_cast<SimpleTravelState::Location>(p_parameters[1]);
	const SimpleTravelState &dst = std::any_cast<SimpleTravelState::Location>(p_parameters[2]);
	SimpleTravelState simpleTravelState = std::any_cast<SimpleTravelState>(p_state.data);

	SimpleTravelState currentLocation = simpleTravelState.location_of(person);
	if (currentLocation && currentLocation.value() == src) {
		State newState(p_state);
		simpleTravelState.set_location_of(person, dst);
		newState.data = simpleTravelState;

		return newState;
	}
	return std::nullopt;
}

std::optional<State> operator_call_taxi(const State &p_state, const Parameters &p_parameters) {
	assert(p_parameters.size() == 2);
	assert(p_state.domain_name == DOMAIN_NAME);

	State newState(p_state);
	SimpleTravelState simpleTravelState = std::any_cast<SimpleTravelState>(newState.data);
	const SimpleTravelState &person = std::any_cast<SimpleTravelState::Object>(p_parameters[0]);
	const SimpleTravelState &taxi = std::any_cast<SimpleTravelState::Object>(p_parameters[1]);

	std::optional<SimpleTravelState::Location> personLocation = simpleTravelState.location_of(person);
	if (personLocation) {
		simpleTravelState.set_location_of(taxi, personLocation.value());
	}
	newState.data = simpleTravelState;

	return newState;

	return std::nullopt;
}

std::optional<State> operator_ride_taxi(const State &state, const Parameters &parameters) {
	assert(parameters.size() == 4);
	assert(state.domain_name == DOMAIN_NAME);

	const SimpleTravelState::Object &person = std::any_cast<SimpleTravelState::Object>(parameters[0]);
	const SimpleTravelState::Object &taxi = std::any_cast<SimpleTravelState::Object>(parameters[1]);
	const SimpleTravelState::Location &src = std::any_cast<SimpleTravelState::Location>(parameters[2]);
	const SimpleTravelState::Location &dst = std::any_cast<SimpleTravelState::Location>(parameters[3]);
	SimpleTravelState simple_travel_state = std::any_cast<SimpleTravelState>(state.data);

	std::optional<SimpleTravelState::Location> current_person_location = simple_travel_state.location_of(person);
	std::optional<SimpleTravelState::Location> current_taxi_location = simple_travel_state.location_of(taxi);

	if (current_person_location && current_taxi_location) {
		if ((current_person_location.value() == src) && (current_taxi_location.value() == src)) {
			State newState(state);
			std::optional<uint64_t> distance = simple_travel_state.distance_between(src, dst);

			if (distance) {
				simple_travel_state.set_location_of(person, dst);
				simple_travel_state.set_location_of(taxi, dst);
				simple_travel_state.set_owe(person, simple_travel_state.taxi_rate(distance.value()));
				newState.data = simple_travel_state;
			}

			return newState;
		}
	}

	return std::nullopt;
}

std::optional<State> operator_pay_driver(const State &state, const Parameters &parameters) {
	assert(parameters.size() == 1);
	assert(state.domain_name == DOMAIN_NAME);

	const SimpleTravelState::Object &person = std::any_cast<SimpleTravelState::Object>(parameters[0]);
	SimpleTravelState simple_travel_state = std::any_cast<SimpleTravelState>(state.data);

	std::optional<uint64_t> cash_owned = simple_travel_state.cast_owned_by(person);
	std::optional<uint64_t> owe = simple_travel_state.owe(person);

	if (cash_owned && owe) {
		if (cash_owned.value() >= owe.value()) {
			State newState(state);

			simple_travel_state.set_cash_ownedBy(person, cash_owned.value() - owe.value());
			simple_travel_state.set_owe(person, 0);
			newState.data = state;

			return newState;
		}
	}

	return std::nullopt;
}

std::optional<std::vector<Task>> method_travel_by_foot(const State &state, const Parameters &parameters) {
	assert(parameters.size() == 4);
	assert(state.domain_name == DOMAIN_NAME);

	const auto &person = std::any_cast<SimpleTravelState::Object>(parameters[0]);
	const auto &src = std::any_cast<SimpleTravelState::Location>(parameters[2]);
	const auto &dst = std::any_cast<SimpleTravelState::Location>(parameters[3]);
	SimpleTravelState simpleTravelState = std::any_cast<SimpleTravelState>(state.data);

	std::optional<uint64_t> distance = simpleTravelState.distance_between(src, dst);
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

std::optional<std::vector<Task>> method_travel_by_taxi(const State &state, const Parameters &parameters) {
	assert(parameters.size() == 4);
	assert(state.domain_name == DOMAIN_NAME);

	const auto &person = std::any_cast<SimpleTravelState::Object>(parameters[0]);
	const auto &taxi = std::any_cast<SimpleTravelState::Object>(parameters[1]);
	const auto &src = std::any_cast<SimpleTravelState::Location>(parameters[2]);
	const auto &dst = std::any_cast<SimpleTravelState::Location>(parameters[3]);
	SimpleTravelState simpleTravelState = std::any_cast<SimpleTravelState>(state.data);

	std::optional<uint64_t> cash = simpleTravelState.cast_owned_by(person);
	std::optional<uint64_t> distance = simpleTravelState.distance_between(src, dst);
	auto currentPersonLocation = simpleTravelState.location_of(person);
	if (cash && distance && currentPersonLocation) {
		if (currentPersonLocation.value() == src) {
			if (cash.value() >= simpleTravelState.taxi_rate(distance.value())) {
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
	TotalOrderForwardDecomposition tfd(create_planning_problem(task));
	TotalOrderForwardDecomposition::Plan solutionPlan = tfd.try_to_plan();
	if (solutionPlan.empty()) {
		FAIL("Failed to plan");
	}
	MESSAGE("Found solution plan");
	for (const OperatorWithParams &_operator : solutionPlan) {
		MESSAGE(_operator.task.task_name);
	}
}
#endif // TEST_TFD_SIMPLE_H
