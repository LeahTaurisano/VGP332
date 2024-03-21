#include "Gatherer.h"

#include "TypeIds.h"
#include "VisualSensor.h"
#include "GathererStrategy.h"
#include "GathererForageStrategy.h"
#include "GathererGoToResourceSpotStrategy.h"
#include "GathererGoToResourceStrategy.h"
#include "GathererGoHomeStrategy.h"
#include "GathererHarvestStrategy.h"
#include "GathererStates.h"

extern float wanderJitter;
extern float wanderRadius;
extern float wanderDistance;

namespace
{
	float ComputeImportance(const AI::Agent& agent, const AI::MemoryRecord& record)
	{
		float score = 0.0f;
		AgentType entityType = static_cast<AgentType>(record.GetProperty<int>("type"));
		switch (entityType)
		{
		case AgentType::Invalid:
			score = 0.0f;
			break;
		case AgentType::Hunter:
		{
			score = 0.0f;
		}
		break;
		case AgentType::Resource:
		{
			int health = record.GetProperty<int>("health", 0);
			if (health > 0)
			{
				X::Math::Vector2 lastSeenPos = record.GetProperty<X::Math::Vector2>("lastSeenPosition");
				float distance = X::Math::Distance(agent.position, lastSeenPos);
				float distanceScore = std::max(1000.0f - distance, 0.0f);
				score = distanceScore;
			}
			else
			{
				score = 0.0f;
			}
		}
		break;
		default:
			break;
		}
		return score;
	}
}

Gatherer::Gatherer(AI::AIWorld& world)
	: Agent(world, static_cast<uint32_t>(AgentType::Gatherer))
{
}

void Gatherer::Load()
{
	mPerceptionModule = std::make_unique<AI::PerceptionModule>(*this, ComputeImportance);
	mPerceptionModule->SetMemorySpan(3.0f);
	mVisualSensor = mPerceptionModule->AddSensor<VisualSensor>();
	mVisualSensor->targetType = AgentType::Resource;
	mHunterSensor = mPerceptionModule->AddSensor<VisualSensor>();
	mHunterSensor->targetType = AgentType::Hunter;
	

	mSteeringModule = std::make_unique<AI::SteeringModule>(*this);
	mSeekBehavior = mSteeringModule->AddBehavior<AI::SeekBehavior>();
	mArriveBehavior = mSteeringModule->AddBehavior<AI::ArriveBehavior>();
	mWanderBehavior = mSteeringModule->AddBehavior<AI::WanderBehavior>();

	mDecisionModule = std::make_unique<AI::DecisionModule<Gatherer>>(*this);
	auto goToResourceSpotStrategy = mDecisionModule->AddStrategy<GathererGoToResourceSpotStrategy>();
	goToResourceSpotStrategy->SetPerception(mPerceptionModule.get());
	goToResourceSpotStrategy->SetDestination(mTileMap->GetTilePosition(gatherSpot));
	goToResourceSpotStrategy->SetTileMap(mTileMap);
	auto goHomeStrategy = mDecisionModule->AddStrategy<GathererGoHomeStrategy>();
	goHomeStrategy->SetPerception(mPerceptionModule.get());
	goHomeStrategy->SetDestination(mTileMap->GetTilePosition(mGathererHome));
	goHomeStrategy->SetTileMap(mTileMap);
	mDecisionModule->AddStrategy<GathererHarvestStrategy>();
	auto goToResourceStrategy = mDecisionModule->AddStrategy<GathererGoToResourceStrategy>();
	goToResourceStrategy->SetPerception(mPerceptionModule.get());
	goToResourceStrategy->SetTileMap(mTileMap);

	for (int i = 0; i < mTextureIds.size(); ++i)
	{
		char name[128];
		sprintf_s(name, "interceptor_%02i.png", i + 1);
		mTextureIds[i] = X::LoadTexture(name);
	}
}

void Gatherer::Unload()
{
}

void Gatherer::Update(float deltaTime)
{
	mVisualSensor->viewRange = gatherViewRange;
	mVisualSensor->viewHalfAngle = gatherViewAngle * X::Math::kDegToRad;

	mHunterSensor->viewRange = hunterViewRange;
	mHunterSensor->viewHalfAngle = hunterViewAngle * X::Math::kDegToRad;

	mPerceptionModule->Update(deltaTime);
	mDecisionModule->Update();
	mStateMachine.Update(deltaTime);

	mWanderBehavior->Setup(wanderRadius, wanderDistance, wanderJitter);

	const X::Math::Vector2 force = mSteeringModule->Calculate();
	const X::Math::Vector2 acceleration = force / mass;
	velocity += acceleration * deltaTime;
	if (X::Math::MagnitudeSqr(velocity) > 1.0f)
	{
		heading = X::Math::Normalize(velocity);
	}

	position += velocity * deltaTime;

	if (showDebug)
	{
		const auto& memoryRecords = mPerceptionModule->GetMemoryRecords();
		for (auto& memory : memoryRecords)
		{
			X::Math::Vector2 pos = memory.GetProperty<X::Math::Vector2>("lastSeenPosition");
			X::DrawScreenLine(position, pos, X::Colors::White);

			std::string score = std::to_string(memory.importance);
			X::DrawScreenText(score.c_str(), pos.x, pos.y, 12.0f, X::Colors::White);
		}
	}
}

void Gatherer::Render()
{
	const float angle = atan2(-heading.x, heading.y) + X::Math::kPi;
	const float percent = angle / X::Math::kTwoPi;
	const int frame = static_cast<int>(percent * mTextureIds.size()) % mTextureIds.size();
	X::DrawSprite(mTextureIds[frame], position);
}

void Gatherer::ShowDebug(bool debug)
{
	mSeekBehavior->ShowDebug(debug);
	mWanderBehavior->ShowDebug(debug);
	mVisualSensor->SetDrawDebug(debug);
	mHunterSensor->SetDrawDebug(debug);
}

void Gatherer::SetSeek(bool active)
{
	mSeekBehavior->SetActive(active);
}

void Gatherer::SetArrive(bool active)
{
	mArriveBehavior->SetActive(active);
}

void Gatherer::SetWander(bool active)
{
	mWanderBehavior->SetActive(active);
}

void Gatherer::GoToGather()
{
	ChangeState(GathererState::GoToGatherSpot);
}

void Gatherer::Gather()
{
	ChangeState(GathererState::GatherResources);
}

void Gatherer::GoHome()
{
	ChangeState(GathererState::GoHome);
}

void Gatherer::StayHome()
{
	ChangeState(GathererState::StayHomeAndRest);
}

void Gatherer::SetTarget(Entity* target)
{
	mTarget = target;
}

void Gatherer::InitializeStates()
{
	mLocation = Location::Home;

	mStateMachine.Initialize(this);
	mStateMachine.AddState<StayHomeAndRestState>();
	mStateMachine.AddState<GoToGatherSpotState>();
	mStateMachine.AddState<GatherResourcesState>();
	mStateMachine.AddState<GoHomeState>();
	ChangeState(GathererState::StayHomeAndRest);
}

void Gatherer::ChangeState(GathererState state)
{
	mStateMachine.ChangeState((int)state);
	mCurrentState = state;
}

void Gatherer::SetShowDebug(bool state)
{
	showDebug = state;
}
