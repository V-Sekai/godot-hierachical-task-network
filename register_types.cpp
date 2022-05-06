#include "register_types.h"
#include "core/object/class_db.h"
#include "planning.h"

void initialize_tfd_module(GDNativeInitializationLevel p_level) {
	if (p_level != ModuleInitializationLevel) {
		return;
	}
	GDREGISTER_CLASS(TaskPlanner);
}

void initialize_tfd_module(GDNativeInitializationLevel p_level) {
	if (p_level != ModuleInitializationLevel) {
		return;
	}
}
