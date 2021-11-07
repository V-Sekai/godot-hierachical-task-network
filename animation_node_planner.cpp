/*************************************************************************/
/*  animation_node_state_machine.cpp                                     */
/*************************************************************************/
/*                       This file is part of:                           */
/*                           GODOT ENGINE                                */
/*                      https://godotengine.org                          */
/*************************************************************************/
/* Copyright (c) 2007-2021 Juan Linietsky, Ariel Manzur.                 */
/* Copyright (c) 2014-2021 Godot Engine contributors (cf. AUTHORS.md).   */
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

#include "animation_node_planner.h"

/////////////////////////////////////////////////

void AnimationNodePlannerTransition::set_switch_mode(SwitchMode p_mode) {
	switch_mode = p_mode;
}

AnimationNodePlannerTransition::SwitchMode AnimationNodePlannerTransition::get_switch_mode() const {
	return switch_mode;
}

void AnimationNodePlannerTransition::set_auto_advance(bool p_enable) {
	auto_advance = p_enable;
}

bool AnimationNodePlannerTransition::has_auto_advance() const {
	return auto_advance;
}

void AnimationNodePlannerTransition::set_advance_condition(const StringName &p_condition) {
	String cs = p_condition;
	ERR_FAIL_COND(cs.find("/") != -1 || cs.find(":") != -1);
	advance_condition = p_condition;
	if (cs != String()) {
		advance_condition_name = "conditions/" + cs;
	} else {
		advance_condition_name = StringName();
	}
	emit_signal(SNAME("advance_condition_changed"));
}

StringName AnimationNodePlannerTransition::get_advance_condition() const {
	return advance_condition;
}

StringName AnimationNodePlannerTransition::get_advance_condition_name() const {
	return advance_condition_name;
}

void AnimationNodePlannerTransition::set_advance_expression(const String &p_expression) {
	advance_expression = p_expression.strip_edges();
	if (advance_expression.strip_edges() == String()) {
		expression.unref();
		return;
	}

	if (expression.is_null()) {
		expression.instantiate();
	}

	expression->parse(advance_expression);
}

String AnimationNodePlannerTransition::get_advance_expression() const {
	return advance_expression;
}

void AnimationNodePlannerTransition::set_advance_expression_base_node(const NodePath &p_expression_base_node) {
	advance_expression_base_node = p_expression_base_node;
}

NodePath AnimationNodePlannerTransition::get_advance_expression_base_node() const {
	return advance_expression_base_node;
}

void AnimationNodePlannerTransition::set_xfade_time(float p_xfade) {
	ERR_FAIL_COND(p_xfade < 0);
	xfade = p_xfade;
	emit_changed();
}

float AnimationNodePlannerTransition::get_xfade_time() const {
	return xfade;
}

void AnimationNodePlannerTransition::set_disabled(bool p_disabled) {
	disabled = p_disabled;
	emit_changed();
}

bool AnimationNodePlannerTransition::is_disabled() const {
	return disabled;
}

void AnimationNodePlannerTransition::set_priority(int p_priority) {
	priority = p_priority;
	emit_changed();
}

int AnimationNodePlannerTransition::get_priority() const {
	return priority;
}

