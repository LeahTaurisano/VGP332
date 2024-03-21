#include "GoalHarvestGatherer.h"

GoalHarvestGatherer::GoalHarvestGatherer()
{
}

void GoalHarvestGatherer::Activate(Hunter& agent)
{
	mStartTime = X::GetTime();
}

GoalHarvestGatherer::Status GoalHarvestGatherer::Process(Hunter& agent)
{
	ActivateIfInactive(agent);
	float currentTime = X::GetTime();
	float deltaTime = currentTime - mStartTime + 0.01f;
	mStartTime = currentTime;
	Gatherer* gatherer = static_cast<Gatherer*>(agent.GetTarget());
	gatherer->SetHealth(gatherer->GetHealth() - (20 * deltaTime));
	if (gatherer->GetHealth() <= 0)
	{
		agent.SetTarget(nullptr);
		agent.HeldGatherer(true);
		mStatus = GoalHarvestGatherer::Status::Completed;
	}
	return mStatus;
}

void GoalHarvestGatherer::Terminate(Hunter& agent)
{
	
}
