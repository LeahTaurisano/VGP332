#include "HunterGoToGathererStrategy.h"

#include "TypeIds.h"
#include "GoalMoveToPositionHunter.h"

void HunterGoToGathererStrategy::SetPerception(const AI::PerceptionModule* perception)
{
	mPerception = perception;
}

float HunterGoToGathererStrategy::CalculateDesirability(Hunter& agent) const
{
	if (agent.GetCurrentState() == HunterState::HuntGatherers)
	{
		const auto& memoryRecords = mPerception->GetMemoryRecords();
		float highestImportance = 0.0f;
		for (auto& record : memoryRecords)
		{
			AgentType agentType = static_cast<AgentType>(record.GetProperty<int>("type", 0));
			if (agentType == AgentType::Gatherer)
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
	return 0.0f;
}

std::unique_ptr<AI::Goal<Hunter>> HunterGoToGathererStrategy::CreateGoal() const
{
	const auto& memoryRecords = mPerception->GetMemoryRecords();
	float highestImportance = 0.0f;
	X::Math::Vector2 targetDestination = X::Math::Vector2::Zero();
	for (auto& record : memoryRecords)
	{
		AgentType agentType = static_cast<AgentType>(record.GetProperty<int>("type", 0));
		if (agentType == AgentType::Gatherer)
		{
			if (record.importance > highestImportance)
			{
				highestImportance = record.importance;
				targetDestination = record.GetProperty<X::Math::Vector2>("lastSeenPosition");
			}
		}
	}
	auto newGoal = std::make_unique<GoalMoveToPositionHunter>();
	newGoal->SetTilemap(mTileMap);
	newGoal->SetDestination(mTileMap->GetTilePosition(targetDestination));
	return newGoal;
}

void HunterGoToGathererStrategy::SetDestination(const X::Math::Vector2& destination)
{
	mDestination = destination;
}

void HunterGoToGathererStrategy::SetTileMap(TileMap* tileMap)
{
	mTileMap = tileMap;
}