void AnimationNodePlannerTransition::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_switch_mode", "mode"), &AnimationNodePlannerTransition::set_switch_mode);
	ClassDB::bind_method(D_METHOD("get_switch_mode"), &AnimationNodePlannerTransition::get_switch_mode);

	ClassDB::bind_method(D_METHOD("set_auto_advance", "auto_advance"), &AnimationNodePlannerTransition::set_auto_advance);
	ClassDB::bind_method(D_METHOD("has_auto_advance"), &AnimationNodePlannerTransition::has_auto_advance);

	ClassDB::bind_method(D_METHOD("set_advance_condition", "name"), &AnimationNodePlannerTransition::set_advance_condition);
	ClassDB::bind_method(D_METHOD("get_advance_condition"), &AnimationNodePlannerTransition::get_advance_condition);

	ClassDB::bind_method(D_METHOD("set_xfade_time", "secs"), &AnimationNodePlannerTransition::set_xfade_time);
	ClassDB::bind_method(D_METHOD("get_xfade_time"), &AnimationNodePlannerTransition::get_xfade_time);

	ClassDB::bind_method(D_METHOD("set_disabled", "disabled"), &AnimationNodePlannerTransition::set_disabled);
	ClassDB::bind_method(D_METHOD("is_disabled"), &AnimationNodePlannerTransition::is_disabled);

	ClassDB::bind_method(D_METHOD("set_priority", "priority"), &AnimationNodePlannerTransition::set_priority);
	ClassDB::bind_method(D_METHOD("get_priority"), &AnimationNodePlannerTransition::get_priority);

	ClassDB::bind_method(D_METHOD("set_advance_expression", "text"), &AnimationNodePlannerTransition::set_advance_expression);
	ClassDB::bind_method(D_METHOD("get_advance_expression"), &AnimationNodePlannerTransition::get_advance_expression);

	ClassDB::bind_method(D_METHOD("set_advance_expression_base_node", "path"), &AnimationNodePlannerTransition::set_advance_expression_base_node);
	ClassDB::bind_method(D_METHOD("get_advance_expression_base_node"), &AnimationNodePlannerTransition::get_advance_expression_base_node);

	ADD_GROUP("Switch", "");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "switch_mode", PROPERTY_HINT_ENUM, "Immediate,Sync,At End"), "set_switch_mode", "get_switch_mode");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "xfade_time", PROPERTY_HINT_RANGE, "0,240,0.01"), "set_xfade_time", "get_xfade_time");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "priority", PROPERTY_HINT_RANGE, "0,32,1"), "set_priority", "get_priority");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "auto_advance"), "set_auto_advance", "has_auto_advance");
	ADD_GROUP("Advance", "advance_");
	ADD_PROPERTY(PropertyInfo(Variant::STRING_NAME, "advance_condition"), "set_advance_condition", "get_advance_condition");
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "advance_expression", PROPERTY_HINT_EXPRESSION, ""), "set_advance_expression", "get_advance_expression");
	ADD_PROPERTY(PropertyInfo(Variant::NODE_PATH, "advance_expression_base_node"), "set_advance_expression_base_node", "get_advance_expression_base_node");
	ADD_GROUP("Disabling", "");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "disabled"), "set_disabled", "is_disabled");

	BIND_ENUM_CONSTANT(SWITCH_MODE_IMMEDIATE);
	BIND_ENUM_CONSTANT(SWITCH_MODE_SYNC);
	BIND_ENUM_CONSTANT(SWITCH_MODE_AT_END);

	ADD_SIGNAL(MethodInfo("advance_condition_changed"));
}

AnimationNodePlannerTransition::AnimationNodePlannerTransition() {
}

////////////////////////////////////////////////////////

void AnimationNodePlannerPlayback::travel(const StringName &p_state) {
	start_request_travel = true;
	start_request = p_state;
	stop_request = false;
}

void AnimationNodePlannerPlayback::start(const StringName &p_state) {
	start_request_travel = false;
	start_request = p_state;
	stop_request = false;
}

void AnimationNodePlannerPlayback::stop() {
	stop_request = true;
}

bool AnimationNodePlannerPlayback::is_playing() const {
	return playing;
}

StringName AnimationNodePlannerPlayback::get_current_node() const {
	return current;
}

StringName AnimationNodePlannerPlayback::get_blend_from_node() const {
	return fading_from;
}

Vector<StringName> AnimationNodePlannerPlayback::get_travel_path() const {
	return path;
}

float AnimationNodePlannerPlayback::get_current_play_pos() const {
	return pos_current;
}

float AnimationNodePlannerPlayback::get_current_length() const {
	return len_current;
}

