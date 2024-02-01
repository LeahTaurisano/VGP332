#pragma once

#include <AI.h>

enum class DogLocation
{
	Bed,
	Kitchen,
	Outside,
	LivingRoom
};

enum class DogState
{
	SleepUntilRested,
	SatiateHunger,
	QuenchThirst,
	UseTheBathroom,
	PlayWithToys
};

class Dog
{
public:
	Dog() = default;
	void Initialize();
	void Terminate();
	void Update(float deltaTime);
	void ChangeState(DogState newState);
	void DebugUI();

	//check Dog status
	DogLocation GetLocation() const;
	bool IsThirsty() const;
	bool IsHungry() const;
	bool IsTired() const;
	bool IsBladderFull() const;

	//update status functions
	void SetLocation(DogLocation location);
	void IncreaseFatigue();
	void IncreaseHungerAndThirst();
	void IncrementBladder();
	void SetHasEaten(bool eaten);
	void SetHasDrank(bool drank);

	void ResetThirst();
	void ResetFatigue();
	void ResetHunger();
	void ResetBladder();

private:
	AI::StateMachine<Dog> mStateMachine;
	DogLocation mLocation;
	int mBladder;
	int mThirst;
	int mHunger;
	int mFatigue;
	bool mHasEaten;
	bool mHasDrank;
};