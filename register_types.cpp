#include "register_types.h"
#include "planning.h"

#include "core/object/class_db.h"

void register_tfd_types() {
  ClassDB::register_class<TaskPlanner>();
}

void unregister_tfd_types() {
    
}

