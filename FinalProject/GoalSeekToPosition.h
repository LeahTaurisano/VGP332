#pragma once

#include <AI.h>

#include "Gatherer.h"

class GoalSeekToPosition : public AI::Goal<Gatherer>
{
public:
	using Status = AI::Goal<Gatherer>::Status;

	GoalSeekToPosition();

	void Activate(Gatherer& agent) override;
	Status Process(Gatherer& agent) override;
	void Terminate(Gatherer& agent) override;

	void SetDestination(const X::Math::Vector2& destination);
	void SetDestinationRange(float range);

private:
	X::Math::Vector2 mDestination;
	float mDestinationRange = 10.0f;
};