bool AnimationNodePlannerPlayback::_travel(AnimationNodePlanner *p_state_machine, const StringName &p_travel) {
	ERR_FAIL_COND_V(!playing, false);
	ERR_FAIL_COND_V(!p_state_machine->states.has(p_travel), false);
	ERR_FAIL_COND_V(!p_state_machine->states.has(current), false);

	path.clear(); //a new one will be needed

	if (current == p_travel) {
		return true; //nothing to do
	}

	loops_current = 0; // reset loops, so fade does not happen immediately

	Vector2 current_pos = p_state_machine->states[current].position;
	Vector2 target_pos = p_state_machine->states[p_travel].position;

	Map<StringName, AStarCost> cost_map;

	List<int> open_list;

	//build open list
	for (int i = 0; i < p_state_machine->transitions.size(); i++) {
		if (p_state_machine->transitions[i].from == current) {
			open_list.push_back(i);
			float cost = p_state_machine->states[p_state_machine->transitions[i].to].position.distance_to(current_pos);
			cost *= p_state_machine->transitions[i].transition->get_priority();
			AStarCost ap;
			ap.prev = current;
			ap.distance = cost;
			cost_map[p_state_machine->transitions[i].to] = ap;

			if (p_state_machine->transitions[i].to == p_travel) { //prematurely found it! :D
				path.push_back(p_travel);
				return true;
			}
		}
	}

	//begin astar
	bool found_route = false;
	while (!found_route) {
		if (open_list.size() == 0) {
			return false; //no path found
		}

		//find the last cost transition
		List<int>::Element *least_cost_transition = nullptr;
		float least_cost = 1e20;

		for (List<int>::Element *E = open_list.front(); E; E = E->next()) {
			float cost = cost_map[p_state_machine->transitions[E->get()].to].distance;
			cost += p_state_machine->states[p_state_machine->transitions[E->get()].to].position.distance_to(target_pos);

			if (cost < least_cost) {
				least_cost_transition = E;
				least_cost = cost;
			}
		}

		StringName transition_prev = p_state_machine->transitions[least_cost_transition->get()].from;
		StringName transition = p_state_machine->transitions[least_cost_transition->get()].to;

		for (int i = 0; i < p_state_machine->transitions.size(); i++) {
			if (p_state_machine->transitions[i].from != transition || p_state_machine->transitions[i].to == transition_prev) {
				continue; //not interested on those
			}

			float distance = p_state_machine->states[p_state_machine->transitions[i].from].position.distance_to(p_state_machine->states[p_state_machine->transitions[i].to].position);
			distance *= p_state_machine->transitions[i].transition->get_priority();
			distance += cost_map[p_state_machine->transitions[i].from].distance;

			if (cost_map.has(p_state_machine->transitions[i].to)) {
				//oh this was visited already, can we win the cost?
				if (distance < cost_map[p_state_machine->transitions[i].to].distance) {
					cost_map[p_state_machine->transitions[i].to].distance = distance;
					cost_map[p_state_machine->transitions[i].to].prev = p_state_machine->transitions[i].from;
				}
			} else {
				//add to open list
				AStarCost ac;
				ac.prev = p_state_machine->transitions[i].from;
				ac.distance = distance;
				cost_map[p_state_machine->transitions[i].to] = ac;

				open_list.push_back(i);

				if (p_state_machine->transitions[i].to == p_travel) {
					found_route = true;
					break;
				}
			}
		}

		if (found_route) {
			break;
		}

		open_list.erase(least_cost_transition);
	}

	//make path
	StringName at = p_travel;
	while (at != current) {
		path.push_back(at);
		at = cost_map[at].prev;
	}

	path.reverse();

	return true;
}

