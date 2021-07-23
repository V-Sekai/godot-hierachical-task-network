/*************************************************************************/
/*  test_tfd_room.h                                                      */
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

#ifndef TEST_TFD_PLAN_ROOM_H
#define TEST_TFD_PLAN_ROOM_H

#include "tests/test_macros.h"
#include <any>
#include <functional>
#include <optional>

#include <modules/tfd/tests/tfd_common.h>

#include <modules/tfd/planning_problem.h>
#include <modules/tfd/tfd.h>
#include <cassert>
#include <sstream>

// class Bottle : public Item {
// 	std::string _description =
// 			"In front of you there is a little bottle on a table. Around the neck of the bottle there is a paper label, with "s
// 			"the words 'DRINK ME' beautifully printed on it in large letters."s;

// public:
// 	virtual const std::string &Description() override { return _description; }

// 	Bottle(std::function<SharedPtr<GameScreen>(AIContext &, GoalState)> SetGoal) :
// 			Item(DefineDomain()) {
// 		_KeywordsToAction = {
// 			{ { "get", "bottle" },
// 					[=](AIContext &ctx) -> SharedPtr<GameScreen> {
// 						if (SetGoal != nullptr) {
// 							return SetGoal(ctx, GoalState::GetBottle);
// 						}
// 						return nullptr;
// 					}

// 			},
// 			{ { "open", "bottle" },
// 					[=](AIContext &ctx) -> SharedPtr<GameScreen> {
// 						if (SetGoal != nullptr) {
// 							return SetGoal(ctx, GoalState::OpenBottle);
// 						}
// 						return nullptr;
// 					} },
// 			{ { "drink", "bottle" },
// 					[=](AIContext &ctx) -> SharedPtr<GameScreen> {
// 						if (SetGoal != nullptr) {
// 							return SetGoal(ctx, GoalState::DrinkBottle);
// 						}
// 						return nullptr;
// 					} },
// 			{ { "break", "bottle" },
// 					[=](AIContext &ctx) -> SharedPtr<GameScreen> {
// 						if (SetGoal != nullptr) {
// 							return SetGoal(ctx, GoalState::BreakBottle);
// 						}
// 						return nullptr;
// 					} },
// 			{ { "throw", "bottle" },
// 					[=](AIContext &ctx) -> SharedPtr<GameScreen> {
// 						if (SetGoal != nullptr) {
// 							return SetGoal(ctx, GoalState::ThrowBottle);
// 						}
// 						return nullptr;
// 					} },
// 			{ { "slash", "bottle" },
// 					[=](AIContext &ctx) -> SharedPtr<GameScreen> {
// 						if (SetGoal != nullptr) {
// 							return SetGoal(ctx, GoalState::SlashBottle);
// 						}
// 						return nullptr;
// 					} },
// 			{ { "drop", "bottle" },
// 					[=](AIContext &ctx) -> SharedPtr<GameScreen> {
// 						if (SetGoal != nullptr) {
// 							return SetGoal(ctx, GoalState::DropBottle);
// 						}
// 						return nullptr;
// 					} },
// 		};
// 	}

// private:
// 	static SharedPtr<FluidHTN::Domain> DefineDomain();
// };

// SharedPtr<FluidHTN::Domain> Bottle::DefineDomain() {
// 	SharedPtr<FluidHTN::Domain> getBottleDomain;
// 	SharedPtr<FluidHTN::Domain> dropBottleDomain;
// 	SharedPtr<FluidHTN::Domain> openBottleActionDomain;
// 	SharedPtr<FluidHTN::Domain> openBottleDomain;
// 	SharedPtr<FluidHTN::Domain> drinkBottleActionDomain;
// 	SharedPtr<FluidHTN::Domain> drinkBottleDomain;
// 	SharedPtr<FluidHTN::Domain> throwBottleDomain;
// 	SharedPtr<FluidHTN::Domain> cutBottleDomain;
// 	SharedPtr<FluidHTN::Domain> breakBottleDomain;
// 	{
// 		BaseDomainBuilder getBottleDomainBuilder("Get Bottle Sub-domain");
// 		getBottleDomainBuilder.AddSelector("Get Bottle");
// 		getBottleDomainBuilder.AddCondition("GOAL: Get Bottle", [](IContext &ctx) {
// 			return static_cast<AIContext &>(ctx).HasGoal(GoalState::GetBottle);
// 		});
// 		getBottleDomainBuilder.AddAction("Get Bottle");
// 		getBottleDomainBuilder.AddCondition("Has NOT Bottle", [](IContext &ctx) {
// 			return static_cast<AIContext &>(ctx).HasStateAIWS(AIWorldState::HasBottle, false);
// 		});
// 		getBottleDomainBuilder.AddOperator([](IContext &ctx) { return Actions::GetBottle(static_cast<AIContext &>(ctx)); });
// 		getBottleDomainBuilder.AddEffect("Has Bottle", EffectType::PlanAndExecute, [](IContext &ctx, EffectType type) {
// 			static_cast<AIContext &>(ctx).SetStateAIWS(AIWorldState::HasBottle, true, type);
// 		});
// 		getBottleDomainBuilder.AddEffect("Try Complete Goal", EffectType::PlanAndExecute, [](IContext &ctx, EffectType type) {
// 			Actions::TryCompleteGoal(static_cast<AIContext &>(ctx), GoalState::GetBottle, type);
// 		});
// 		getBottleDomainBuilder.End();
// 		getBottleDomainBuilder.End();
// 		getBottleDomain = getBottleDomainBuilder.Build();
// 	}

// 	{
// 		BaseDomainBuilder dropBottleDomainBuilder("Drop Bottle Sub-domain");
// 		dropBottleDomainBuilder.AddSelector("Drop Bottle");
// 		dropBottleDomainBuilder.AddCondition("GOAL: Drop Bottle", [](IContext &ctx) {
// 			return static_cast<AIContext &>(ctx).HasGoal(GoalState::DropBottle);
// 		});
// 		dropBottleDomainBuilder.AddAction("Drop Bottle");
// 		dropBottleDomainBuilder.AddCondition("Has Bottle", [](IContext &ctx) {
// 			return static_cast<AIContext &>(ctx).HasStateAIWS(AIWorldState::HasBottle, true);
// 		});
// 		dropBottleDomainBuilder.AddOperator([](IContext &ctx) { return Actions::DropBottle(static_cast<AIContext &>(ctx)); });
// 		dropBottleDomainBuilder.AddEffect("Has NOT Bottle", EffectType::PlanAndExecute, [](IContext &ctx, EffectType type) {
// 			static_cast<AIContext &>(ctx).SetStateAIWS(AIWorldState::HasBottle, true, type);
// 		});
// 		dropBottleDomainBuilder.AddEffect("Try Complete Goal", EffectType::PlanAndExecute, [](IContext &ctx, EffectType type) {
// 			Actions::TryCompleteGoal(static_cast<AIContext &>(ctx), GoalState::DropBottle, type);
// 		});
// 		dropBottleDomainBuilder.End();
// 		dropBottleDomainBuilder.End();
// 		dropBottleDomain = dropBottleDomainBuilder.Build();
// 	}

