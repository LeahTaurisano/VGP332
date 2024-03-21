#include "GathererStates.h"

void StayHomeAndRestState::Enter(Gatherer& agent)
{
	if (agent.GetHasResource())
	{
		agent.HeldResource(false);
		agent.SetDepositedResource(true);
	}
}

void StayHomeAndRestState::Update(Gatherer& agent, float deltaTime)
{
	if (agent.GetEnergy() > agent.GetEnergyMax())
	{
		agent.GoToGather();
	}
	else
	{
		agent.SetEnergy(agent.GetEnergy() + deltaTime * 4.0f);
	}
}

void StayHomeAndRestState::Exit(Gatherer& agent)
{
	agent.SetLocation(Location::Home);
}

void StayHomeAndRestState::DebugUI()
{
}

//==========================================================================

void GoToGatherSpotState::Enter(Gatherer& agent)
{

}

void GoToGatherSpotState::Update(Gatherer& agent, float deltaTime)
{
	X::Math::Vector2 agentPos = agent.GetTileMap()->GetTilePosition(agent.position);
	X::Math::Vector2 gatherPos = agent.GetTileMap()->GetTilePosition(agent.GetGatherSpot());
	if (X::Math::DistanceSqr(agentPos, gatherPos) < 5.0f)
	{
		agent.Gather();
	}
	agent.SetEnergy(agent.GetEnergy() - deltaTime);
	if (agent.GetEnergy() < 0.0f)
	{
		agent.GoHome();
	}
}

void GoToGatherSpotState::Exit(Gatherer& agent)
{
	agent.SetLocation(Location::GatherSpot);
}

void GoToGatherSpotState::DebugUI()
{
}

//==========================================================================

void GatherResourcesState::Enter(Gatherer& agent)
{
}

void GatherResourcesState::Update(Gatherer& agent, float deltaTime)
{
	if (agent.GetHasResource())
	{
		agent.GoHome();
	}
	agent.SetEnergy(agent.GetEnergy() - deltaTime);
	if (agent.GetEnergy() < 0.0f)
	{
		agent.GoHome();
	}
}

void GatherResourcesState::Exit(Gatherer& agent)
{
}

void GatherResourcesState::DebugUI()
{
}

void GoHomeState::Enter(Gatherer& agent)
{
}

void GoHomeState::Update(Gatherer& agent, float deltaTime)
{
	X::Math::Vector2 agentPos = agent.GetTileMap()->GetTilePosition(agent.position);
	X::Math::Vector2 agentHome = agent.GetTileMap()->GetTilePosition(agent.GetGathererHome());
	if (X::Math::DistanceSqr(agentPos, agentHome) < 25.0f)
	{
		agent.StayHome();
	}
	agent.SetEnergy(agent.GetEnergy() - deltaTime);
}

void GoHomeState::Exit(Gatherer& agent)
{
	agent.SetLocation(Location::Home);
}

void GoHomeState::DebugUI()
{
}
