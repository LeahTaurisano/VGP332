#pragma once

#include <AI.h>

#include "Gatherer.h"

class GathererStrategy : public AI::Strategy<Gatherer>
{
public:
	void SetTargetDestination(const X::Math::Vector2& destination);

	float CalculateDesirability(Gatherer& agent) const override;
	std::unique_ptr<AI::Goal<Gatherer>> CreateGoal() const override;

private:
	X::Math::Vector2 mTargetDestination;
};