// 	{
// 		BaseDomainBuilder openBottleActionDomainBuilder("Open Bottle Action Sub-domain");
// 		openBottleActionDomainBuilder.AddAction("Open Bottle");
// 		openBottleActionDomainBuilder.AddCondition("Has Bottle", [](IContext &ctx) {
// 			return static_cast<AIContext &>(ctx).HasStateAIWS(AIWorldState::HasBottle, true);
// 		});
// 		openBottleActionDomainBuilder.AddCondition("Bottle is NOT Open", [](IContext &ctx) {
// 			return static_cast<AIContext &>(ctx).HasStateAIWS(AIWorldState::BottleIsOpen, false);
// 		});
// 		openBottleActionDomainBuilder.AddOperator([](IContext &ctx) { return Actions::OpenBottle(static_cast<AIContext &>(ctx)); });
// 		openBottleActionDomainBuilder.AddEffect("Open Bottle", EffectType::PlanAndExecute, [](IContext &ctx, EffectType type) {
// 			static_cast<AIContext &>(ctx).SetStateAIWS(AIWorldState::BottleIsOpen, true, type);
// 		});
// 		openBottleActionDomainBuilder.AddEffect(
// 				"Try Complete Goal",
// 				EffectType::PlanAndExecute,
// 				[](IContext &ctx, EffectType type) {
// 					Actions::TryCompleteGoal(static_cast<AIContext &>(ctx), GoalState::OpenBottle, type);
// 				});
// 		openBottleActionDomainBuilder.End();
// 		openBottleActionDomain = openBottleActionDomainBuilder.Build();
// 	}
// 	{
// 		BaseDomainBuilder openBottleDomainBuilder("Open Bottle Sub-domain");
// 		openBottleDomainBuilder.AddSelector("Open Bottle");
// 		openBottleDomainBuilder.AddCondition("GOAL: Open Bottle", [](IContext &ctx) {
// 			return static_cast<AIContext &>(ctx).HasGoal(GoalState::OpenBottle);
// 		});
// 		openBottleDomainBuilder.AddSelector("Already Has Bottle?");
// 		openBottleDomainBuilder.Splice(*openBottleActionDomain);
// 		openBottleDomainBuilder.AddSequence("Get bottle and open it");
// 		openBottleDomainBuilder.AddAction("Temporary change goal");
// 		openBottleDomainBuilder.AddEffect("Get Bottle Goal", EffectType::PlanOnly, [](IContext &ctx, EffectType type) {
// 			Actions::ChangeGoal(static_cast<AIContext &>(ctx), type, GoalState::GetBottle);
// 		});
// 		openBottleDomainBuilder.End();
// 		openBottleDomainBuilder.Splice(*getBottleDomain);
// 		openBottleDomainBuilder.AddAction("Temporary change goal");
// 		openBottleDomainBuilder.AddEffect("Open Bottle Goal", EffectType::PlanOnly, [](IContext &ctx, EffectType type) {
// 			Actions::ChangeGoal(static_cast<AIContext &>(ctx), type, GoalState::OpenBottle);
// 		});
// 		openBottleDomainBuilder.End();
// 		openBottleDomainBuilder.Splice(*openBottleActionDomain);
// 		openBottleDomainBuilder.End();
// 		openBottleDomainBuilder.End();
// 		openBottleDomainBuilder.End();
// 		openBottleDomain = openBottleDomainBuilder.Build();
// 	}
// 	{
// 		BaseDomainBuilder drinkBottleActionDomainBuilder("Drink Bottle Action Sub-domain");
// 		drinkBottleActionDomainBuilder.AddAction("Drink Bottle");
// 		drinkBottleActionDomainBuilder.AddCondition("Has Bottle", [](IContext &ctx) {
// 			return static_cast<AIContext &>(ctx).HasStateAIWS(AIWorldState::HasBottle, true);
// 		});
// 		drinkBottleActionDomainBuilder.AddCondition("Bottle is Open", [](IContext &ctx) {
// 			return static_cast<AIContext &>(ctx).HasStateAIWS(AIWorldState::BottleIsOpen, true);
// 		});
// 		drinkBottleActionDomainBuilder.AddCondition("Bottle is NOT Empty", [](IContext &ctx) {
// 			return static_cast<AIContext &>(ctx).HasStateAIWS(AIWorldState::BottleIsEmpty, false);
// 		});
// 		drinkBottleActionDomainBuilder.AddOperator([](IContext &ctx) { return Actions::SipBottle(static_cast<AIContext &>(ctx)); });
// 		drinkBottleActionDomainBuilder.AddEffect("Drink Bottle", EffectType::PlanAndExecute, [](IContext &ctx, EffectType type) {
// 			static_cast<AIContext &>(ctx).SetStateAIWS(AIWorldState::BottleIsEmpty, true, type);
// 		});
// 		drinkBottleActionDomainBuilder.AddEffect(
// 				"Try Complete Goal",
// 				EffectType::PlanAndExecute,
// 				[](IContext &ctx, EffectType type) {
// 					Actions::TryCompleteGoal(static_cast<AIContext &>(ctx), GoalState::DrinkBottle, type);
// 				});
// 		drinkBottleActionDomainBuilder.End();
// 		drinkBottleActionDomain = drinkBottleActionDomainBuilder.Build();
// 	}

// 	{
// 		BaseDomainBuilder drinkBottleDomainBuilder("Drink Bottle Sub-domain");
// 		drinkBottleDomainBuilder.AddSelector("Drink Bottle");
// 		drinkBottleDomainBuilder.AddCondition("GOAL: Drink Bottle", [](IContext &ctx) {
// 			return static_cast<AIContext &>(ctx).HasGoal(GoalState::DrinkBottle);
// 		});
// 		drinkBottleDomainBuilder.AddSelector("Already Has Opened Bottle?");
// 		drinkBottleDomainBuilder.Splice(*drinkBottleActionDomain);
// 		drinkBottleDomainBuilder.AddSequence("Get bottle, open it, then drink it!");
// 		drinkBottleDomainBuilder.AddAction("Temporary change goal");
// 		drinkBottleDomainBuilder.AddEffect("Open Bottle Goal", EffectType::PlanOnly, [](IContext &ctx, EffectType type) {
// 			Actions::ChangeGoal(static_cast<AIContext &>(ctx), type, GoalState::OpenBottle);
// 		});
// 		drinkBottleDomainBuilder.End();
// 		drinkBottleDomainBuilder.Splice(*openBottleDomain);
// 		drinkBottleDomainBuilder.AddAction("Temporary change goal");
// 		drinkBottleDomainBuilder.AddEffect("Drink Bottle Goal", EffectType::PlanOnly, [](IContext &ctx, EffectType type) {
// 			Actions::ChangeGoal(static_cast<AIContext &>(ctx), type, GoalState::DrinkBottle);
// 		});
// 		drinkBottleDomainBuilder.End();
// 		drinkBottleDomainBuilder.Splice(*drinkBottleActionDomain);
// 		drinkBottleDomainBuilder.End();
// 		drinkBottleDomainBuilder.End();
// 		drinkBottleDomainBuilder.End();
// 		drinkBottleDomain = drinkBottleDomainBuilder.Build();
// 	}

