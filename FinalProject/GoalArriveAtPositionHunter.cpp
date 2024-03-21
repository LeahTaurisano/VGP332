#include "GoalArriveAtPositionHunter.h"

GoalArriveAtPositionHunter::GoalArriveAtPositionHunter()
{
}

void GoalArriveAtPositionHunter::Activate(Hunter& agent)
{
	mStatus = GoalArriveAtPositionHunter::Status::Active;
	agent.SetArrive(true);
	agent.destination = mDestination;
}

GoalArriveAtPositionHunter::Status GoalArriveAtPositionHunter::Process(Hunter& agent)
{
	ActivateIfInactive(agent);
	if (X::Math::DistanceSqr(agent.position, mDestination) < 25.0f)
	{
		mStatus = GoalArriveAtPositionHunter::Status::Completed;
	}
	return mStatus;
}

void GoalArriveAtPositionHunter::Terminate(Hunter& agent)
{
	agent.SetArrive(false);
}

void GoalArriveAtPositionHunter::SetDestination(const X::Math::Vector2& destination)
{
	mDestination = destination;
}
