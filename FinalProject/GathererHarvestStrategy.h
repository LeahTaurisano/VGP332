#pragma once

#include <AI.h>

#include "Gatherer.h"

class GathererHarvestStrategy : public AI::Strategy<Gatherer>
{
public:
	float CalculateDesirability(Gatherer& agent) const override;
	std::unique_ptr<AI::Goal<Gatherer>> CreateGoal() const override;

private:
	const AI::PerceptionModule* mPerception = nullptr;
};