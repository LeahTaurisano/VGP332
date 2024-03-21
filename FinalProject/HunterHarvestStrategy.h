#pragma once

#include <AI.h>

#include "Hunter.h"

class HunterHarvestStrategy : public AI::Strategy<Hunter>
{
public:
	float CalculateDesirability(Hunter& agent) const override;
	std::unique_ptr<AI::Goal<Hunter>> CreateGoal() const override;

private:
	const AI::PerceptionModule* mPerception = nullptr;
};