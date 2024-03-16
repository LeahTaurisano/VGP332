#include "GathererGoHomeStrategy.h"

#include "TypeIds.h"
#include "GoalMoveToPosition.h"

void GathererGoHomeStrategy::SetPerception(const AI::PerceptionModule* perception)
{
	mPerception = perception;
}

float GathererGoHomeStrategy::CalculateDesirability(Gatherer& agent) const
{
	if (agent.GetCurrentState() == GathererState::GoHome)
	{
		return 1000.0f;
	}
	return 0.0f;
}

std::unique_ptr<AI::Goal<Gatherer>> GathererGoHomeStrategy::CreateGoal() const
{
	auto newGoal = std::make_unique<GoalMoveToPosition>();
	newGoal->SetDestination(mDestination);
	newGoal->SetTilemap(mTileMap);
	return newGoal;
}

void GathererGoHomeStrategy::SetDestination(const X::Math::Vector2& destination)
{
	mDestination = destination;
}

void GathererGoHomeStrategy::SetTileMap(TileMap* tileMap)
{
	mTileMap = tileMap;
}
