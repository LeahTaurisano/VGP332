#pragma once

#include <AI.h>
#include "TileMap.h"

class VisualSensorHunter;

enum class HunterLocation
{
	Home,
	HuntingSpot
};

enum class HunterState
{
	StayHomeAndRest,
	GoToHuntingSpot,
	HuntGatherers,
	GoHome
};

class Hunter : public AI::Agent
{
public:
	Hunter(AI::AIWorld& world);

	void Load();
	void Unload();

	void Update(float deltaTime);
	void Render();

	void ShowDebug(bool debug);
	void SetShowDebug(bool state);

	X::Math::Vector2 GetHunterHome() { return mHunterHome; }

	void SetSeek(bool active);
	void SetArrive(bool active);
	void SetWander(bool active);
	void GoToHunt();
	void Hunt();
	void GoHome();
	void StayHome();
	void SetTarget(Entity* target);
	Entity* GetTarget() { return mTarget; }

	const AI::PerceptionModule* GetPerception() const { return mPerceptionModule.get(); }

	void InitializeStates();
	void ChangeState(HunterState state);
	HunterState GetCurrentState() { return mCurrentState; }
	void SetLocation(HunterLocation location) { mLocation = location; }
	HunterLocation GetLocation() { return mLocation; }

	void SetTileMap(TileMap* tileMap) { mTileMap = tileMap; }
	TileMap* GetTileMap() { return mTileMap; }
	X::Math::Vector2 GetHuntingSpot(){ return huntingSpot; }

	void HeldGatherer(bool held) { mHasGatherer = held; }
	bool GetHasGatherer() { return mHasGatherer; }
	void SetDepositedGatherer(bool deposited) { mDepositedGatherer = deposited; }
	bool GetDepositedGatherer() { return mDepositedGatherer; }

	float GetEnergyMax() const { return mHunterEnergyMax; }
	float GetEnergy() const { return mHunterEnergy; }
	void SetEnergy(float energy) { mHunterEnergy = energy; }

private:
	std::unique_ptr<AI::PerceptionModule> mPerceptionModule;
	std::unique_ptr<AI::SteeringModule> mSteeringModule;
	std::unique_ptr<AI::DecisionModule<Hunter>> mDecisionModule;

	AI::StateMachine<Hunter> mStateMachine;
	HunterState mCurrentState;
	HunterLocation mLocation;

	VisualSensorHunter* mVisualSensor = nullptr;
	AI::SeekBehavior* mSeekBehavior = nullptr;
	AI::ArriveBehavior* mArriveBehavior = nullptr;
	AI::WanderBehavior* mWanderBehavior = nullptr;
	std::array<X::TextureId, 16> mTextureIds;

	Entity* mTarget;
	TileMap* mTileMap;

	X::Math::Vector2 mHunterHome = X::RandomVector2({ 100.0f, 100.0f }, { 300.0f, 300.0f }); //actual screen dimensions
	X::Math::Vector2 huntingSpot = X::RandomVector2({ 700.0f, 500.0f }, { 1000.0f, 650.0f });

	float huntViewRange = 200;
	float huntViewAngle = 180;

	float mHunterEnergyMax = 30.0f;
	float mHunterEnergy = mHunterEnergyMax;

	bool mHasGatherer = false;
	bool mDepositedGatherer = false;
	bool showDebug = false;
};