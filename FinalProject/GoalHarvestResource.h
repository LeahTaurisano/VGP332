#pragma once

#include <AI.h>

#include "Gatherer.h"
#include "Resource.h"

class GoalHarvestResource : public AI::Goal<Gatherer>
{
public:
	using Status = AI::Goal<Gatherer>::Status;

	GoalHarvestResource();

	void Activate(Gatherer& agent) override;
	Status Process(Gatherer& agent) override;
	void Terminate(Gatherer& agent) override;

private:
	float mStartTime = 0.0f;
	float mHarvestTime = 5.0f;
	Resource* mTargetMineral = nullptr;
};