double AnimationNodePlannerPlayback::process(AnimationNodePlanner *p_state_machine, double p_time, bool p_seek) {
	//if not playing and it can restart, then restart
	if (!playing && start_request == StringName()) {
		if (!stop_request && p_state_machine->start_node) {
			start(p_state_machine->start_node);
		} else {
			return 0;
		}
	}

	if (playing && stop_request) {
		stop_request = false;
		playing = false;
		return 0;
	}

	bool play_start = false;

	if (start_request != StringName()) {
		if (start_request_travel) {
			if (!playing) {
				if (!stop_request && p_state_machine->start_node) {
					// can restart, just postpone traveling
					path.clear();
					current = p_state_machine->start_node;
					playing = true;
					play_start = true;
				} else {
					// stopped, invalid state
					String node_name = start_request;
					start_request = StringName(); //clear start request
					ERR_FAIL_V_MSG(0, "Can't travel to '" + node_name + "' if state machine is not playing. Maybe you need to enable Autoplay on Load for one of the nodes in your state machine or call .start() first?");
				}
			} else {
				if (!_travel(p_state_machine, start_request)) {
					// can't travel, then teleport
					path.clear();
					current = start_request;
				}
				start_request = StringName(); //clear start request
			}
		} else {
			// teleport to start
			if (p_state_machine->states.has(start_request)) {
				path.clear();
				current = start_request;
				playing = true;
				play_start = true;
				start_request = StringName(); //clear start request
			} else {
				StringName node = start_request;
				start_request = StringName(); //clear start request
				ERR_FAIL_V_MSG(0, "No such node: '" + node + "'");
			}
		}
	}

	bool do_start = (p_seek && p_time == 0) || play_start || current == StringName();

	if (do_start) {
		if (p_state_machine->start_node != StringName() && p_seek && p_time == 0) {
			current = p_state_machine->start_node;
		}

		len_current = p_state_machine->blend_node(current, p_state_machine->states[current].node, 0, true, 1.0, AnimationNode::FILTER_IGNORE, false);
		pos_current = 0;
		loops_current = 0;
	}

	if (!p_state_machine->states.has(current)) {
		playing = false; //current does not exist
		current = StringName();
		return 0;
	}
	float fade_blend = 1.0;

	if (fading_from != StringName()) {
		if (!p_state_machine->states.has(fading_from)) {
			fading_from = StringName();
		} else {
			if (!p_seek) {
				fading_pos += p_time;
			}
			fade_blend = MIN(1.0, fading_pos / fading_time);
			if (fade_blend >= 1.0) {
				fading_from = StringName();
			}
		}
	}

	float rem = p_state_machine->blend_node(current, p_state_machine->states[current].node, p_time, p_seek, fade_blend, AnimationNode::FILTER_IGNORE, false);

	if (fading_from != StringName()) {
		p_state_machine->blend_node(fading_from, p_state_machine->states[fading_from].node, p_time, p_seek, 1.0 - fade_blend, AnimationNode::FILTER_IGNORE, false);
	}

	//guess playback position
	if (rem > len_current) { // weird but ok
		len_current = rem;
	}

	{ //advance and loop check

		float next_pos = len_current - rem;

		if (next_pos < pos_current) {
			loops_current++;
		}
		pos_current = next_pos; //looped
	}

	//find next
	StringName next;
	float next_xfade = 0.0;
	AnimationNodePlannerTransition::SwitchMode switch_mode = AnimationNodePlannerTransition::SWITCH_MODE_IMMEDIATE;

	if (path.size()) {
		for (int i = 0; i < p_state_machine->transitions.size(); i++) {
			if (p_state_machine->transitions[i].from == current && p_state_machine->transitions[i].to == path[0]) {
				next_xfade = p_state_machine->transitions[i].transition->get_xfade_time();
				switch_mode = p_state_machine->transitions[i].transition->get_switch_mode();
				next = path[0];
			}
		}
	} else {
		float priority_best = 1e20;
		int auto_advance_to = -1;
		for (int i = 0; i < p_state_machine->transitions.size(); i++) {
			bool auto_advance = false;
			if (p_state_machine->transitions[i].transition->has_auto_advance()) {
				auto_advance = true;
			}
			StringName advance_condition_name = p_state_machine->transitions[i].transition->get_advance_condition_name();
			if (advance_condition_name != StringName() && bool(p_state_machine->get_parameter(advance_condition_name))) {
				auto_advance = true;
			}

			if (p_state_machine->transitions[i].from == current && p_state_machine->transitions[i].transition->expression.is_valid()) {
				Node *base = p_state_machine->get_animation_tree();
				ERR_CONTINUE(base == nullptr);
				Ref<Expression> exp = p_state_machine->transitions[i].transition->expression;
				base = base->get_node_or_null(p_state_machine->transitions[i].transition->advance_expression_base_node);
				if (base) {
					bool ret = exp->execute(Array(), base, false, Engine::get_singleton()->is_editor_hint()); // Avoid user from crashing the system with an expression by only allowing const calls when editor runs
					if (!exp->has_execute_failed()) {
						auto_advance = ret;
					}
				}
			}

			if (p_state_machine->transitions[i].from == current && auto_advance) {
				if (p_state_machine->transitions[i].transition->get_priority() <= priority_best) {
					priority_best = p_state_machine->transitions[i].transition->get_priority();
					auto_advance_to = i;
				}
			}
		}

		if (auto_advance_to != -1) {
			next = p_state_machine->transitions[auto_advance_to].to;
			next_xfade = p_state_machine->transitions[auto_advance_to].transition->get_xfade_time();
			switch_mode = p_state_machine->transitions[auto_advance_to].transition->get_switch_mode();
		}
	}

	//if next, see when to transition
	if (next != StringName()) {
		bool goto_next = false;

		if (switch_mode == AnimationNodePlannerTransition::SWITCH_MODE_AT_END) {
			goto_next = next_xfade >= (len_current - pos_current) || loops_current > 0;
			if (loops_current > 0) {
				next_xfade = 0;
			}
		} else {
			goto_next = fading_from == StringName();
		}

		if (goto_next) { //loops should be used because fade time may be too small or zero and animation may have looped

			if (next_xfade) {
				//time to fade, baby
				fading_from = current;
				fading_time = next_xfade;
				fading_pos = 0;
			} else {
				fading_from = StringName();
				fading_pos = 0;
			}

			if (path.size()) { //if it came from path, remove path
				path.remove(0);
			}
			current = next;
			if (switch_mode == AnimationNodePlannerTransition::SWITCH_MODE_SYNC) {
				len_current = p_state_machine->blend_node(current, p_state_machine->states[current].node, 0, true, 0, AnimationNode::FILTER_IGNORE, false);
				pos_current = MIN(pos_current, len_current);
				p_state_machine->blend_node(current, p_state_machine->states[current].node, pos_current, true, 0, AnimationNode::FILTER_IGNORE, false);

			} else {
				len_current = p_state_machine->blend_node(current, p_state_machine->states[current].node, 0, true, 0, AnimationNode::FILTER_IGNORE, false);
				pos_current = 0;
			}

			rem = len_current; //so it does not show 0 on transition
			loops_current = 0;
		}
	}

	//compute time left for transitions by using the end node
	if (p_state_machine->end_node != StringName() && p_state_machine->end_node != current) {
		rem = p_state_machine->blend_node(p_state_machine->end_node, p_state_machine->states[p_state_machine->end_node].node, 0, true, 0, AnimationNode::FILTER_IGNORE, false);
	}

	return rem;
}

