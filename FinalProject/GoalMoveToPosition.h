#pragma once

#include <AI.h>

#include "Gatherer.h"
#include "TileMap.h"

class GoalMoveToPosition : public AI::GoalComposite<Gatherer>
{
public:
	using Status = AI::Goal<Gatherer>::Status;

	GoalMoveToPosition();

	void Activate(Gatherer& agent) override;
	Status Process(Gatherer& agent) override;
	void Terminate(Gatherer& agent) override;

	void SetDestination(const X::Math::Vector2& destination);
	void SetTilemap(TileMap* tileMap);

private:
	X::Math::Vector2 mDestination;
	TileMap* mTileMap = nullptr;
};