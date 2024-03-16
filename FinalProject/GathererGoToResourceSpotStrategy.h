#pragma once

#include <AI.h>

#include "Gatherer.h"
#include "TileMap.h"

class GathererGoToResourceSpotStrategy : public AI::Strategy<Gatherer>
{
public:
	void SetPerception(const AI::PerceptionModule* perception);
	float CalculateDesirability(Gatherer& agent) const override;
	std::unique_ptr<AI::Goal<Gatherer>> CreateGoal() const override;

	void SetDestination(const X::Math::Vector2& destination);
	void SetTileMap(TileMap* tileMap);

private:
	const AI::PerceptionModule* mPerception = nullptr;
	X::Math::Vector2 mDestination;
	TileMap* mTileMap = nullptr;
};