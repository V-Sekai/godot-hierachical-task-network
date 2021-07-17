#include "register_types.h"

#include "planner.h"

void register_tfd_types() {
	ClassDB::register_class<TotalOrderForwardDecomposition>();
	ClassDB::register_class<PlanningDomain>();
	ClassDB::register_class<State>();
	ClassDB::register_class<Task>();
}

void unregister_tfd_types() {
}