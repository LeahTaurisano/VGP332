#include "GathererGoToResourceStrategy.h"

#include "TypeIds.h"
#include "GoalMoveToPosition.h"

void GathererGoToResourceStrategy::SetPerception(const AI::PerceptionModule* perception)
{
	mPerception = perception;
}

float GathererGoToResourceStrategy::CalculateDesirability(Gatherer& agent) const
{
	return 1000.0f;
}

std::unique_ptr<AI::Goal<Gatherer>> GathererGoToResourceStrategy::CreateGoal() const
{
	auto newGoal = std::make_unique<GoalMoveToPosition>();
	newGoal->SetDestination(mDestination);
	newGoal->SetTilemap(mTileMap);
	return newGoal;
}

void GathererGoToResourceStrategy::SetDestination(const X::Math::Vector2& destination)
{
	mDestination = destination;
}

void GathererGoToResourceStrategy::SetTileMap(TileMap* tileMap)
{
	mTileMap = tileMap;
}
