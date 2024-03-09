#include "GathererGoToResourceStrategy.h"

#include "TypeIds.h"
#include "GoalMoveToPosition.h"

void GathererGoToResourceStrategy::SetPerception(const AI::PerceptionModule* perception)
{
	mPerception = perception;
}

float GathererGoToResourceStrategy::CalculateDesirability(Gatherer& agent) const
{
	const auto& memoryRecords = mPerception->GetMemoryRecords();
	float highestImportance = 0.0f;
	for (auto& record : memoryRecords)
	{
		AgentType agentType = static_cast<AgentType>(record.GetProperty<int>("type", 0));
		if (agentType == AgentType::Resource)
		{
			if (record.importance > highestImportance)
			{
				highestImportance = record.importance;
				const AI::EntityPtrs entities = agent.world.GetEntities();
				for (AI::Entity* entity : entities)
				{
					if (entity->GetUniqueId() == record.uniqueId)
					{
						agent.SetTarget(entity);
					}
				}
			}
		}
	}
	return highestImportance;
}

std::unique_ptr<AI::Goal<Gatherer>> GathererGoToResourceStrategy::CreateGoal() const
{
	const auto& memoryRecords = mPerception->GetMemoryRecords();
	float highestImportance = 0.0f;
	X::Math::Vector2 targetDestination = X::Math::Vector2::Zero();
	for (auto& record : memoryRecords)
	{
		AgentType agentType = static_cast<AgentType>(record.GetProperty<int>("type", 0));
		if (agentType == AgentType::Resource)
		{
			if (record.importance > highestImportance)
			{
				highestImportance = record.importance;
				targetDestination = record.GetProperty<X::Math::Vector2>("lastSeenPosition");
			}
		}
	}
	auto newGoal = std::make_unique<GoalMoveToPosition>();
	newGoal->SetDestination(targetDestination);
	return newGoal;
}