void AnimationNodePlannerPlayback::_bind_methods() {
	ClassDB::bind_method(D_METHOD("travel", "to_node"), &AnimationNodePlannerPlayback::travel);
	ClassDB::bind_method(D_METHOD("start", "node"), &AnimationNodePlannerPlayback::start);
	ClassDB::bind_method(D_METHOD("stop"), &AnimationNodePlannerPlayback::stop);
	ClassDB::bind_method(D_METHOD("is_playing"), &AnimationNodePlannerPlayback::is_playing);
	ClassDB::bind_method(D_METHOD("get_current_node"), &AnimationNodePlannerPlayback::get_current_node);
	ClassDB::bind_method(D_METHOD("get_current_play_position"), &AnimationNodePlannerPlayback::get_current_play_pos);
	ClassDB::bind_method(D_METHOD("get_current_length"), &AnimationNodePlannerPlayback::get_current_length);
	ClassDB::bind_method(D_METHOD("get_travel_path"), &AnimationNodePlannerPlayback::get_travel_path);
}

AnimationNodePlannerPlayback::AnimationNodePlannerPlayback() {
	set_local_to_scene(true); //only one per instantiated scene
}

///////////////////////////////////////////////////////

void AnimationNodePlanner::get_parameter_list(List<PropertyInfo> *r_list) const {
	r_list->push_back(PropertyInfo(Variant::OBJECT, playback, PROPERTY_HINT_RESOURCE_TYPE, "AnimationNodePlannerPlayback", PROPERTY_USAGE_DEFAULT | PROPERTY_USAGE_DO_NOT_SHARE_ON_DUPLICATE));
	List<StringName> advance_conditions;
	for (int i = 0; i < transitions.size(); i++) {
		StringName ac = transitions[i].transition->get_advance_condition_name();
		if (ac != StringName() && advance_conditions.find(ac) == nullptr) {
			advance_conditions.push_back(ac);
		}
	}

	advance_conditions.sort_custom<StringName::AlphCompare>();
	for (const StringName &E : advance_conditions) {
		r_list->push_back(PropertyInfo(Variant::BOOL, E));
	}
}

Variant AnimationNodePlanner::get_parameter_default_value(const StringName &p_parameter) const {
	if (p_parameter == playback) {
		Ref<AnimationNodePlannerPlayback> p;
		p.instantiate();
		return p;
	} else {
		return false; //advance condition
	}
}

void AnimationNodePlanner::add_node(const StringName &p_name, Ref<AnimationNode> p_node, const Vector2 &p_position) {
	ERR_FAIL_COND(states.has(p_name));
	ERR_FAIL_COND(p_node.is_null());
	ERR_FAIL_COND(String(p_name).find("/") != -1);

	State state;
	state.node = p_node;
	state.position = p_position;

	states[p_name] = state;

	emit_changed();
	emit_signal(SNAME("tree_changed"));

	p_node->connect("tree_changed", callable_mp(this, &AnimationNodePlanner::_tree_changed), varray(), CONNECT_REFERENCE_COUNTED);
}

void AnimationNodePlanner::replace_node(const StringName &p_name, Ref<AnimationNode> p_node) {
	ERR_FAIL_COND(states.has(p_name) == false);
	ERR_FAIL_COND(p_node.is_null());
	ERR_FAIL_COND(String(p_name).find("/") != -1);

	{
		Ref<AnimationNode> node = states[p_name].node;
		if (node.is_valid()) {
			node->disconnect("tree_changed", callable_mp(this, &AnimationNodePlanner::_tree_changed));
		}
	}

	states[p_name].node = p_node;

	emit_changed();
	emit_signal(SNAME("tree_changed"));

	p_node->connect("tree_changed", callable_mp(this, &AnimationNodePlanner::_tree_changed), varray(), CONNECT_REFERENCE_COUNTED);
}

Ref<AnimationNode> AnimationNodePlanner::get_node(const StringName &p_name) const {
	ERR_FAIL_COND_V(!states.has(p_name), Ref<AnimationNode>());

	return states[p_name].node;
}

StringName AnimationNodePlanner::get_node_name(const Ref<AnimationNode> &p_node) const {
	for (const KeyValue<StringName, State> &E : states) {
		if (E.value.node == p_node) {
			return E.key;
		}
	}

	ERR_FAIL_V(StringName());
}