// 	{
// 		BaseDomainBuilder throwBottleDomainBuilder("Throw Bottle Sub-domain");
// 		throwBottleDomainBuilder.AddAction("Throw the bottle");
// 		throwBottleDomainBuilder.AddCondition("Has Bottle", [](IContext &ctx) {
// 			return static_cast<AIContext &>(ctx).HasStateAIWS(AIWorldState::HasBottle, true);
// 		});
// 		throwBottleDomainBuilder.AddOperator([](IContext &ctx) { return Actions::ThrowFullBottle(static_cast<AIContext &>(ctx)); });
// 		throwBottleDomainBuilder.AddEffect("Break Bottle", EffectType::PlanAndExecute, [](IContext &ctx, EffectType type) {
// 			static_cast<AIContext &>(ctx).SetStateAIWS(AIWorldState::BottleIsBroken, true, type);
// 		});
// 		throwBottleDomainBuilder.AddEffect("Complete Goal", EffectType::PlanAndExecute, [](IContext &ctx, EffectType type) {
// 			static_cast<AIContext &>(ctx).SetGoal(GoalState::None, true, type);
// 		});
// 		throwBottleDomainBuilder.End();
// 		throwBottleDomain = throwBottleDomainBuilder.Build();
// 	}
// 	{
// 		BaseDomainBuilder cutBottleDomainBuilder("Cut Bottle Sub-domain");
// 		cutBottleDomainBuilder.AddAction("Cut the bottle?");
// 		cutBottleDomainBuilder.AddCondition("Has NOT Bottle", [](IContext &ctx) {
// 			return static_cast<AIContext &>(ctx).HasStateAIWS(AIWorldState::HasBottle, false);
// 		});
// 		cutBottleDomainBuilder.AddCondition("Has Weapon", [](IContext &ctx) {
// 			return static_cast<AIContext &>(ctx).HasStateAIWS(AIWorldState::HasWeapon, true);
// 		});
// 		cutBottleDomainBuilder.AddOperator([](IContext &ctx) { return Actions::SlashBottle(static_cast<AIContext &>(ctx)); });
// 		cutBottleDomainBuilder.AddEffect("Break Bottle", EffectType::PlanAndExecute, [](IContext &ctx, EffectType type) {
// 			static_cast<AIContext &>(ctx).SetStateAIWS(AIWorldState::BottleIsBroken, true, type);
// 		});
// 		cutBottleDomainBuilder.AddEffect("Complete Goal", EffectType::PlanAndExecute, [](IContext &ctx, EffectType type) {
// 			static_cast<AIContext &>(ctx).SetGoal(GoalState::None, true, type);
// 		});
// 		cutBottleDomainBuilder.End();
// 		cutBottleDomain = cutBottleDomainBuilder.Build();
// 	}
// 	// clang-format off
//     {
//         BaseDomainBuilder breakBottleDomainBuilder("Break Bottle Sub-domain");
//         breakBottleDomainBuilder.AddSelector("Break Bottle");
// 			breakBottleDomainBuilder.AddCondition("GOAL: Break Bottle", [](IContext& ctx) {
// 				return static_cast<AIContext&>(ctx).HasGoal(GoalState::BreakBottle);
// 			});
// 			breakBottleDomainBuilder.AddCondition("Bottle NOT already broken", [](IContext& ctx) {
// 				return static_cast<AIContext&>(ctx).HasStateAIWS(AIWorldState::BottleIsBroken, false);
// 			});
// 			breakBottleDomainBuilder.Splice(*throwBottleDomain);
// 			breakBottleDomainBuilder.Splice(*cutBottleDomain);
// 			breakBottleDomainBuilder.AddRandomSelector("Select random");
// 				breakBottleDomainBuilder.AddSequence("Get bottle and throw it");
// 					breakBottleDomainBuilder.AddAction("Temporary change goal");
// 						breakBottleDomainBuilder.AddEffect("Get Bottle Goal", EffectType::PlanOnly, [](IContext& ctx, EffectType type) {
// 							Actions::ChangeGoal(static_cast<AIContext&>(ctx), type, GoalState::GetBottle);
// 						});
// 						breakBottleDomainBuilder.End();
// 						breakBottleDomainBuilder.Splice(*getBottleDomain);
// 						breakBottleDomainBuilder.AddAction("Temporary change goal");
// 							breakBottleDomainBuilder.AddEffect("Break Bottle Goal", EffectType::PlanOnly, [](IContext& ctx, EffectType type) {
// 								Actions::ChangeGoal(static_cast<AIContext&>(ctx), type, GoalState::BreakBottle);
// 							});
// 						breakBottleDomainBuilder.End();
// 						breakBottleDomainBuilder.Splice(*throwBottleDomain);
// 				breakBottleDomainBuilder.End();
// 				breakBottleDomainBuilder.AddSequence("Get sword and cut bottle");
// 					breakBottleDomainBuilder.AddAction("Get Sword");
// 						breakBottleDomainBuilder.AddCondition("Has NOT Weapon", [](IContext& ctx) {
// 							return static_cast<AIContext&>(ctx).HasStateAIWS(AIWorldState::HasWeapon, false);
// 						});
// 						breakBottleDomainBuilder.AddOperator([](IContext& ctx) { return Actions::GetSword(static_cast<AIContext&>(ctx)); });
// 							breakBottleDomainBuilder.AddEffect("Has Weapon", EffectType::PlanAndExecute, [](IContext& ctx, EffectType type) {
// 								static_cast<AIContext&>(ctx).SetStateAIWS(AIWorldState::HasWeapon, true, type);
// 							});
// 						breakBottleDomainBuilder.End();
// 						breakBottleDomainBuilder.Splice(*cutBottleDomain);
// 				breakBottleDomainBuilder.End();
// 			breakBottleDomainBuilder.End(); // end random
//         breakBottleDomainBuilder.End(); //end outer selector
//         breakBottleDomainBuilder.AddSelector("Throw Bottle");
// 			breakBottleDomainBuilder.AddCondition("GOAL: Throw Bottle", [](IContext& ctx) {
// 				return static_cast<AIContext&>(ctx).HasGoal(GoalState::ThrowBottle);
// 			});
// 			breakBottleDomainBuilder.AddCondition("Bottle NOT already broken", [](IContext& ctx) {
// 				return static_cast<AIContext&>(ctx).HasStateAIWS(AIWorldState::BottleIsBroken, false);
// 			});
// 			breakBottleDomainBuilder.Splice(*throwBottleDomain);
// 			breakBottleDomainBuilder.AddSequence("Get bottle and throw it");
// 				breakBottleDomainBuilder.AddAction("Temporary change goal");
// 					breakBottleDomainBuilder.AddEffect("Get Bottle Goal", EffectType::PlanOnly, [](IContext& ctx, EffectType type) {
// 							Actions::ChangeGoal(static_cast<AIContext&>(ctx), type, GoalState::GetBottle);
// 					});
// 				breakBottleDomainBuilder.End();
// 				breakBottleDomainBuilder.Splice(*getBottleDomain);
// 				breakBottleDomainBuilder.AddAction("Temporary change goal");
// 					breakBottleDomainBuilder.AddEffect("Throw Bottle Goal", EffectType::PlanOnly,[](IContext& ctx, EffectType type) {
// 							Actions::ChangeGoal(static_cast<AIContext&>(ctx), type, GoalState::ThrowBottle);
// 					});
// 				breakBottleDomainBuilder.End();
// 				breakBottleDomainBuilder.Splice(*throwBottleDomain);
//             breakBottleDomainBuilder.End();
// 		breakBottleDomainBuilder.End(); // throw bottle selector
// 		breakBottleDomainBuilder.AddSelector("Slash Bottle");
//             breakBottleDomainBuilder.AddCondition("GOAL: Slash Bottle", [](IContext& ctx) {
// 				return static_cast<AIContext&>(ctx).HasGoal(GoalState::SlashBottle);
// 			});
//             breakBottleDomainBuilder.AddCondition("Bottle NOT already broken", [](IContext& ctx) {
// 				return static_cast<AIContext&>(ctx).HasStateAIWS(AIWorldState::BottleIsBroken, false);
// 			});
//             breakBottleDomainBuilder.Splice(*cutBottleDomain);
//             breakBottleDomainBuilder.AddSequence("Get sword and cut bottle");
// 				breakBottleDomainBuilder.AddAction("Get Sword");
// 					breakBottleDomainBuilder.AddCondition("Has NOT Weapon", [](IContext& ctx) {
//                          return static_cast<AIContext&>(ctx).HasStateAIWS(AIWorldState::HasWeapon, false);
// 					 });
// 					breakBottleDomainBuilder.AddOperator([](IContext& ctx) { return Actions::GetSword(static_cast<AIContext&>(ctx)); });
// 					breakBottleDomainBuilder.AddEffect("Has Weapon", EffectType::PlanAndExecute,[](IContext& ctx, EffectType type) {
//                          static_cast<AIContext&>(ctx).SetStateAIWS(AIWorldState::HasWeapon, true, type);
// 					});
// 				breakBottleDomainBuilder.End();
// 				breakBottleDomainBuilder.Splice(*cutBottleDomain);
//             breakBottleDomainBuilder.End();
//             breakBottleDomainBuilder.AddSequence("Set down bottle and slash it");
// 				breakBottleDomainBuilder.AddAction("Drop bottle");
// 					breakBottleDomainBuilder.AddCondition("Has Bottle",[](IContext& ctx) {
// 								 return static_cast<AIContext&>(ctx).HasStateAIWS(AIWorldState::HasBottle, false);
// 					 });
// 					breakBottleDomainBuilder.AddOperator([](IContext& ctx) { return Actions::DropBottle(static_cast<AIContext&>(ctx)); });
// 					breakBottleDomainBuilder.AddEffect("Has NOT Bottle", EffectType::PlanAndExecute, [](IContext& ctx, EffectType type) {
// 						 static_cast<AIContext&>(ctx).SetStateAIWS(AIWorldState::HasBottle, true, type);
// 					});
// 				breakBottleDomainBuilder.End();
// 				breakBottleDomainBuilder.Splice(*cutBottleDomain);
//             breakBottleDomainBuilder.End();
// 		breakBottleDomainBuilder.End();
//         breakBottleDomain = breakBottleDomainBuilder.Build();
//     }
//     {
// 		BaseDomainBuilder bottleDomainBuilder("Bottle sub-domain");
// 		bottleDomainBuilder.AddAction("Bottle is broken");
// 			bottleDomainBuilder.AddCondition("GOAL: * Bottle", [](IContext& ctx) {
// 				return ( static_cast<AIContext&>(ctx).GetGoal() > GoalState::None &&  static_cast<AIContext&>(ctx).GetGoal() <= GoalState::ThrowBottle);
// 			});
// 			bottleDomainBuilder.AddCondition("Bottle is broken",[](IContext& ctx) { 
// 				return  static_cast<AIContext&>(ctx).HasState(AIWorldState::BottleIsBroken);
// 			});
// 			bottleDomainBuilder.AddOperator( [](IContext& ctx) {return Actions::Write(static_cast<AIContext&>(ctx), "But the bottle lies shattered on the ground!");});
// 			bottleDomainBuilder.AddEffect("Complete Goal", EffectType::PlanAndExecute, [](IContext& ctx, EffectType type) {
// 				 static_cast<AIContext&>(ctx).SetGoal(GoalState::None, true, type);
// 			 });
// 		bottleDomainBuilder.End();
// 		bottleDomainBuilder.Splice(*getBottleDomain);
// 		bottleDomainBuilder.Splice(*openBottleDomain);
// 		bottleDomainBuilder.Splice(*drinkBottleDomain);
// 		bottleDomainBuilder.Splice(*breakBottleDomain);
// 		bottleDomainBuilder.Splice(*dropBottleDomain);
// 		bottleDomainBuilder.AddAction("Already has bottle");
// 			bottleDomainBuilder.AddCondition("GOAL: Get Bottle", [](IContext& ctx) { 
// 				 return  static_cast<AIContext&>(ctx).HasGoal(GoalState::GetBottle);
// 			 });
// 			bottleDomainBuilder.AddCondition("Has Bottle", [](IContext& ctx) { 
// 				 return  static_cast<AIContext&>(ctx).HasState(AIWorldState::HasBottle);
// 			});
// 			bottleDomainBuilder.AddOperator( [](IContext& ctx) { return Actions::Write(static_cast<AIContext&>(ctx), "But you already hold the bottle!");});
// 			bottleDomainBuilder.AddEffect("Complete Goal", EffectType::PlanAndExecute,[](IContext& ctx, EffectType type) {
// 				 static_cast<AIContext&>(ctx).SetGoal(GoalState::None, true, type);
// 			});
// 			bottleDomainBuilder.End();
// 		bottleDomainBuilder.AddAction("Already opened bottle");
// 			bottleDomainBuilder.AddCondition("GOAL: Open Bottle", [](IContext& ctx) { 
// 				 return  static_cast<AIContext&>(ctx).HasGoal(GoalState::OpenBottle);
// 			});
// 			bottleDomainBuilder.AddCondition("Bottle is open",[](IContext& ctx) { 
// 				 return  static_cast<AIContext&>(ctx).HasState(AIWorldState::BottleIsOpen);
// 			});
// 			bottleDomainBuilder.AddOperator( [](IContext& ctx) { return Actions::Write(static_cast<AIContext&>(ctx), "But you already opened the bottle!");});
// 			bottleDomainBuilder.AddEffect("Complete Goal", EffectType::PlanAndExecute, [](IContext& ctx, EffectType type) {
// 				 static_cast<AIContext&>(ctx).SetGoal(GoalState::None, true, type);
// 			 });
// 			bottleDomainBuilder.End();
// 		bottleDomainBuilder.AddAction("Already emptied bottle");
// 			bottleDomainBuilder.AddCondition("GOAL: Drink Bottle",[](IContext& ctx) { 
// 				 return  static_cast<AIContext&>(ctx).HasGoal(GoalState::DrinkBottle);
// 			});
// 			bottleDomainBuilder.AddCondition("Bottle is empty",  [](IContext& ctx) { 
// 				 return  static_cast<AIContext&>(ctx).HasState(AIWorldState::BottleIsEmpty);
// 			});
// 			bottleDomainBuilder.AddOperator( [](IContext& ctx) { return Actions::Write(static_cast<AIContext&>(ctx), "But the bottle is empty!");});
// 			bottleDomainBuilder.AddEffect("Complete Goal", EffectType::PlanAndExecute, [](IContext& ctx, EffectType type) {
// 				 static_cast<AIContext&>(ctx).SetGoal(GoalState::None, true, type);
// 			 });
// 			bottleDomainBuilder.End();
// 		bottleDomainBuilder.AddAction("Not holding bottle to drop");
// 			bottleDomainBuilder.AddCondition("GOAL: Drop Bottle",[](IContext& ctx) { 
// 				 return  static_cast<AIContext&>(ctx).HasGoal(GoalState::DropBottle);
// 			});
// 			bottleDomainBuilder.AddCondition("Has NOT Bottle",[](IContext& ctx) { 
// 				 return  static_cast<AIContext&>(ctx).HasState(AIWorldState::HasBottle);
// 			});
// 			bottleDomainBuilder.AddOperator( [](IContext& ctx) { return Actions::Write(static_cast<AIContext&>(ctx), "But you're not holding the bottle!");});
// 			bottleDomainBuilder.AddEffect("Complete Goal", EffectType::PlanAndExecute, [](IContext& ctx, EffectType type) {
// 				 static_cast<AIContext&>(ctx).SetGoal(GoalState::None, true, type);
// 			 });
// 			bottleDomainBuilder.End();
// 		return bottleDomainBuilder.Build();
// 		}
// }

