#pragma once

#include <AI.h>

#include "Gatherer.h"

class GathererGoToResourceStrategy : public AI::Strategy<Gatherer>
{
public:
	void SetPerception(const AI::PerceptionModule* perception);
	float CalculateDesirability(Gatherer& agent) const override;
	std::unique_ptr<AI::Goal<Gatherer>> CreateGoal() const override;

private:
	const AI::PerceptionModule* mPerception = nullptr;
};