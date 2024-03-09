#include "GathererForageStrategy.h"

#include "GoalWander.h"


float GathererForageStrategy::CalculateDesirability(Gatherer& agent) const
{
	return 10.0f;
}

std::unique_ptr<AI::Goal<Gatherer>> GathererForageStrategy::CreateGoal() const
{
	auto newGoal = std::make_unique<GoalWander>();
	return newGoal;
}