// class Sword
// {
//     std::string _description = "On the ground lies a rusty, old sword.";

// public :
//     virtual const std::string& Description() override { return _description; }

//     Sword(std::function<SharedPtr<GameScreen>(AIContext&, GoalState)> SetGoal) : Item(DefineDomain())
//     {
//         _KeywordsToAction = {
//             {{"get", "sword"},
//              [=](AIContext& ctx) -> SharedPtr<GameScreen> {
//                  if (SetGoal != nullptr)
//                  {
//                      return SetGoal(ctx, GoalState::GetSword);
//                  }
//                  return nullptr;
//              }},
//             {{"slash", "sword"},
//              [=](AIContext& ctx) -> SharedPtr<GameScreen> {
//                  if (SetGoal != nullptr)
//                  {
//                      return SetGoal(ctx, GoalState::SlashAir);
//                  }
//                  return nullptr;
//              }},
//             {{"slash"},
//              [=](AIContext& ctx) -> SharedPtr<GameScreen> {
//                  if (SetGoal != nullptr)
//                  {
//                      return SetGoal(ctx, GoalState::SlashAir);
//                  }
//                  return nullptr;
//              }},
//             {{"drop", "sword"},
//              [=](AIContext& ctx) -> SharedPtr<GameScreen> {
//                  if (SetGoal != nullptr)
//                  {
//                      return SetGoal(ctx, GoalState::DropSword);
//                  }
//                  return nullptr;
//              }},
//         };
//     }

