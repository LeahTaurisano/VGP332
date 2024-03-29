#pragma once

#include <AI.h>
#include "TileMap.h"

class VisualSensor;

enum class Location
{
	Home,
	GatherSpot
};

enum class GathererState
{
	StayHomeAndRest,
	GoToGatherSpot,
	GatherResources,
	GoHome
};

class Gatherer : public AI::Agent
{
public:
	Gatherer(AI::AIWorld& world);

	void Load();
	void Unload();

	void Update(float deltaTime);
	void Render();

	void ShowDebug(bool debug);
	void SetShowDebug(bool state);

	X::Math::Vector2 GetGathererHome() { return mGathererHome; }

	void SetSeek(bool active);
	void SetArrive(bool active);
	void SetWander(bool active);
	void GoToGather();
	void Gather();
	void GoHome();
	void StayHome();
	void SetTarget(Entity* target);
	Entity* GetTarget() { return mTarget; }

	const AI::PerceptionModule* GetPerception() const { return mPerceptionModule.get(); }

	void InitializeStates();
	void ChangeState(GathererState state);
	GathererState GetCurrentState() { return mCurrentState; }
	void SetLocation(Location location) { mLocation = location; }
	Location GetLocation() { return mLocation; }

	void SetTileMap(TileMap* tileMap) { mTileMap = tileMap; }
	TileMap* GetTileMap() { return mTileMap; }
	X::Math::Vector2 GetGatherSpot(){ return gatherSpot; }

	void HeldResource(bool held) { mHasResource = held; }
	bool GetHasResource() { return mHasResource; }
	void SetDepositedResource(bool deposited) { mDepositedResource = deposited; }
	bool GetDepositedResource() { return mDepositedResource; }

	int GetHealth() const { return mHealth; }
	void SetHealth(int health) { mHealth = health; }

	float GetEnergyMax() const { return mGathererEnergyMax; }
	float GetEnergy() const { return mGathererEnergy; }
	void SetEnergy(float energy) { mGathererEnergy = energy; }

private:
	std::unique_ptr<AI::PerceptionModule> mPerceptionModule;
	std::unique_ptr<AI::SteeringModule> mSteeringModule;
	std::unique_ptr<AI::DecisionModule<Gatherer>> mDecisionModule;

	AI::StateMachine<Gatherer> mStateMachine;
	GathererState mCurrentState;
	Location mLocation;

	VisualSensor* mVisualSensor = nullptr;
	VisualSensor* mHunterSensor = nullptr;
	AI::SeekBehavior* mSeekBehavior = nullptr;
	AI::ArriveBehavior* mArriveBehavior = nullptr;
	AI::WanderBehavior* mWanderBehavior = nullptr;
	std::array<X::TextureId, 32> mTextureIds;

	Entity* mTarget;
	TileMap* mTileMap;

	X::Math::Vector2 mGathererHome = X::RandomVector2({ 1400.0f, 650.0f }, { 1650.0f, 875.0f }); //actual screen dimensions
	X::Math::Vector2 gatherSpot = X::RandomVector2({ 700.0f, 500.0f }, { 1000.0f, 650.0f });

	float gatherViewRange = 250;
	float gatherViewAngle = 180;
	float hunterViewRange = 150;
	float hunterViewAngle = 50;

	int mHealth = 35;
	float mGathererEnergyMax = 30.0f;
	float mGathererEnergy = mGathererEnergyMax;

	bool mHasResource = false;
	bool mDepositedResource = false;

	bool showDebug = false;
};