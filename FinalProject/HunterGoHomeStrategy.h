#pragma once

#include <AI.h>

#include "Hunter.h"
#include "TileMap.h"

class HunterGoHomeStrategy : public AI::Strategy<Hunter>
{
public:
	void SetPerception(const AI::PerceptionModule* perception);
	float CalculateDesirability(Hunter& agent) const override;
	std::unique_ptr<AI::Goal<Hunter>> CreateGoal() const override;

	void SetDestination(const X::Math::Vector2& destination);
	void SetTileMap(TileMap* tileMap);

private:
	const AI::PerceptionModule* mPerception = nullptr;
	X::Math::Vector2 mDestination;
	TileMap* mTileMap = nullptr;
};