// private:
//     static SharedPtr<FluidHTN::Domain> DefineDomain();
    
// };

// SharedPtr<FluidHTN::Domain> Sword::DefineDomain()
// {

//     SharedPtr<FluidHTN::Domain> getSwordDomain;
//     SharedPtr<FluidHTN::Domain> dropSwordDomain;
//     SharedPtr<FluidHTN::Domain> slashAirActionDomain;
//     // clang-format off
//     {
//     BaseDomainBuilder getSwordDomainBuilder("Get Sword Sub-domain");
//         getSwordDomainBuilder.AddSelector("Get Sword");
// 			getSwordDomainBuilder.AddCondition("GOAL: Get Sword",
// 											   [](IContext& ctx) { return static_cast<AIContext&>(ctx).HasGoal(GoalState::GetSword); });
// 			getSwordDomainBuilder.AddAction("Get Sword");
//                 getSwordDomainBuilder.AddCondition("Has NOT Weapon",[](IContext& ctx) {
//                      return static_cast<AIContext&>(ctx).HasStateAIWS(AIWorldState::HasWeapon,false);
// 				});
// 				getSwordDomainBuilder.AddOperator([](IContext& ctx) { return Actions::GetSword(static_cast<AIContext&>(ctx)); });
//                 getSwordDomainBuilder.AddEffect("Has Weapon", EffectType::PlanAndExecute, [](IContext& ctx, EffectType type) {
//                      static_cast<AIContext&>(ctx).SetStateAIWS(AIWorldState::HasWeapon, true, type);
// 				});
//                 getSwordDomainBuilder.AddEffect("Try Complete Goal",
//                         EffectType::PlanAndExecute,[](IContext& ctx, EffectType type) {
//                          Actions::TryCompleteGoal(static_cast<AIContext&>(ctx), GoalState::GetSword,type);
// 				});
// 			getSwordDomainBuilder.End();
// 		getSwordDomainBuilder.End();

//         getSwordDomain = getSwordDomainBuilder.Build();
//     }
// 	{
//             BaseDomainBuilder dropSwordDomainBuilder("Drop Sword Sub-domain");
// 			  dropSwordDomainBuilder.AddSelector("Drop Sword");
// 				  dropSwordDomainBuilder.AddCondition("GOAL: Drop Sword",  
// 											   [](IContext& ctx) { return static_cast<AIContext&>(ctx).HasGoal(GoalState::DropSword); });
// 				  dropSwordDomainBuilder.AddAction("Drop Sword");
// 					  dropSwordDomainBuilder.AddCondition("Has Weapon",[](IContext& ctx) {
// 						 return static_cast<AIContext&>(ctx).HasState(AIWorldState::HasWeapon);
// 					  });
// 					  dropSwordDomainBuilder.AddOperator([](IContext& ctx) { return Actions::DropSword(static_cast<AIContext&>(ctx)); });
// 					  dropSwordDomainBuilder.AddEffect("Has NOT Weapon",
//                                           EffectType::PlanAndExecute,[](IContext& ctx, EffectType type) {
//                                            static_cast<AIContext&>(ctx).SetStateAIWS(AIWorldState::HasWeapon, false, type);
// 					   });
// 						dropSwordDomainBuilder.AddEffect("Try Complete Goal",
// 								EffectType::PlanAndExecute,[](IContext& ctx, EffectType type) {
// 								 Actions::TryCompleteGoal(static_cast<AIContext&>(ctx), GoalState::DropSword,type);
// 						});
// 				  dropSwordDomainBuilder.End();
// 			  dropSwordDomainBuilder.End();
// 			dropSwordDomain = dropSwordDomainBuilder.Build();
// 	}
//     {
// 	    BaseDomainBuilder slashAirActionDomainBuilder("Slash Air Action Sub-domain");
//         slashAirActionDomainBuilder.AddAction("Slash Air");
// 		   slashAirActionDomainBuilder.AddCondition("GOAL: Slash Air", 
// 						   [](IContext& ctx) { return static_cast<AIContext&>(ctx).HasGoal(GoalState::SlashAir); });
// 			slashAirActionDomainBuilder.AddCondition("Has Weapon",[](IContext& ctx) {
// 				 return static_cast<AIContext&>(ctx).HasState(AIWorldState::HasWeapon);
// 			});
// 			slashAirActionDomainBuilder.AddOperator([](IContext& ctx) { return Actions::Write(static_cast<AIContext&>(ctx), "You slash your sword through the air elegantly!");});
// 			slashAirActionDomainBuilder.AddEffect("Try Complete Goal",
// 					EffectType::PlanAndExecute,[](IContext& ctx, EffectType type) {
// 					 Actions::TryCompleteGoal(static_cast<AIContext&>(ctx), GoalState::SlashAir,type);
// 			 });
// 		slashAirActionDomainBuilder.End();
//         slashAirActionDomain = slashAirActionDomainBuilder.Build();

