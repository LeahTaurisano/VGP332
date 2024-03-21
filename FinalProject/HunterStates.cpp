#include "HunterStates.h"

void HunterStayHomeAndRestState::Enter(Hunter& agent)
{
	if (agent.GetHasGatherer())
	{
		agent.HeldGatherer(false);
		agent.SetDepositedGatherer(true);
	}
}

void HunterStayHomeAndRestState::Update(Hunter& agent, float deltaTime)
{
	if (agent.GetEnergy() > agent.GetEnergyMax())
	{
		agent.GoToHunt();
	}
	else
	{
		agent.SetEnergy(agent.GetEnergy() + deltaTime * 1.0f);
	}
}

void HunterStayHomeAndRestState::Exit(Hunter& agent)
{
	agent.SetLocation(HunterLocation::Home);
}

void HunterStayHomeAndRestState::DebugUI()
{
}

//==========================================================================

void GoToHuntingSpotState::Enter(Hunter& agent)
{

}

void GoToHuntingSpotState::Update(Hunter& agent, float deltaTime)
{
	X::Math::Vector2 agentPos = agent.GetTileMap()->GetTilePosition(agent.position);
	X::Math::Vector2 gatherPos = agent.GetTileMap()->GetTilePosition(agent.GetHuntingSpot());
	if (X::Math::DistanceSqr(agentPos, gatherPos) < 5.0f)
	{
		agent.Hunt();
	}
	agent.SetEnergy(agent.GetEnergy() - deltaTime);
	if (agent.GetEnergy() < 0.0f)
	{
		agent.GoHome();
	}
}

void GoToHuntingSpotState::Exit(Hunter& agent)
{
	agent.SetLocation(HunterLocation::HuntingSpot);
}

void GoToHuntingSpotState::DebugUI()
{
}

//==========================================================================

void HuntGatherersState::Enter(Hunter& agent)
{
}

void HuntGatherersState::Update(Hunter& agent, float deltaTime)
{
	if (agent.GetHasGatherer())
	{
		agent.GoHome();
	}
	agent.SetEnergy(agent.GetEnergy() - deltaTime);
	if (agent.GetEnergy() < 0.0f)
	{
		agent.GoHome();
	}
}

void HuntGatherersState::Exit(Hunter& agent)
{
}

void HuntGatherersState::DebugUI()
{
}

void HunterGoHomeState::Enter(Hunter& agent)
{
}

void HunterGoHomeState::Update(Hunter& agent, float deltaTime)
{
	X::Math::Vector2 agentPos = agent.GetTileMap()->GetTilePosition(agent.position);
	X::Math::Vector2 agentHome = agent.GetTileMap()->GetTilePosition(agent.GetHunterHome());
	if (X::Math::DistanceSqr(agentPos, agentHome) < 25.0f)
	{
		agent.StayHome();
	}
	agent.SetEnergy(agent.GetEnergy() - deltaTime);
}

void HunterGoHomeState::Exit(Hunter& agent)
{
	agent.SetLocation(HunterLocation::Home);
}

void HunterGoHomeState::DebugUI()
{
}
