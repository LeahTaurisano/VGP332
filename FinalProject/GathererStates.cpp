#include "GathererStates.h"

void StayHomeAndRestState::Enter(Gatherer& agent)
{

}

void StayHomeAndRestState::Update(Gatherer& agent, float deltaTime)
{
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
	agent.Gather();
}

void GoToGatherSpotState::Update(Gatherer& agent, float deltaTime)
{
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
}

void GatherResourcesState::Exit(Gatherer& agent)
{
}

void GatherResourcesState::DebugUI()
{
}