// 	}
//     {
//         BaseDomainBuilder swordDomainBuilder("Sword sub-domain");
//             swordDomainBuilder.Splice(*getSwordDomain);
//             swordDomainBuilder.Splice(*dropSwordDomain);
//             swordDomainBuilder.AddAction("Already has sword");
// 				swordDomainBuilder.AddCondition("GOAL: Get Sword", 
// 						   [](IContext& ctx) { return static_cast<AIContext&>(ctx).HasGoal(GoalState::GetSword); });
// 				swordDomainBuilder.AddCondition("Has Weapon", [](IContext& ctx) {
// 					 return static_cast<AIContext&>(ctx).HasState(AIWorldState::HasWeapon);
// 				});
// 				swordDomainBuilder.AddOperator([](IContext& ctx) { return Actions::Write(static_cast<AIContext&>(ctx), "But you're already wielding the sword!");});
// 				swordDomainBuilder.AddEffect("Complete Goal", EffectType::PlanAndExecute, [](IContext& ctx, EffectType type) {
// 					 static_cast<AIContext&>(ctx).SetGoal(GoalState::None, true, type);
// 				 });
//             swordDomainBuilder.End();
//             swordDomainBuilder.AddAction("Not holding sword to drop");
// 				swordDomainBuilder.AddCondition("GOAL: Drop Sword",
// 							   [](IContext& ctx) { return static_cast<AIContext&>(ctx).HasGoal(GoalState::DropSword); });
// 				swordDomainBuilder.AddCondition("Has NOT Weapon", [](IContext& ctx) {
// 						 return static_cast<AIContext&>(ctx).HasStateAIWS(AIWorldState::HasWeapon,false);
// 				});
// 				swordDomainBuilder.AddOperator([](IContext& ctx) { return Actions::Write(static_cast<AIContext&>(ctx), "But you're not holding a sword!");});
// 				swordDomainBuilder.AddEffect("Complete Goal", EffectType::PlanAndExecute, [](IContext& ctx, EffectType type) {
// 					 static_cast<AIContext&>(ctx).SetGoal(GoalState::None, true, type);
// 				 });
//             swordDomainBuilder.End();
//             swordDomainBuilder.AddSelector("Slash");
// 				swordDomainBuilder.AddCondition("GOAL: Slash Air", 
// 							   [](IContext& ctx) { return static_cast<AIContext&>(ctx).HasGoal(GoalState::SlashAir); });
// 				swordDomainBuilder.Splice(*slashAirActionDomain);
// 				swordDomainBuilder.AddSequence("Pick up sword, then slash with it");
// 				swordDomainBuilder.AddAction("Temporary change goal");
// 					swordDomainBuilder.AddEffect("Get Sword Goal", EffectType::PlanOnly, [](IContext& ctx, EffectType type) {
// 						 Actions::ChangeGoal(static_cast<AIContext&>(ctx), type, GoalState::GetSword);
// 					});
// 				swordDomainBuilder.End();
// 				swordDomainBuilder.Splice(*getSwordDomain);
// 				swordDomainBuilder.AddAction("Temporary change goal");
// 						swordDomainBuilder.AddEffect("Slash Air Goal", EffectType::PlanOnly,  [](IContext& ctx, EffectType type) {
// 						 Actions::ChangeGoal(static_cast<AIContext&>(ctx), type, GoalState::GetSword);
// 						});
// 				swordDomainBuilder.End();
// 				swordDomainBuilder.Splice(*slashAirActionDomain);
//             swordDomainBuilder.End();
// 		swordDomainBuilder.End();

// 		return swordDomainBuilder.Build();
// 	}
// }

// std::optional<State> plan_operator(const State &state, const Parameters &parameters) {
// 	State newState(state);
// 	bool status = !std::any_cast<bool>(state.data);
// 	newState.data = status;

// 	return newState;
// }

// std::optional<std::vector<Task>> plan_method(const State &state, const Parameters &parameters) {
// 	Task task;
// 	task.task_name = "test_operator";
// 	task.parameters.push_back(true);
// 	std::vector<Task> subtasks{ task };

// 	return subtasks;
// }

// IntroScreen::IntroScreen()
// {
// 	_Items =
// 	{
//         MakeSharedPtr<Bottle>([this](AIContext& ctx, GoalState gs) { return this->SetGoal(ctx, gs); }),
// 		MakeSharedPtr<Sword>(([this](AIContext& ctx, GoalState gs) { return this->SetGoal(ctx, gs); })),
// 	};
// SharedPtr<Domain> IntroScreen::DefineDomain(std::vector<SharedPtr<Item>>& itemsInScreen)
// {
//     BaseDomainBuilder itemDomainBuilder("Item Sub-domains");
//     for (auto item : itemsInScreen)
//     {
//         itemDomainBuilder.Splice(*(item->Domain()));
//     }

//     BaseDomainBuilder builder("Intro Screen Domain");
//     builder.Splice(*(itemDomainBuilder.Build()));
//     builder.AddAction("Failed");
//     builder.AddCondition("Failed to address goal",
//                          [](IContext& ctx) { return (static_cast<AIContext&>(ctx).HasGoal(GoalState::None) == false); });
//     builder.AddOperator([](IContext& ctx) { return Actions::Failed(static_cast<AIContext&>(ctx)); });
//     builder.AddEffect("Complete Goal", EffectType::PlanAndExecute, [](IContext& ctx, EffectType type) {
//         static_cast<AIContext&>(ctx).SetGoal(GoalState::None, true, type);
//     });
//     builder.End();
//     builder.AddAction("Idle");
//     builder.AddOperator([](IContext&) { return TaskStatus::Continue; });
//     builder.End();
//     return builder.Build();
// }

// enum class AIWorldState
// {
// 	HasBottle,
// 	BottleIsOpen,
// 	BottleIsEmpty,
// 	BottleIsBroken,
// 	HasWeapon,
// 	Goal
// };

// enum class GoalState
// {
// 	None,
// 	GetBottle,
// 	DropBottle,
// 	OpenBottle,
// 	CloseBottle,
// 	DrinkBottle,
// 	BreakBottle,
// 	SlashBottle,
// 	ThrowBottle,
// 	GetSword,
// 	DropSword,
// 	SlashAir,
// };

// static TaskStatus GetBottle(AIContext& context)
// {
// 	context.CurrentScreen()->Write("You pick up the bottle", false);
// 	return TaskStatus::Success;
// }
// static TaskStatus DropBottle(AIContext& context)
// {
// 	context.CurrentScreen()->Write("You drop the bottle", false);
// 	return TaskStatus::Success;
// }

// static TaskStatus OpenBottle(AIContext& context)
// {
// 	context.CurrentScreen()->Write("You unscrew the cork of the bottle", false);
// 	return TaskStatus::Success;
// }

// static TaskStatus CloseBottle(AIContext& context)
// {
// 	context.CurrentScreen()->Write("You screw the cork back onto the bottle", false);
// 	return TaskStatus::Success;
// }

// static TaskStatus SipBottle(AIContext& context)
// {
// 	context.CurrentScreen()->Write("You take a sip from the bottle", false);
// 	context.CurrentScreen()->Write("As the liquid runs down your throat and fills your belly, something weird starts happening. Your vision blur!");


// 	print_line("The room seems to change around you.");
// 	print_line("The walls disappear.");
// 	print_line("The ceiling disappears.");
// 	print_line("The floor disappears!");
// 	print_line("You fall into the darkness...");

// 	return TaskStatus::Success;
// }

// static TaskStatus ThrowFullBottle(AIContext& context)
// {
// 	context.CurrentScreen()->Write("You throw the bottle at the wall, it shatters!", false);
// 	context.CurrentScreen()->Write("The liquid splashes all over the place, even on you!");
// 	print_line("The room seems to change around you.");
// 	print_line("The walls disappear.");
// 	print_line("The ceiling disappears.");
// 	print_line("The floor disappears!");
// 	print_line("You fall into the darkness...");
// 	return TaskStatus::Success;
// }

// static TaskStatus SlashBottle(AIContext& context)
// {
// 	context.CurrentScreen()->Write("You swing your sword furiously at the bottle, it shatters!", false);
// 	context.CurrentScreen()->Write("The liquid splashes all over the place, even on you!");
// 	print_line("The room seems to change around you.");
// 	print_line("The walls disappear.");
// 	print_line("The ceiling disappears.");
// 	print_line("The floor disappears!");
// 	print_line("You fall into the darkness...");
// 	return TaskStatus::Success;
// }

// static TaskStatus SipEmptyBottle(AIContext& context)
// {
// 	context.CurrentScreen()->Write("The bottle is empty", false);
// 	return TaskStatus::Success;
// }