void AnimationNodePlanner::get_child_nodes(List<ChildNode> *r_child_nodes) {
	Vector<StringName> nodes;

	for (const KeyValue<StringName, State> &E : states) {
		nodes.push_back(E.key);
	}

	nodes.sort_custom<StringName::AlphCompare>();

	for (int i = 0; i < nodes.size(); i++) {
		ChildNode cn;
		cn.name = nodes[i];
		cn.node = states[cn.name].node;
		r_child_nodes->push_back(cn);
	}
}

bool AnimationNodePlanner::has_node(const StringName &p_name) const {
	return states.has(p_name);
}

void AnimationNodePlanner::remove_node(const StringName &p_name) {
	ERR_FAIL_COND(!states.has(p_name));

	{
		Ref<AnimationNode> node = states[p_name].node;

		ERR_FAIL_COND(node.is_null());

		node->disconnect("tree_changed", callable_mp(this, &AnimationNodePlanner::_tree_changed));
	}

	states.erase(p_name);
	//path.erase(p_name);

	for (int i = 0; i < transitions.size(); i++) {
		if (transitions[i].from == p_name || transitions[i].to == p_name) {
			transitions.write[i].transition->disconnect("advance_condition_changed", callable_mp(this, &AnimationNodePlanner::_tree_changed));
			transitions.remove(i);
			i--;
		}
	}

	if (start_node == p_name) {
		start_node = StringName();
	}

	if (end_node == p_name) {
		end_node = StringName();
	}

	/*if (playing && current == p_name) {
		stop();
	}*/

	emit_changed();
	emit_signal(SNAME("tree_changed"));
}

void AnimationNodePlanner::rename_node(const StringName &p_name, const StringName &p_new_name) {
	ERR_FAIL_COND(!states.has(p_name));
	ERR_FAIL_COND(states.has(p_new_name));

	states[p_new_name] = states[p_name];
	states.erase(p_name);

	for (int i = 0; i < transitions.size(); i++) {
		if (transitions[i].from == p_name) {
			transitions.write[i].from = p_new_name;
		}

		if (transitions[i].to == p_name) {
			transitions.write[i].to = p_new_name;
		}
	}

	if (start_node == p_name) {
		start_node = p_new_name;
	}

	if (end_node == p_name) {
		end_node = p_new_name;
	}

	/*if (playing && current == p_name) {
		current = p_new_name;
	}*/

	//path.clear(); //clear path
	emit_signal(SNAME("tree_changed"));
}

void AnimationNodePlanner::get_node_list(List<StringName> *r_nodes) const {
	List<StringName> nodes;
	for (const KeyValue<StringName, State> &E : states) {
		nodes.push_back(E.key);
	}
	nodes.sort_custom<StringName::AlphCompare>();

	for (const StringName &E : nodes) {
		r_nodes->push_back(E);
	}
}

bool AnimationNodePlanner::has_transition(const StringName &p_from, const StringName &p_to) const {
	for (int i = 0; i < transitions.size(); i++) {
		if (transitions[i].from == p_from && transitions[i].to == p_to) {
			return true;
		}
	}
	return false;
}

int AnimationNodePlanner::find_transition(const StringName &p_from, const StringName &p_to) const {
	for (int i = 0; i < transitions.size(); i++) {
		if (transitions[i].from == p_from && transitions[i].to == p_to) {
			return i;
		}
	}
	return -1;
}

void AnimationNodePlanner::add_transition(const StringName &p_from, const StringName &p_to, const Ref<AnimationNodePlannerTransition> &p_transition) {
	ERR_FAIL_COND(p_from == p_to);
	ERR_FAIL_COND(!states.has(p_from));
	ERR_FAIL_COND(!states.has(p_to));
	ERR_FAIL_COND(p_transition.is_null());

	for (int i = 0; i < transitions.size(); i++) {
		ERR_FAIL_COND(transitions[i].from == p_from && transitions[i].to == p_to);
	}

	Transition tr;
	tr.from = p_from;
	tr.to = p_to;
	tr.transition = p_transition;

	tr.transition->connect("advance_condition_changed", callable_mp(this, &AnimationNodePlanner::_tree_changed), varray(), CONNECT_REFERENCE_COUNTED);

	transitions.push_back(tr);
}

Ref<AnimationNodePlannerTransition> AnimationNodePlanner::get_transition(int p_transition) const {
	ERR_FAIL_INDEX_V(p_transition, transitions.size(), Ref<AnimationNodePlannerTransition>());
	return transitions[p_transition].transition;
}

StringName AnimationNodePlanner::get_transition_from(int p_transition) const {
	ERR_FAIL_INDEX_V(p_transition, transitions.size(), StringName());
	return transitions[p_transition].from;
}

StringName AnimationNodePlanner::get_transition_to(int p_transition) const {
	ERR_FAIL_INDEX_V(p_transition, transitions.size(), StringName());
	return transitions[p_transition].to;
}

