#pragma once

#include <AI.h>
#include "Gatherer.h"

class StayHomeAndRestState : public AI::State<Gatherer>
{
public:
	void Enter(Gatherer& agent) override;
	void Update(Gatherer& agent, float deltaTime) override;
	void Exit(Gatherer& agent) override;
	void DebugUI() override;
};

class GoToGatherSpotState : public AI::State<Gatherer>
{
public:
	void Enter(Gatherer& agent) override;
	void Update(Gatherer& agent, float deltaTime) override;
	void Exit(Gatherer& agent) override;
	void DebugUI() override;
};

class GatherResourcesState : public AI::State<Gatherer>
{
public:
	void Enter(Gatherer& agent) override;
	void Update(Gatherer& agent, float deltaTime) override;
	void Exit(Gatherer& agent) override;
	void DebugUI() override;
};

class GoHomeState : public AI::State<Gatherer>
{
public:
	void Enter(Gatherer& agent) override;
	void Update(Gatherer& agent, float deltaTime) override;
	void Exit(Gatherer& agent) override;
	void DebugUI() override;
};