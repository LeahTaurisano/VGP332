#include "GoalWander.h"

GoalWander::GoalWander()
{
}

void GoalWander::Activate(Gatherer& agent)
{
	mStatus = GoalWander::Status::Active;
	agent.SetWander(true);
}

GoalWander::Status GoalWander::Process(Gatherer& agent)
{
	ActivateIfInactive(agent);
	return mStatus;
}

void GoalWander::Terminate(Gatherer& agent)
{
	agent.SetWander(false);
}
