#pragma once

#include <AI.h>

#include "Hunter.h"
#include "Gatherer.h"

class GoalHarvestGatherer : public AI::Goal<Hunter>
{
public:
	using Status = AI::Goal<Hunter>::Status;

	GoalHarvestGatherer();

	void Activate(Hunter& agent) override;
	Status Process(Hunter& agent) override;
	void Terminate(Hunter& agent) override;

private:
	float mStartTime = 0.0f;
	float mHarvestTime = 5.0f;
	Gatherer* mTargetGatherer = nullptr;
};