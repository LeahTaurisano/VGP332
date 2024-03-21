#include "GoalMoveToPositionHunter.h"
#include "GoalSeekToPositionHunter.h"
#include "GoalArriveAtPositionHunter.h"

GoalMoveToPositionHunter::GoalMoveToPositionHunter()
{
}

void GoalMoveToPositionHunter::Activate(Hunter& agent)
{
	mStatus = GoalMoveToPositionHunter::Status::Active;
	RemoveAllSubGoals(agent);

	//Need to pass in the Node positions, not the agent position. Need to figure out how to convert one to the other
	//Fix these magic numbers
	X::Math::Vector2 startPos = mTileMap->GetTilePosition(agent.position);
	Path path = mTileMap->FindPathAStar(startPos.x, startPos.y, mDestination.x, mDestination.y);

	float seekDistance = 25.0f;
	for (int i = path.size() - 1; i >= 0; --i)
	{
		if (i != path.size() - 1)
		{
			GoalSeekToPositionHunter* seek = AddSubGoal<GoalSeekToPositionHunter>();
			seek->SetDestination(path[i]);
			seek->SetDestinationRange(seekDistance);
		}
		else
		{
			GoalArriveAtPositionHunter* arrive = AddSubGoal<GoalArriveAtPositionHunter>();
			arrive->SetDestination(path[i]);
		}
	}
}

GoalMoveToPositionHunter::Status GoalMoveToPositionHunter::Process(Hunter& agent)
{
	ActivateIfInactive(agent);
	mStatus = ProcessSubGoals(agent);
	ReactivateIfFailed(agent);
	return mStatus;
}

void GoalMoveToPositionHunter::Terminate(Hunter& agent)
{
	RemoveAllSubGoals(agent);
}

void GoalMoveToPositionHunter::SetDestination(const X::Math::Vector2& destination)
{
	mDestination = destination;
}

void GoalMoveToPositionHunter::SetTilemap(TileMap* tileMap)
{
	mTileMap = tileMap;
}
