#pragma once

#include <AI.h>
#include "Hunter.h"

class HunterStayHomeAndRestState : public AI::State<Hunter>
{
public:
	void Enter(Hunter& agent) override;
	void Update(Hunter& agent, float deltaTime) override;
	void Exit(Hunter& agent) override;
	void DebugUI() override;
};

class GoToHuntingSpotState : public AI::State<Hunter>
{
public:
	void Enter(Hunter& agent) override;
	void Update(Hunter& agent, float deltaTime) override;
	void Exit(Hunter& agent) override;
	void DebugUI() override;
};

class HuntGatherersState : public AI::State<Hunter>
{
public:
	void Enter(Hunter& agent) override;
	void Update(Hunter& agent, float deltaTime) override;
	void Exit(Hunter& agent) override;
	void DebugUI() override;
};

class HunterGoHomeState : public AI::State<Hunter>
{
public:
	void Enter(Hunter& agent) override;
	void Update(Hunter& agent, float deltaTime) override;
	void Exit(Hunter& agent) override;
	void DebugUI() override;
};