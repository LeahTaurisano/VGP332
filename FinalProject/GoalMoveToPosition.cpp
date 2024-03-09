#include "GoalMoveToPosition.h"
#include "GoalSeekToPosition.h"
#include "GoalArriveAtPosition.h"

GoalMoveToPosition::GoalMoveToPosition()
{
}

void GoalMoveToPosition::Activate(Gatherer& agent)
{
	mStatus = GoalMoveToPosition::Status::Active;
	RemoveAllSubGoals(agent);
	float distanceSqr = X::Math::DistanceSqr(agent.position, mDestination);
	float seekDistance = 200.0f;
	if (distanceSqr > 10.0f)
	{
		GoalArriveAtPosition* arrive = AddSubGoal<GoalArriveAtPosition>();
		arrive->SetDestination(mDestination);
	}
	if (distanceSqr > seekDistance * seekDistance)
	{
		GoalSeekToPosition* seek = AddSubGoal<GoalSeekToPosition>();
		seek->SetDestination(mDestination);
		seek->SetDestinationRange(seekDistance);
	}
}

GoalMoveToPosition::Status GoalMoveToPosition::Process(Gatherer& agent)
{
	ActivateIfInactive(agent);
	mStatus = ProcessSubGoals(agent);
	ReactivateIfFailed(agent);
	return mStatus;
}

void GoalMoveToPosition::Terminate(Gatherer& agent)
{
	RemoveAllSubGoals(agent);
}

void GoalMoveToPosition::SetDestination(const X::Math::Vector2& destination)
{
	mDestination = destination;
}