int AnimationNodePlanner::get_transition_count() const {
	return transitions.size();
}

void AnimationNodePlanner::remove_transition(const StringName &p_from, const StringName &p_to) {
	for (int i = 0; i < transitions.size(); i++) {
		if (transitions[i].from == p_from && transitions[i].to == p_to) {
			transitions.write[i].transition->disconnect("advance_condition_changed", callable_mp(this, &AnimationNodePlanner::_tree_changed));
			transitions.remove(i);
			return;
		}
	}

	/*if (playing) {
		path.clear();
	}*/
}

void AnimationNodePlanner::remove_transition_by_index(int p_transition) {
	ERR_FAIL_INDEX(p_transition, transitions.size());
	transitions.write[p_transition].transition->disconnect("advance_condition_changed", callable_mp(this, &AnimationNodePlanner::_tree_changed));
	transitions.remove(p_transition);
	/*if (playing) {
		path.clear();
	}*/
}

void AnimationNodePlanner::set_start_node(const StringName &p_node) {
	ERR_FAIL_COND(p_node != StringName() && !states.has(p_node));
	start_node = p_node;
}

String AnimationNodePlanner::get_start_node() const {
	return start_node;
}

void AnimationNodePlanner::set_end_node(const StringName &p_node) {
	ERR_FAIL_COND(p_node != StringName() && !states.has(p_node));
	end_node = p_node;
}

String AnimationNodePlanner::get_end_node() const {
	return end_node;
}

void AnimationNodePlanner::set_graph_offset(const Vector2 &p_offset) {
	graph_offset = p_offset;
}

Vector2 AnimationNodePlanner::get_graph_offset() const {
	return graph_offset;
}

double AnimationNodePlanner::process(double p_time, bool p_seek) {
	Ref<AnimationNodePlannerPlayback> playback = get_parameter(this->playback);
	ERR_FAIL_COND_V(playback.is_null(), 0.0);

	return playback->process(this, p_time, p_seek);
}

String AnimationNodePlanner::get_caption() const {
	return "StateMachine";
}

void AnimationNodePlanner::_notification(int p_what) {
}

Ref<AnimationNode> AnimationNodePlanner::get_child_by_name(const StringName &p_name) {
	return get_node(p_name);
}

bool AnimationNodePlanner::_set(const StringName &p_name, const Variant &p_value) {
	String name = p_name;
	if (name.begins_with("states/")) {
		String node_name = name.get_slicec('/', 1);
		String what = name.get_slicec('/', 2);

		if (what == "node") {
			Ref<AnimationNode> anode = p_value;
			if (anode.is_valid()) {
				add_node(node_name, p_value);
			}
			return true;
		}

		if (what == "position") {
			if (states.has(node_name)) {
				states[node_name].position = p_value;
			}
			return true;
		}
	} else if (name == "transitions") {
		Array trans = p_value;
		ERR_FAIL_COND_V(trans.size() % 3 != 0, false);

		for (int i = 0; i < trans.size(); i += 3) {
			add_transition(trans[i], trans[i + 1], trans[i + 2]);
		}
		return true;
	} else if (name == "start_node") {
		set_start_node(p_value);
		return true;
	} else if (name == "end_node") {
		set_end_node(p_value);
		return true;
	} else if (name == "graph_offset") {
		set_graph_offset(p_value);
		return true;
	}

	return false;
}

bool AnimationNodePlanner::_get(const StringName &p_name, Variant &r_ret) const {
	String name = p_name;
	if (name.begins_with("states/")) {
		String node_name = name.get_slicec('/', 1);
		String what = name.get_slicec('/', 2);

		if (what == "node") {
			if (states.has(node_name)) {
				r_ret = states[node_name].node;
				return true;
			}
		}

		if (what == "position") {
			if (states.has(node_name)) {
				r_ret = states[node_name].position;
				return true;
			}
		}
	} else if (name == "transitions") {
		Array trans;
		trans.resize(transitions.size() * 3);

		for (int i = 0; i < transitions.size(); i++) {
			trans[i * 3 + 0] = transitions[i].from;
			trans[i * 3 + 1] = transitions[i].to;
			trans[i * 3 + 2] = transitions[i].transition;
		}

		r_ret = trans;
		return true;
	} else if (name == "start_node") {
		r_ret = get_start_node();
		return true;
	} else if (name == "end_node") {
		r_ret = get_end_node();
		return true;
	} else if (name == "graph_offset") {
		r_ret = get_graph_offset();
		return true;
	}

	return false;
}

