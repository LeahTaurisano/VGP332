#pragma once

#include <AI.h>

#include "Gatherer.h"

class GoalWander : public AI::Goal<Gatherer>
{
public:
	using Status = AI::Goal<Gatherer>::Status;

	GoalWander();

	void Activate(Gatherer& agent) override;
	Status Process(Gatherer& agent) override;
	void Terminate(Gatherer& agent) override;
};