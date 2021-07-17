#include "tfd.h"
#include <iostream>

namespace tfd_cpp
{
    TFD::TFD(const PlanningProblem& planningProblem) : 
        m_planningProblem(planningProblem)
    {
    }

    TFD::~TFD() {}

    TFD::Plan TFD::TryToPlan()
    {
        std::vector<Task> tasks;
        Plan solutionPlan;
        tasks.push_back(m_planningProblem.GetTopLevelTask());

        std::cout << "TryToPlan for: " << tasks.back().taskName;

        return SeekPlan(tasks, m_planningProblem.GetInitialState(), solutionPlan);
    }

    TFD::Plan TFD::SeekPlan(const std::vector<Task>& tasks, const State& currentState, Plan& currentPlan)
    {
        if (tasks.empty())
        {
            std::cout << "SeekPlan: No more tasks, returning current plan.\n";
            if (not currentPlan.empty())
            {
                std::cout << "TFD found solution plan." << std::endl;
                for (const auto& operatorWithParams : currentPlan)
                {
                    std::cout << operatorWithParams.task.taskName;
                }
            }
            return currentPlan;
        }

        if (m_planningProblem.TaskIsOperator(tasks.back().taskName))
        {
            std::cout << "SeekPlan: Task is operator type.\n";
            return SearchOperators(tasks, currentState, currentPlan);
        }

        if (m_planningProblem.TaskIsMethod(tasks.back().taskName))
        {
            std::cout << "SeekPlan: Task is method type.\n";
            return SearchMethods(tasks, currentState, currentPlan);
        }

        return {};
    }

    TFD::Plan TFD::SearchMethods(const std::vector<Task>& tasks, const State& currentState, Plan& currentPlan)
    {
        std::cout << "SearchMethods for " << tasks.back().taskName;
        RelevantMethods relevantMethods = m_planningProblem.GetMethodsForTask(tasks.back(), currentState);
        
        if (not relevantMethods.empty())
        {
            std::cout << "SearchMethods: " << relevantMethods.size() <<  " relevant methods found.\n";

            for (const auto& relevantMethod : relevantMethods)
            {
                auto subTasks = relevantMethod.func(currentState, relevantMethod.task.parameters);
                if (subTasks and not subTasks.value().empty())
                {
                    std::vector<Task> newTasks(tasks);
                    newTasks.pop_back();
                    newTasks.insert(newTasks.end(), subTasks.value().begin(), subTasks.value().end());

                    auto solution = SeekPlan(newTasks, currentState, currentPlan);
                    if (not solution.empty())
                    {
                        return solution;
                    }
                }
            }            
        }
        else
        {
            std::cout << "SearchMethods: No relevant methods found.\n";
        }

        std::cout << "SearchMethods: Failed to plan";
        return {};
    }

    TFD::Plan TFD::SearchOperators(const std::vector<Task>& tasks, const State& currentState, Plan& currentPlan)
    {
        std::cout << "SearchOperators for " << tasks.back().taskName;
        ApplicableOperators applicableOperators = m_planningProblem.GetOperatorsForTask(tasks.back(), currentState);
        
        if (not applicableOperators.empty())
        {
            for (const auto& chosenOperator : applicableOperators)
            {
                const auto newState = chosenOperator.func(currentState, chosenOperator.task.parameters);
                
                if (newState)
                {
                    std::vector<Task> newTasks(tasks);
                    newTasks.pop_back();
                    currentPlan.push_back(chosenOperator);

                    auto solution = SeekPlan(newTasks, newState.value(), currentPlan);
                    if (not solution.empty())
                    {
                        return solution;
                    }
                }
            }
        }
        else
        {
            std::cout << "SearchOperators: No applicable operator found.\n";
        }

        return {};
    }
}
