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