// static TaskStatus GetSword(AIContext& context)
// {
// 	context.CurrentScreen()->Write("You pick up the sword", false);
// 	return TaskStatus::Success;
// }

// static TaskStatus DropSword(AIContext& context)
// {
// 	context.CurrentScreen()->Write("You drop the sword", false);
// 	return TaskStatus::Success;
// }

// static TaskStatus Write(AIContext& context, const std::string& text)
// {
// 	context.CurrentScreen()->Write(text, false);
// 	return TaskStatus::Success;
// }

// static TaskStatus Failed(AIContext& context)
// {
// 	context.CurrentScreen()->Write("Sorry, I don't know how to do that yet!", false);
// 	return TaskStatus::Success;
// }

// static void TryCompleteGoal(AIContext& context, GoalState endOfGoal, EffectType type)
// {
// 	if (context.HasGoal(endOfGoal))
// 	{
// 		context.SetGoal(GoalState::None, true, type);
// 	}
// }

// static void ChangeGoal(AIContext& context, EffectType type, GoalState goal)
// {
// 	context.SetGoal(goal, true, type);
// }

// class GameScreen: public std::enable_shared_from_this<GameScreen>
// {
// protected:
// 	SharedPtr<class Domain> _Domain;
// 	std::vector<SharedPtr<class Item>> _Items;
// 	std::vector<KeywordsToActionEntry> KeywordsToAction;

// 	bool _FirstRun = true;

// 	virtual SharedPtr<GameScreen> OnRun(AIContext& context) = 0;
// public:
// 	SharedPtr<Domain> GetDomain() { return _Domain; }
// 	std::vector<SharedPtr<Item>>& Items() { return _Items; }
// 	bool FirstRun() { return _FirstRun; }


// 	void Initialize(SharedPtr<Domain> domain)
// 	{
// 		_Domain = domain;
// 	}


// 	SharedPtr<GameScreen> Run(AIContext& context)
// 	{
// 		auto result = OnRun(context);
// 		if (_FirstRun) _FirstRun = false;
// 		return result;
// 	}

// 	void Write(std::string text, bool readLineAtEnd = true)
// 	{
// 		print_line(text.c_str());
// 		if (readLineAtEnd)
// 		{
// 			print_line("[Enter]");
// 		}
// 	}

// 	SharedPtr<GameScreen> PerformAction(AIContext context, std::string& action);
	

// private:
// 	std::vector<std::string> SpliceKnownCommands(std::vector<std::string>& parameters)
// 	{
// 		std::vector<std::string> result;

// 		// corner case of one-word commands.
// 		if(parameters.size() == 1)
//         {
//             result = parameters;
//         }
// 		for (int i = 0; i < parameters.size() - 1; i++)
// 		{
// 			if (parameters[i] == "pick" && parameters[i + 1] == "up")
// 			{
// 				result.push_back("pick up"s);
// 				i++;
// 			}
// 			else
// 			{
// 				result.push_back(parameters[i]);
// 			}
// 		}
// 		parameters = result;
// 		return parameters;
// 	}

// 	ScreenFuncType FindBestMatch(std::vector<std::string>& parameters);
	
// 	bool IsEqual(std::string& a, std::string& b)
// 	{
// 		if (a == b) return true;

// 		if (a == "get")
// 		{
// 			if (b == "pick up") return true;
// 			if (b == "fetch") return true;
// 			if (b == "hold") return true;
// 			if (b == "wield") return true;
// 			if (b == "gather") return true;
// 			if (b == "acquire") return true;
// 			if (b == "take") return true;
// 		}

// 		if (a == "slash")
// 		{
// 			if (b == "swing") return true;
// 			if (b == "cut") return true;
// 			if (b == "attack") return true;
// 			if (b == "hit") return true;
// 			if (b == "hack") return true;
// 			if (b == "slice") return true;
// 			if (b == "slit") return true;
// 		}

// 		if (a == "drink")
// 		{
// 			if (b == "sip") return true;
// 			if (b == "gulp") return true;
// 			if (b == "swallow") return true;
// 			if (b == "quaff") return true;
// 			if (b == "taste") return true;
// 			if (b == "consume") return true;
// 			if (b == "drain") return true;
// 			if (b == "slurp") return true;
// 			if (b == "down") return true;
// 		}

// 		if (a == "open")
// 		{
// 			if (b == "unscrew") return true;
// 			if (b == "uncork") return true;
// 		}

// 		if (a == "throw")
// 		{
// 			if (b == "whirl") return true;
// 			if (b == "fling") return true;
// 			if (b == "hurl") return true;
// 			if (b == "lob") return true;
// 			if (b == "thrust") return true;
// 		}

// 		if (a == "break")
// 		{
// 			if (b == "destroy") return true;
// 			if (b == "shatter") return true;
// 			if (b == "crack") return true;
// 			if (b == "crush") return true;
// 			if (b == "demolish") return true;
// 			if (b == "fracture") return true;
// 			if (b == "ruin") return true;
// 			if (b == "smash") return true;
// 			if (b == "wreck") return true;
// 		}

// 		return false;
// 	}
// };

// SharedPtr<GameScreen> GameScreen::PerformAction(AIContext context, std::string& action)
// {
// 	action = StringToLower(action);
// 	std::vector<std::string> parameters;
// 	StringSplit(action, parameters, ' ');
// 	parameters = SpliceKnownCommands(parameters);
// 	auto func = FindBestMatch(parameters);
// 	SharedPtr<GameScreen> result;
// 	if (func)
// 	{
// 		result = func(context);

// 	}
// 	if (result == nullptr)
// 	{
// 		Actions::Failed(context);
// 		return context.CurrentScreen();
// 	}

// 	return result;
// }

// ScreenFuncType GameScreen::FindBestMatch(std::vector<std::string>& parameters)
// {

// 	ScreenFuncType bestMatch;
// 	int bestScore = 0;
// 	for (auto kvp : KeywordsToAction)
// 	{
// 		auto KvpKeys = std::get<0>(kvp);
// 		if (!IsEqual(KvpKeys[0], parameters[0]))
// 		{
// 			continue;
// 		}

// 		auto score = 0;
// 		for (auto key : KvpKeys)
// 		{
// 			for (auto parameter : parameters)
// 			{
// 				if (IsEqual(key, parameter))
// 				{
// 					score++;
// 				}
// 			}
// 		}

// 		if (KvpKeys.size() == parameters.size())
// 		{
// 			score *= 10;
// 		}

// 		if (score > bestScore)
// 		{
// 			bestScore = score;
// 			bestMatch = std::get<1>(kvp);
// 		}
// 	}

// 	for (auto item : _Items)
// 	{
// 		for(auto kvp : item->KeywordsToAction())
// 		{
// 			auto KvpKeys = std::get<0>(kvp);
// 			if (!IsEqual(KvpKeys[0], parameters[0]))
// 			{
// 				continue;
// 			}

// 			auto score = 0;
// 			for (auto key : KvpKeys)
// 			{
// 				for (auto parameter : parameters)
// 				{
// 					if (IsEqual(key, parameter))
// 					{
// 						score++;
// 					}
// 				}
// 			}

// 			if (KvpKeys.size() == parameters.size())
// 				score *= 10;

// 			if (score > bestScore)
// 			{
// 				bestScore = score;
// 				bestMatch = std::get<1>(kvp);
// 			}
// 		}
// 	}

// 	if (bestScore == 1 && parameters.size() == 1)
// 	{
// 		return bestMatch;
// 	}

// 	return (bestScore > 1 ? bestMatch : nullptr);
// }

