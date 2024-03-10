#pragma once

#include <AI.h>
#include "TileMap.h"

class VisualSensor;

class Gatherer : public AI::Agent
{
public:
	Gatherer(AI::AIWorld& world);

	void Load();
	void Unload();

	void Update(float deltaTime);
	void Render();

	void ShowDebug(bool debug);

	void SetSeek(bool active);
	void SetArrive(bool active);
	void SetWander(bool active);
	void SetTargetDestination(const X::Math::Vector2& targetDestination, const TileMap& tileMap);
	void SetTarget(Entity* target);
	Entity* GetTarget() { return mTarget; }

	const AI::PerceptionModule* GetPerception() const { return mPerceptionModule.get(); }

private:
	std::unique_ptr<AI::PerceptionModule> mPerceptionModule;
	std::unique_ptr<AI::SteeringModule> mSteeringModule;
	std::unique_ptr<AI::DecisionModule<Gatherer>> mDecisionModule;

	VisualSensor* mVisualSensor = nullptr;
	AI::SeekBehavior* mSeekBehavior = nullptr;
	AI::ArriveBehavior* mArriveBehavior = nullptr;
	AI::WanderBehavior* mWanderBehavior = nullptr;

	std::array<X::TextureId, 32> mTextureIds;

	Entity* mTarget;
};