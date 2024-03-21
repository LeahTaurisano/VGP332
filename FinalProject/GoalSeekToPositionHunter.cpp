#include "GoalSeekToPositionHunter.h"

GoalSeekToPositionHunter::GoalSeekToPositionHunter()
{
}

void GoalSeekToPositionHunter::Activate(Hunter& agent)
{
	mStatus = GoalSeekToPositionHunter::Status::Active;
	agent.SetSeek(true);
	agent.destination = mDestination;
}

GoalSeekToPositionHunter::Status GoalSeekToPositionHunter::Process(Hunter& agent)
{
	ActivateIfInactive(agent);
	if (X::Math::DistanceSqr(agent.position, mDestination) < mDestinationRange * mDestinationRange)
	{
		mStatus = GoalSeekToPositionHunter::Status::Completed;
	}
	return mStatus;
}

void GoalSeekToPositionHunter::Terminate(Hunter& agent)
{
	agent.SetSeek(false);
}

void GoalSeekToPositionHunter::SetDestination(const X::Math::Vector2& destination)
{
	mDestination = destination;
}

void GoalSeekToPositionHunter::SetDestinationRange(float range)
{
	mDestinationRange = range;
}
