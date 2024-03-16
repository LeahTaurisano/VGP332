#include "GoalHarvestResource.h"

GoalHarvestResource::GoalHarvestResource()
{
}

void GoalHarvestResource::Activate(Gatherer& agent)
{
	mStartTime = X::GetTime();
}

GoalHarvestResource::Status GoalHarvestResource::Process(Gatherer& agent)
{
	ActivateIfInactive(agent);
	float currentTime = X::GetTime();
	float deltaTime = currentTime - mStartTime + 0.01f;
	mStartTime = currentTime;
	Resource* resource = static_cast<Resource*>(agent.GetTarget());
	resource->SetHealth(resource->GetHealth() - (20 * deltaTime));
	if (resource->GetHealth() <= 0)
	{
		agent.SetTarget(nullptr);
		agent.HeldResource(true);
		mStatus = GoalHarvestResource::Status::Completed;
	}
	return mStatus;
}

void GoalHarvestResource::Terminate(Gatherer& agent)
{
	
}
