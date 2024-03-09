#include "GathererHarvestStrategy.h"

#include "GoalHarvestResource.h"

float GathererHarvestStrategy::CalculateDesirability(Gatherer& agent) const
{
	if (agent.GetTarget() != nullptr)
	{
		float distanceSqr = X::Math::DistanceSqr(agent.position, agent.GetTarget()->position);
		Resource* resource = static_cast<Resource*>(agent.GetTarget());
		if (distanceSqr < 25.0f && resource->GetHealth() > 0)
		{
			return 1000.0f;
		}
	}
	return 0.0f;
}

std::unique_ptr<AI::Goal<Gatherer>> GathererHarvestStrategy::CreateGoal() const
{
	auto newGoal = std::make_unique<GoalHarvestResource>();
	return newGoal;
}