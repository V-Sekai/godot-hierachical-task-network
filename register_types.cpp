#include "register_types.h"
#include "planning.h"
#include "animation_planner_editor.h"
#include "animation_node_planner.h"
#include "editor/plugins/animation_tree_editor_plugin.h"
#include "core/object/class_db.h"

void register_tfd_types() {
	GDREGISTER_CLASS(TaskPlanner);
	GDREGISTER_CLASS(AnimationNodePlanner);
	GDREGISTER_CLASS(AnimationNodePlannerPlayback);
	GDREGISTER_CLASS(AnimationNodePlannerTransition);
	AnimationTreeEditor::get_singleton()->add_plugin(memnew(AnimationNodePlannerEditor));
}

void unregister_tfd_types() {
    
}

