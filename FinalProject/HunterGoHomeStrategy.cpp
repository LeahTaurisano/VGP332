#include "HunterGoHomeStrategy.h"

#include "TypeIds.h"
#include "GoalMoveToPositionHunter.h"

void HunterGoHomeStrategy::SetPerception(const AI::PerceptionModule* perception)
{
	mPerception = perception;
}

float HunterGoHomeStrategy::CalculateDesirability(Hunter& agent) const
{
	if (agent.GetCurrentState() == HunterState::GoHome)
	{
		return 1000.0f;
	}
	return 0.0f;
}

std::unique_ptr<AI::Goal<Hunter>> HunterGoHomeStrategy::CreateGoal() const
{
	auto newGoal = std::make_unique<GoalMoveToPositionHunter>();
	newGoal->SetDestination(mDestination);
	newGoal->SetTilemap(mTileMap);
	return newGoal;
}

void HunterGoHomeStrategy::SetDestination(const X::Math::Vector2& destination)
{
	mDestination = destination;
}

void HunterGoHomeStrategy::SetTileMap(TileMap* tileMap)
{
	mTileMap = tileMap;
}
