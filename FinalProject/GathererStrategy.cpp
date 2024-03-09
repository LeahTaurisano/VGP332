#include "GathererStrategy.h"
#include "GoalMoveToPosition.h"

void GathererStrategy::SetTargetDestination(const X::Math::Vector2& destination)
{
	mTargetDestination = destination;
}

float GathererStrategy::CalculateDesirability(Gatherer& agent) const
{
	return 100.0f;
}

std::unique_ptr<AI::Goal<Gatherer>> GathererStrategy::CreateGoal() const
{
	auto newGoal = std::make_unique<GoalMoveToPosition>();
	newGoal->SetDestination(mTargetDestination);
	return newGoal;
}
