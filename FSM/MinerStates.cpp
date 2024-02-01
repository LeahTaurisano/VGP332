#include "MinerStates.h"
#include <ImGui/Inc/imgui.h>

void GoHomeAndSleepUntilRestedState::Enter(Miner& agent)
{
	agent.SetLocation(Location::Home);
}

void GoHomeAndSleepUntilRestedState::Update(Miner& agent, float deltaTime)
{
	agent.ResetFatigue();
	if (agent.IsRested())
	{
		if (!agent.IsWealthy())
		{
			agent.ChangeState(MinerState::EnterMineAndDigForNugget);
		}
		else
		{
			//we are done
		}
	}
}

void GoHomeAndSleepUntilRestedState::Exit(Miner& agent)
{
}

void GoHomeAndSleepUntilRestedState::DebugUI()
{
	ImGui::Text("GoHomeAndSleepUntilRestedState: Miner is resting");
}

void EnterMineAndDigForNuggetState::Enter(Miner& agent)
{
	agent.SetLocation(Location::Mine);
}

void EnterMineAndDigForNuggetState::Update(Miner& agent, float deltaTime)
{
	agent.IncreaseFatigue();
	agent.AddGoldCarried(1);
	if (agent.IsPocketFull())
	{
		agent.ChangeState(MinerState::VisitBankAndDepositGold);
	}
	else if (agent.IsThirsty())
	{
		agent.ChangeState(MinerState::QuenchThirst);
	}
}

void EnterMineAndDigForNuggetState::Exit(Miner& agent)
{
}

void EnterMineAndDigForNuggetState::DebugUI()
{
	ImGui::Text("GoHomeAndSleepUntilRestedState: Miner is digging for gold");
}

void QuenchThirstState::Enter(Miner& agent)
{
	agent.SetLocation(Location::Saloon);
}

void QuenchThirstState::Update(Miner& agent, float deltaTime)
{
	agent.ResetThirst();
	if (!agent.IsThirsty())
	{
		agent.ChangeState(MinerState::EnterMineAndDigForNugget);
	}
}

void QuenchThirstState::Exit(Miner& agent)
{
}

void QuenchThirstState::DebugUI()
{
	ImGui::Text("QuenchThirstState: Miner is drinking");
}

void VisitBankAndDepositGoldState::Enter(Miner& agent)
{
	agent.SetLocation(Location::Bank);
}

void VisitBankAndDepositGoldState::Update(Miner& agent, float deltaTime)
{
	agent.AddGoldToBank();
	if (!agent.IsRested())
	{
		agent.ChangeState(MinerState::GoHomeAndSleepUntilRested);
	}
	else
	{
		agent.ChangeState(MinerState::EnterMineAndDigForNugget);
	}
}

void VisitBankAndDepositGoldState::Exit(Miner& agent)
{
}

void VisitBankAndDepositGoldState::DebugUI()
{
	ImGui::Text("VisitBankAndDepositGoldState: Miner is depositing gold in the bank");
}
