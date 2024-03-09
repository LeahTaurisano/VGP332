#include "GoalArriveAtPosition.h"

GoalArriveAtPosition::GoalArriveAtPosition()
{
}

void GoalArriveAtPosition::Activate(Gatherer& agent)
{
	mStatus = GoalArriveAtPosition::Status::Active;
	agent.SetArrive(true);
	agent.destination = mDestination;
}

GoalArriveAtPosition::Status GoalArriveAtPosition::Process(Gatherer& agent)
{
	ActivateIfInactive(agent);
	if (X::Math::DistanceSqr(agent.position, mDestination) < 25.0f)
	{
		mStatus = GoalArriveAtPosition::Status::Completed;
	}
	return mStatus;
}

void GoalArriveAtPosition::Terminate(Gatherer& agent)
{
	agent.SetArrive(false);
}

void GoalArriveAtPosition::SetDestination(const X::Math::Vector2& destination)
{
	mDestination = destination;
}
