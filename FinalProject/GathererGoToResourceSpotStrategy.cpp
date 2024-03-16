#include "GathererGoToResourceSpotStrategy.h"

#include "TypeIds.h"
#include "GoalMoveToPosition.h"

void GathererGoToResourceSpotStrategy::SetPerception(const AI::PerceptionModule* perception)
{
	mPerception = perception;
}

float GathererGoToResourceSpotStrategy::CalculateDesirability(Gatherer& agent) const
{
	if (agent.GetCurrentState() == GathererState::GoToGatherSpot)
	{
		return 1000.0f;
	}
	return 0.0f;
}

std::unique_ptr<AI::Goal<Gatherer>> GathererGoToResourceSpotStrategy::CreateGoal() const
{
	auto newGoal = std::make_unique<GoalMoveToPosition>();
	newGoal->SetDestination(mDestination);
	newGoal->SetTilemap(mTileMap);
	return newGoal;
}

void GathererGoToResourceSpotStrategy::SetDestination(const X::Math::Vector2& destination)
{
	mDestination = destination;
}

void GathererGoToResourceSpotStrategy::SetTileMap(TileMap* tileMap)
{
	mTileMap = tileMap;
}
