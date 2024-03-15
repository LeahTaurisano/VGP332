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
	GatherResources
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
	void Gather();
	void SetTarget(Entity* target);
	Entity* GetTarget() { return mTarget; }

	const AI::PerceptionModule* GetPerception() const { return mPerceptionModule.get(); }

	void InitializeStates();
	void ChangeState(GathererState state);
	void SetLocation(Location location) { mLocation = location; }
	Location GetLocation() { return mLocation; }

	void SetTileMap(TileMap* tileMap) { mTileMap = tileMap; }

private:
	std::unique_ptr<AI::PerceptionModule> mPerceptionModule;
	std::unique_ptr<AI::SteeringModule> mSteeringModule;
	std::unique_ptr<AI::DecisionModule<Gatherer>> mDecisionModule;

	AI::StateMachine<Gatherer> mStateMachine;
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
	X::Math::Vector2 gatherSpot = { 27.0f, 15.0f }; //grid dimensions

	float gatherViewRange = 250;
	float gatherViewAngle = 180;
	float hunterViewRange = 150;
	float hunterViewAngle = 50;

	bool hasResource = false;
	bool showDebug = false;
};