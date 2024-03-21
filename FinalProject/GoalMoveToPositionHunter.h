#pragma once

#include <AI.h>

#include "Hunter.h"
#include "TileMap.h"

class GoalMoveToPositionHunter : public AI::GoalComposite<Hunter>
{
public:
	using Status = AI::Goal<Hunter>::Status;

	GoalMoveToPositionHunter();

	void Activate(Hunter& agent) override;
	Status Process(Hunter& agent) override;
	void Terminate(Hunter& agent) override;

	void SetDestination(const X::Math::Vector2& destination);
	void SetTilemap(TileMap* tileMap);

private:
	X::Math::Vector2 mDestination;
	TileMap* mTileMap = nullptr;
};