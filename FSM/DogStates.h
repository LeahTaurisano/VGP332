#pragma once

#include <AI.h>
#include "Dog.h"

class GoToBedAndSleepUntilRestedState : public AI::State<Dog>
{
public:
	void Enter(Dog& agent) override;
	void Update(Dog& agent, float deltaTime) override;
	void Exit(Dog& agent) override;
	void DebugUI() override;
};

class EnterKitchenAndSatiateHungerState : public AI::State<Dog>
{
public:
	void Enter(Dog& agent) override;
	void Update(Dog& agent, float deltaTime) override;
	void Exit(Dog& agent) override;
	void DebugUI() override;
};

class EnterKitchenAndQuenchThirstState : public AI::State<Dog>
{
public:
	void Enter(Dog& agent) override;
	void Update(Dog& agent, float deltaTime) override;
	void Exit(Dog& agent) override;
	void DebugUI() override;
};

class GoOutsideAndUseBathroomState : public AI::State<Dog>
{
public:
	void Enter(Dog& agent) override;
	void Update(Dog& agent, float deltaTime) override;
	void Exit(Dog& agent) override;
	void DebugUI() override;
};

class GoToLivingRoomAndPlayState : public AI::State<Dog>
{
public:
	void Enter(Dog& agent) override;
	void Update(Dog& agent, float deltaTime) override;
	void Exit(Dog& agent) override;
	void DebugUI() override;
};