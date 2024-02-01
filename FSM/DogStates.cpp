#include "DogStates.h"
#include <ImGui/Inc/imgui.h>

void GoToBedAndSleepUntilRestedState::Enter(Dog& agent)
{
	agent.SetLocation(DogLocation::Bed);
}

void GoToBedAndSleepUntilRestedState::Update(Dog& agent, float deltaTime)
{
	agent.ResetFatigue();
	if (agent.IsThirsty())
	{
		agent.ChangeState(DogState::QuenchThirst);
	}
	else if (agent.IsHungry())
	{
		agent.ChangeState(DogState::SatiateHunger);
	}
	else if (agent.IsBladderFull())
	{
		agent.ChangeState(DogState::UseTheBathroom);
	}
	else
	{
		agent.ChangeState(DogState::PlayWithToys);
	}
	agent.IncreaseHungerAndThirst();
	agent.IncrementBladder();
}

void GoToBedAndSleepUntilRestedState::Exit(Dog& agent)
{
}

void GoToBedAndSleepUntilRestedState::DebugUI()
{
	ImGui::Text("GoToBedAndSleepUntilRestedState: Dog is resting");
}

void EnterKitchenAndSatiateHungerState::Enter(Dog& agent)
{
	agent.SetLocation(DogLocation::Kitchen);
}

void EnterKitchenAndSatiateHungerState::Update(Dog& agent, float deltaTime)
{
	agent.ResetHunger();
	agent.SetHasEaten(true);
	if (agent.IsThirsty())
	{
		agent.ChangeState(DogState::QuenchThirst);
	}
	else if (agent.IsBladderFull())
	{
		agent.ChangeState(DogState::UseTheBathroom);
	}
	else if (agent.IsTired())
	{
		agent.ChangeState(DogState::SleepUntilRested);
	}
	else
	{
		agent.ChangeState(DogState::PlayWithToys);
	}
	agent.IncreaseHungerAndThirst();
	agent.IncrementBladder();
}

void EnterKitchenAndSatiateHungerState::Exit(Dog& agent)
{
}

void EnterKitchenAndSatiateHungerState::DebugUI()
{
	ImGui::Text("EnterKitchenAndSatiateHungerState: Dog is eating");
}

void EnterKitchenAndQuenchThirstState::Enter(Dog& agent)
{
	agent.SetLocation(DogLocation::Kitchen);
}

void EnterKitchenAndQuenchThirstState::Update(Dog& agent, float deltaTime)
{
	agent.ResetThirst();
	agent.SetHasDrank(true);
	if (agent.IsHungry())
	{
		agent.ChangeState(DogState::SatiateHunger);
	}
	else if (agent.IsBladderFull())
	{
		agent.ChangeState(DogState::UseTheBathroom);
	}
	else if (agent.IsTired())
	{
		agent.ChangeState(DogState::SleepUntilRested);
	}
	else
	{
		agent.ChangeState(DogState::PlayWithToys);
	}
	agent.IncreaseHungerAndThirst();
	agent.IncrementBladder();
}

void EnterKitchenAndQuenchThirstState::Exit(Dog& agent)
{
}

void EnterKitchenAndQuenchThirstState::DebugUI()
{
	ImGui::Text("EnterKitchenAndQuenchThirstState: Dog is drinking");
}

void GoOutsideAndUseBathroomState::Enter(Dog& agent)
{
	agent.SetLocation(DogLocation::Outside);
}

void GoOutsideAndUseBathroomState::Update(Dog& agent, float deltaTime)
{
	agent.ResetBladder();
	if (agent.IsHungry())
	{
		agent.ChangeState(DogState::SatiateHunger);
	}
	else if (agent.IsThirsty())
	{
		agent.ChangeState(DogState::QuenchThirst);
	}
	else if (agent.IsTired())
	{
		agent.ChangeState(DogState::SleepUntilRested);
	}
	else
	{
		agent.ChangeState(DogState::PlayWithToys);
	}
	agent.IncreaseHungerAndThirst();
	agent.IncreaseFatigue();
}

void GoOutsideAndUseBathroomState::Exit(Dog& agent)
{
}

void GoOutsideAndUseBathroomState::DebugUI()
{
	ImGui::Text("GoOutsideAndUseBathroomState: Dog is using the bathroom");
}

void GoToLivingRoomAndPlayState::Enter(Dog& agent)
{
	agent.SetLocation(DogLocation::LivingRoom);
}

void GoToLivingRoomAndPlayState::Update(Dog& agent, float deltaTime)
{
	if (agent.IsHungry())
	{
		agent.ChangeState(DogState::SatiateHunger);
	}
	else if (agent.IsThirsty())
	{
		agent.ChangeState(DogState::QuenchThirst);
	}
	else if (agent.IsBladderFull())
	{
		agent.ChangeState(DogState::UseTheBathroom);
	}
	else if (agent.IsTired())
	{
		agent.ChangeState(DogState::SleepUntilRested);
	}
	else
	{
		//keeps playing
	}
	agent.IncreaseHungerAndThirst();
	agent.IncrementBladder();
	agent.IncreaseFatigue();
}

void GoToLivingRoomAndPlayState::Exit(Dog& agent)
{
}

void GoToLivingRoomAndPlayState::DebugUI()
{
	ImGui::Text("GoToLivingRoomAndPlayState: Dog is playing");
}
