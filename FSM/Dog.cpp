# include "Dog.h"
#include "DogStates.h"
#include <ImGui/Inc/imgui.h>

void Dog::Initialize()
{
	mLocation = DogLocation::Bed;
	mBladder = 2;
	mThirst = 5;
	mHunger = 2;
	mFatigue = 0;
	mHasEaten = false;
	mHasDrank = false;

	mStateMachine.Initialize(this);
	mStateMachine.AddState<GoToBedAndSleepUntilRestedState>();
	mStateMachine.AddState<EnterKitchenAndSatiateHungerState>();
	mStateMachine.AddState<EnterKitchenAndQuenchThirstState>();
	mStateMachine.AddState<GoOutsideAndUseBathroomState>();
	mStateMachine.AddState<GoToLivingRoomAndPlayState>();
	ChangeState(DogState::SleepUntilRested);
}

void Dog::Terminate()
{
	mStateMachine.Terminate();
}

void Dog::Update(float deltaTime)
{
	mStateMachine.Update(deltaTime);
}

void Dog::ChangeState(DogState newState)
{
	mStateMachine.ChangeState((int)newState);
}

void Dog::DebugUI()
{
	mStateMachine.DebugUI();
	ImGui::Text("Status: HasEaten[%d] HasDrank[%d] Hunger[%d] Thirst[%d] Fatigue[%d] Bladder[%d]", mHasEaten, mHasDrank, mHunger, mThirst, mFatigue, mBladder);
}

DogLocation Dog::GetLocation() const
{
	return mLocation;
}

bool Dog::IsThirsty() const
{
	return mThirst >= 10;
}

bool Dog::IsHungry() const
{
	return mHunger >= 15;
}

bool Dog::IsBladderFull() const
{
	return mBladder >= 25;
}

bool Dog::IsTired() const
{
	return mFatigue >= 50;
}

void Dog::SetLocation(DogLocation location)
{
	mLocation = location;
}

void Dog::SetHasEaten(bool eaten)
{
	mHasEaten = eaten;
}

void Dog::SetHasDrank(bool drank)
{
	mHasDrank = drank;
}

void Dog::IncreaseFatigue()
{
	++mFatigue;
}

void Dog::IncreaseHungerAndThirst()
{
	if (!mHasEaten)
	{
		++mHunger;
	}
	if (!mHasDrank)
	{
		++mThirst;
	}
}

void Dog::IncrementBladder()
{
	++mBladder;
	if (mHasDrank)
	{
		++mBladder;
	}
	if (mHasEaten)
	{
		++mBladder;
	}
}

void Dog::ResetThirst()
{
	mThirst = 0;
}

void Dog::ResetFatigue()
{
	mFatigue = 0;
}

void Dog::ResetHunger()
{
	mHunger = 0;
}

void Dog::ResetBladder()
{
	mBladder = 0;
	mHasDrank = false;
	mHasEaten = false;
}

