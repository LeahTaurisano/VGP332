#pragma once

#include <AI.h>

#include "Gatherer.h"

class GoalArriveAtPosition : public AI::Goal<Gatherer>
{
public:
	using Status = AI::Goal<Gatherer>::Status;

	GoalArriveAtPosition();

	void Activate(Gatherer& agent) override;
	Status Process(Gatherer& agent) override;
	void Terminate(Gatherer& agent) override;

	void SetDestination(const X::Math::Vector2& destination);

private:
	float mDestinatioRange = 10.0f;
	X::Math::Vector2 mDestination;
};