void AnimationNodePlanner::_get_property_list(List<PropertyInfo> *p_list) const {
	List<StringName> names;
	for (const KeyValue<StringName, State> &E : states) {
		names.push_back(E.key);
	}
	names.sort_custom<StringName::AlphCompare>();

	for (const StringName &name : names) {
		p_list->push_back(PropertyInfo(Variant::OBJECT, "states/" + name + "/node", PROPERTY_HINT_RESOURCE_TYPE, "AnimationNode", PROPERTY_USAGE_NO_EDITOR));
		p_list->push_back(PropertyInfo(Variant::VECTOR2, "states/" + name + "/position", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NO_EDITOR));
	}

	p_list->push_back(PropertyInfo(Variant::ARRAY, "transitions", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NO_EDITOR));
	p_list->push_back(PropertyInfo(Variant::STRING_NAME, "start_node", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NO_EDITOR));
	p_list->push_back(PropertyInfo(Variant::STRING_NAME, "end_node", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NO_EDITOR));
	p_list->push_back(PropertyInfo(Variant::VECTOR2, "graph_offset", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NO_EDITOR));
}

void AnimationNodePlanner::reset_state() {
	states.clear();
	transitions.clear();
	playback = "playback";
	start_node = StringName();
	end_node = StringName();
	graph_offset = Vector2();

	emit_changed();
	emit_signal(SNAME("tree_changed"));
}

void AnimationNodePlanner::set_node_position(const StringName &p_name, const Vector2 &p_position) {
	ERR_FAIL_COND(!states.has(p_name));
	states[p_name].position = p_position;
}

Vector2 AnimationNodePlanner::get_node_position(const StringName &p_name) const {
	ERR_FAIL_COND_V(!states.has(p_name), Vector2());
	return states[p_name].position;
}

void AnimationNodePlanner::_tree_changed() {
	emit_signal(SNAME("tree_changed"));
}

void AnimationNodePlanner::_bind_methods() {
	ClassDB::bind_method(D_METHOD("add_node", "name", "node", "position"), &AnimationNodePlanner::add_node, DEFVAL(Vector2()));
	ClassDB::bind_method(D_METHOD("replace_node", "name", "node"), &AnimationNodePlanner::replace_node);
	ClassDB::bind_method(D_METHOD("get_node", "name"), &AnimationNodePlanner::get_node);
	ClassDB::bind_method(D_METHOD("remove_node", "name"), &AnimationNodePlanner::remove_node);
	ClassDB::bind_method(D_METHOD("rename_node", "name", "new_name"), &AnimationNodePlanner::rename_node);
	ClassDB::bind_method(D_METHOD("has_node", "name"), &AnimationNodePlanner::has_node);
	ClassDB::bind_method(D_METHOD("get_node_name", "node"), &AnimationNodePlanner::get_node_name);

	ClassDB::bind_method(D_METHOD("set_node_position", "name", "position"), &AnimationNodePlanner::set_node_position);
	ClassDB::bind_method(D_METHOD("get_node_position", "name"), &AnimationNodePlanner::get_node_position);

	ClassDB::bind_method(D_METHOD("has_transition", "from", "to"), &AnimationNodePlanner::has_transition);
	ClassDB::bind_method(D_METHOD("add_transition", "from", "to", "transition"), &AnimationNodePlanner::add_transition);
	ClassDB::bind_method(D_METHOD("get_transition", "idx"), &AnimationNodePlanner::get_transition);
	ClassDB::bind_method(D_METHOD("get_transition_from", "idx"), &AnimationNodePlanner::get_transition_from);
	ClassDB::bind_method(D_METHOD("get_transition_to", "idx"), &AnimationNodePlanner::get_transition_to);
	ClassDB::bind_method(D_METHOD("get_transition_count"), &AnimationNodePlanner::get_transition_count);
	ClassDB::bind_method(D_METHOD("remove_transition_by_index", "idx"), &AnimationNodePlanner::remove_transition_by_index);
	ClassDB::bind_method(D_METHOD("remove_transition", "from", "to"), &AnimationNodePlanner::remove_transition);

	ClassDB::bind_method(D_METHOD("set_start_node", "name"), &AnimationNodePlanner::set_start_node);
	ClassDB::bind_method(D_METHOD("get_start_node"), &AnimationNodePlanner::get_start_node);

	ClassDB::bind_method(D_METHOD("set_end_node", "name"), &AnimationNodePlanner::set_end_node);
	ClassDB::bind_method(D_METHOD("get_end_node"), &AnimationNodePlanner::get_end_node);

	ClassDB::bind_method(D_METHOD("set_graph_offset", "offset"), &AnimationNodePlanner::set_graph_offset);
	ClassDB::bind_method(D_METHOD("get_graph_offset"), &AnimationNodePlanner::get_graph_offset);
}

AnimationNodePlanner::AnimationNodePlanner() {
}