// class GameScreen: public std::enable_shared_from_this<GameScreen>
// {
// protected:
// 	SharedPtr<class Domain> _Domain;
// 	std::vector<SharedPtr<class Item>> _Items;
// 	std::vector<KeywordsToActionEntry> KeywordsToAction;

// 	bool _FirstRun = true;

// 	virtual SharedPtr<GameScreen> OnRun(AIContext& context) = 0;
// public:
// 	SharedPtr<Domain> GetDomain() { return _Domain; }
// 	std::vector<SharedPtr<Item>>& Items() { return _Items; }
// 	bool FirstRun() { return _FirstRun; }


// 	void Initialize(SharedPtr<Domain> domain)
// 	{
// 		_Domain = domain;
// 	}


// 	SharedPtr<GameScreen> Run(AIContext& context)
// 	{
// 		auto result = OnRun(context);
// 		if (_FirstRun) _FirstRun = false;
// 		return result;
// 	}

// 	void Write(std::string text, bool readLineAtEnd = true)
// 	{
// 		print_line(text.c_str());
// 		if (readLineAtEnd)
// 		{
// 			print_line("[Enter]");
// 		}
// 	}

// 	SharedPtr<GameScreen> PerformAction(AIContext context, std::string& action);
	

// private:
// 	std::vector<std::string> ToLower(std::vector<std::string>& parameters)
// 	{
// 		for (auto i = 0; i < parameters.size(); i++)
// 		{
// 			parameters[i] = StringToLower(parameters[i]);
// 		}

// 		return parameters;
// 	}

// 	std::vector<std::string> SpliceKnownCommands(std::vector<std::string>& parameters)
// 	{
// 		std::vector<std::string> result;

// 		// corner case of one-word commands.
// 		if(parameters.size() == 1)
//         {
//             result = parameters;
//         }
// 		for (int i = 0; i < parameters.size() - 1; i++)
// 		{
// 			if (parameters[i] == "pick" && parameters[i + 1] == "up")
// 			{
// 				result.push_back("pick up"s);
// 				i++;
// 			}
// 			else
// 			{
// 				result.push_back(parameters[i]);
// 			}
// 		}
// 		parameters = result;
// 		return parameters;
// 	}

// 	ScreenFuncType FindBestMatch(std::vector<std::string>& parameters);
	
// 	bool IsEqual(std::string& a, std::string& b)
// 	{
// 		if (a == b) return true;

// 		if (a == "get")
// 		{
// 			if (b == "pick up") return true;
// 			if (b == "fetch") return true;
// 			if (b == "hold") return true;
// 			if (b == "wield") return true;
// 			if (b == "gather") return true;
// 			if (b == "acquire") return true;
// 			if (b == "take") return true;
// 		}

// 		if (a == "slash")
// 		{
// 			if (b == "swing") return true;
// 			if (b == "cut") return true;
// 			if (b == "attack") return true;
// 			if (b == "hit") return true;
// 			if (b == "hack") return true;
// 			if (b == "slice") return true;
// 			if (b == "slit") return true;
// 		}

// 		if (a == "drink")
// 		{
// 			if (b == "sip") return true;
// 			if (b == "gulp") return true;
// 			if (b == "swallow") return true;
// 			if (b == "quaff") return true;
// 			if (b == "taste") return true;
// 			if (b == "consume") return true;
// 			if (b == "drain") return true;
// 			if (b == "slurp") return true;
// 			if (b == "down") return true;
// 		}

// 		if (a == "open")
// 		{
// 			if (b == "unscrew") return true;
// 			if (b == "uncork") return true;
// 		}

// 		if (a == "throw")
// 		{
// 			if (b == "whirl") return true;
// 			if (b == "fling") return true;
// 			if (b == "hurl") return true;
// 			if (b == "lob") return true;
// 			if (b == "thrust") return true;
// 		}

// 		if (a == "break")
// 		{
// 			if (b == "destroy") return true;
// 			if (b == "shatter") return true;
// 			if (b == "crack") return true;
// 			if (b == "crush") return true;
// 			if (b == "demolish") return true;
// 			if (b == "fracture") return true;
// 			if (b == "ruin") return true;
// 			if (b == "smash") return true;
// 			if (b == "wreck") return true;
// 		}

// 		return false;
// 	}
// };

// SharedPtr<GameScreen> GameScreen::PerformAction(AIContext context, std::string& action)
// {
// 	action = StringToLower(action);
// 	std::vector<std::string> parameters;
// 	StringSplit(action, parameters, ' ');
// 	parameters = SpliceKnownCommands(parameters);
// 	auto func = FindBestMatch(parameters);
// 	SharedPtr<GameScreen> result;
// 	if (func)
// 	{
// 		result = func(context);

// 	}
// 	if (result == nullptr)
// 	{
// 		Actions::Failed(context);
// 		return context.CurrentScreen();
// 	}

// 	return result;
// }

// ScreenFuncType GameScreen::FindBestMatch(std::vector<std::string>& parameters)
// {

// 	ScreenFuncType bestMatch;
// 	int bestScore = 0;
// 	for (auto kvp : KeywordsToAction)
// 	{
// 		auto KvpKeys = std::get<0>(kvp);
// 		if (!IsEqual(KvpKeys[0], parameters[0]))
// 		{
// 			continue;
// 		}

// 		auto score = 0;
// 		for (auto key : KvpKeys)
// 		{
// 			for (auto parameter : parameters)
// 			{
// 				if (IsEqual(key, parameter))
// 				{
// 					score++;
// 				}
// 			}
// 		}

// 		if (KvpKeys.size() == parameters.size())
// 		{
// 			score *= 10;
// 		}

// 		if (score > bestScore)
// 		{
// 			bestScore = score;
// 			bestMatch = std::get<1>(kvp);
// 		}
// 	}

// 	for (auto item : _Items)
// 	{
// 		for(auto kvp : item->KeywordsToAction())
// 		{
// 			auto KvpKeys = std::get<0>(kvp);
// 			if (!IsEqual(KvpKeys[0], parameters[0]))
// 			{
// 				continue;
// 			}

// 			auto score = 0;
// 			for (auto key : KvpKeys)
// 			{
// 				for (auto parameter : parameters)
// 				{
// 					if (IsEqual(key, parameter))
// 					{
// 						score++;
// 					}
// 				}
// 			}

// 			if (KvpKeys.size() == parameters.size())
// 				score *= 10;

// 			if (score > bestScore)
// 			{
// 				bestScore = score;
// 				bestMatch = std::get<1>(kvp);
// 			}
// 		}
// 	}

// 	if (bestScore == 1 && parameters.size() == 1)
// 	{
// 		return bestMatch;
// 	}

// 	return (bestScore > 1 ? bestMatch : nullptr);
// }

TEST_CASE("[Modules][TotalOrderForwardDecomposition] Try to plan succeed") {
    print_line("Hello World!\n");
	// print_line("You're standing in an empty room of white tapestries. There's no doors or windows here.");
	// 	for (auto item : _Items) {
	// 		Write(item->Description());
	// }
	// SharedPtr<GameScreen> Interactibles(AIContext& context)
	// {
	// 	auto action = Prompt("Now, what to do..?");
	// 	return PerformAction(context, action);
	// }

    // auto player = MakeSharedPtr<Player>();
    // player->Init();

    // SharedPtr<GameScreen> nextScreen = MakeSharedPtr<IntroScreen>();
    // while (nextScreen)
    // {
    //     nextScreen = nextScreen->Run(*(player->Context()));
    // }
    print_line("The End!");
}
#endif
