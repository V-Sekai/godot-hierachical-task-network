#pragma once

#include <memory>
#include <vector>

#include <modules/tfd/planning_domain.h>
#include <modules/tfd/planning_problem.h>

PlanningProblem CreatePlanningProblem(const Task& topLevelTask);
