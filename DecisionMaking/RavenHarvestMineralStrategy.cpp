#include "RavenHarvestMineralStrategy.h"

#include "GoalHarvestMineral.h"

float RavenHarvestMineralStrategy::CalculateDesirability(Raven& agent) const
{
	float distanceSqr = X::Math::DistanceSqr(agent.position, agent.GetTarget()->position);
	Mineral* mineral = static_cast<Mineral*>(agent.GetTarget());
	if (distanceSqr < 10.0f && mineral->GetHealth() > 0)
	{
		return 1000.0f;
	}
	return 0.0f;
}

std::unique_ptr<AI::Goal<Raven>> RavenHarvestMineralStrategy::CreateGoal() const
{
	auto newGoal = std::make_unique<GoalHarvestMineral>();
	return newGoal;
}