#pragma once

#include <AI.h>

#include "Hunter.h"

class GoalSeekToPositionHunter : public AI::Goal<Hunter>
{
public:
	using Status = AI::Goal<Hunter>::Status;

	GoalSeekToPositionHunter();

	void Activate(Hunter& agent) override;
	Status Process(Hunter& agent) override;
	void Terminate(Hunter& agent) override;

	void SetDestination(const X::Math::Vector2& destination);
	void SetDestinationRange(float range);

private:
	X::Math::Vector2 mDestination;
	float mDestinationRange = 10.0f;
};