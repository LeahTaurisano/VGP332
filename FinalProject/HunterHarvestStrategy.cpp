#include "HunterHarvestStrategy.h"

#include "GoalHarvestGatherer.h"

float HunterHarvestStrategy::CalculateDesirability(Hunter& agent) const
{
	if (agent.GetTarget() != nullptr && agent.GetCurrentState() == HunterState::HuntGatherers)
	{
		float distanceSqr = X::Math::DistanceSqr(agent.position, agent.GetTarget()->position);
		Gatherer* gatherer = static_cast<Gatherer*>(agent.GetTarget());
		if (distanceSqr < 1000.0f && gatherer->GetHealth() > 0)
		{
			return 1000.0f;
		}
	}
	return 0.0f;
}

std::unique_ptr<AI::Goal<Hunter>> HunterHarvestStrategy::CreateGoal() const
{
	auto newGoal = std::make_unique<GoalHarvestGatherer>();
	return newGoal;
}