#include "HunterGoToHuntingSpotStrategy.h"

#include "TypeIds.h"
#include "GoalMoveToPositionHunter.h"

void HunterGoToHuntingSpotStrategy::SetPerception(const AI::PerceptionModule* perception)
{
	mPerception = perception;
}

float HunterGoToHuntingSpotStrategy::CalculateDesirability(Hunter& agent) const
{
	if (agent.GetCurrentState() == HunterState::GoToHuntingSpot)
	{
		return 1000.0f;
	}
	return 0.0f;
}

std::unique_ptr<AI::Goal<Hunter>> HunterGoToHuntingSpotStrategy::CreateGoal() const
{
	auto newGoal = std::make_unique<GoalMoveToPositionHunter>();
	newGoal->SetDestination(mDestination);
	newGoal->SetTilemap(mTileMap);
	return newGoal;
}

void HunterGoToHuntingSpotStrategy::SetDestination(const X::Math::Vector2& destination)
{
	mDestination = destination;
}

void HunterGoToHuntingSpotStrategy::SetTileMap(TileMap* tileMap)
{
	mTileMap = tileMap;
}
