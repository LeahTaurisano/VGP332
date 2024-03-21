#pragma once

#include <AI.h>

#include "Hunter.h"

class GoalArriveAtPositionHunter : public AI::Goal<Hunter>
{
public:
	using Status = AI::Goal<Hunter>::Status;

	GoalArriveAtPositionHunter();

	void Activate(Hunter& agent) override;
	Status Process(Hunter& agent) override;
	void Terminate(Hunter& agent) override;

	void SetDestination(const X::Math::Vector2& destination);

private:
	float mDestinatioRange = 10.0f;
	X::Math::Vector2 mDestination;
};