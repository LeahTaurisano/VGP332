#include "Precompiled.h"
#include "EvadeBehavior.h"

#include "Agent.h"

using namespace AI;

X::Math::Vector2 AI::EvadeBehavior::Calculate(Agent& agent)
{
	if (agent.target == nullptr)
	{
		return X::Math::Vector2();
	}

	X::Math::Vector2 agentToTarget = agent.target->position - agent.position;
	float distToTarget = X::Math::Magnitude(agentToTarget);
	if (distToTarget >= 10.0f)
	{
		return X::Math::Vector2();
	}


}
