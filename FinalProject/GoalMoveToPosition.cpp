#include "GoalMoveToPosition.h"
#include "GoalSeekToPosition.h"
#include "GoalArriveAtPosition.h"

GoalMoveToPosition::GoalMoveToPosition()
{
}

void GoalMoveToPosition::Activate(Gatherer& agent)
{
	mStatus = GoalMoveToPosition::Status::Active;
	RemoveAllSubGoals(agent);

	//Need to pass in the Node positions, not the agent position. Need to figure out how to convert one to the other
	Path path = mTileMap.FindPathAStar(agent.position.x, agent.position.y, mDestination.x, mDestination.y);

	float seekDistance = 200.0f;
	for (int i = 0; i < path.size(); ++i)
	{
		if (i != path.size() - 1)
		{
			GoalSeekToPosition* seek = AddSubGoal<GoalSeekToPosition>();
			seek->SetDestination(path[i]);
			seek->SetDestinationRange(seekDistance);
		}
		else
		{
			GoalArriveAtPosition* arrive = AddSubGoal<GoalArriveAtPosition>();
			arrive->SetDestination(path[i]);
		}
	}
}

GoalMoveToPosition::Status GoalMoveToPosition::Process(Gatherer& agent)
{
	ActivateIfInactive(agent);
	mStatus = ProcessSubGoals(agent);
	ReactivateIfFailed(agent);
	return mStatus;
}

void GoalMoveToPosition::Terminate(Gatherer& agent)
{
	RemoveAllSubGoals(agent);
}

void GoalMoveToPosition::SetDestination(const X::Math::Vector2& destination)
{
	mDestination = destination;
}

void GoalMoveToPosition::SetTilemap(const TileMap& tileMap)
{
	mTileMap = tileMap;
}
