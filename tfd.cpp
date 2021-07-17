#include "tfd.h"
#include <iostream>

namespace tfd_cpp
{
    TotalOrderForwardDecomposition::TotalOrderForwardDecomposition(const PlanningProblem& planningProblem) : 
        m_planningProblem(planningProblem)
    {
    }

    TotalOrderForwardDecomposition::~TotalOrderForwardDecomposition() {}

    TotalOrderForwardDecomposition::Plan TotalOrderForwardDecomposition::TryToPlan()
    {
        std::vector<Task> tasks;
        Plan solutionPlan;
        tasks.push_back(m_planningProblem.GetTopLevelTask());

        std::cout << "TryToPlan for: " << tasks.back().taskName;

        return SeekPlan(tasks, m_planningProblem.GetInitialState(), solutionPlan);
    }

    TotalOrderForwardDecomposition::Plan TotalOrderForwardDecomposition::SeekPlan(const std::vector<Task>& tasks, const State& currentState, Plan& currentPlan)
    {
        if (tasks.empty())
        {
            std::cout << "SeekPlan: No more tasks, returning current plan.\n";
            if (!currentPlan.empty())
            {
                std::cout << "TFD found solution plan." << std::endl;
				for (const OperatorWithParams &operatorWithParams : currentPlan) {
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

    TotalOrderForwardDecomposition::Plan TotalOrderForwardDecomposition::SearchMethods(const std::vector<Task>& tasks, const State& currentState, Plan& currentPlan)
    {
        std::cout << "SearchMethods for " << tasks.back().taskName;
        RelevantMethods relevantMethods = m_planningProblem.GetMethodsForTask(tasks.back(), currentState);
        
        if (! relevantMethods.empty())
        {
            std::cout << "SearchMethods: " << relevantMethods.size() <<  " relevant methods found.\n";

			for (const MethodWithParams &relevantMethod : relevantMethods) {
				std::optional<std::vector<Task>> subTasks = relevantMethod.func(currentState, relevantMethod.task.parameters);
				if (subTasks && ! subTasks.value().empty())
                {
                    std::vector<Task> newTasks(tasks);
                    newTasks.pop_back();
                    newTasks.insert(newTasks.end(), subTasks.value().begin(), subTasks.value().end());

					std::vector<OperatorWithParams> solution = SeekPlan(newTasks, currentState, currentPlan);
					if (! solution.empty())
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

    TotalOrderForwardDecomposition::Plan TotalOrderForwardDecomposition::SearchOperators(const std::vector<Task>& tasks, const State& currentState, Plan& currentPlan)
    {
        std::cout << "SearchOperators for " << tasks.back().taskName;
        ApplicableOperators applicableOperators = m_planningProblem.GetOperatorsForTask(tasks.back(), currentState);
        
        if (! applicableOperators.empty())
        {
			for (const OperatorWithParams &chosenOperator : applicableOperators) {
				const std::optional<State> newState = chosenOperator.func(currentState, chosenOperator.task.parameters);

				if (newState)
                {
                    std::vector<Task> newTasks(tasks);
                    newTasks.pop_back();
                    currentPlan.push_back(chosenOperator);

					std::vector<OperatorWithParams> solution = SeekPlan(newTasks, newState.value(), currentPlan);
					if (! solution.empty())
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
