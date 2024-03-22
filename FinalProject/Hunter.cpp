#include "Hunter.h"

#include "TypeIds.h"
#include "VisualSensorHunter.h"
#include "HunterGoToHuntingSpotStrategy.h"
#include "HunterGoToGathererStrategy.h"
#include "HunterGoHomeStrategy.h"
#include "HunterHarvestStrategy.h"
#include "HunterStates.h"

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
		case AgentType::Resource:
		{
			score = 0.0f;
		}
		break;
		case AgentType::Gatherer:
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

Hunter::Hunter(AI::AIWorld& world)
	: Agent(world, static_cast<uint32_t>(AgentType::Hunter))
{
}

void Hunter::Load()
{
	mPerceptionModule = std::make_unique<AI::PerceptionModule>(*this, ComputeImportance);
	mPerceptionModule->SetMemorySpan(3.0f);
	mVisualSensor = mPerceptionModule->AddSensor<VisualSensorHunter>();
	mVisualSensor->targetType = AgentType::Gatherer;

	mSteeringModule = std::make_unique<AI::SteeringModule>(*this);
	mSeekBehavior = mSteeringModule->AddBehavior<AI::SeekBehavior>();
	mArriveBehavior = mSteeringModule->AddBehavior<AI::ArriveBehavior>();
	mWanderBehavior = mSteeringModule->AddBehavior<AI::WanderBehavior>();

	mDecisionModule = std::make_unique<AI::DecisionModule<Hunter>>(*this);
	auto goToHuntingSpotStrategy = mDecisionModule->AddStrategy<HunterGoToHuntingSpotStrategy>();
	goToHuntingSpotStrategy->SetPerception(mPerceptionModule.get());
	goToHuntingSpotStrategy->SetDestination(mTileMap->GetTilePosition(huntingSpot));
	goToHuntingSpotStrategy->SetTileMap(mTileMap);
	auto goHomeStrategy = mDecisionModule->AddStrategy<HunterGoHomeStrategy>();
	goHomeStrategy->SetPerception(mPerceptionModule.get());
	goHomeStrategy->SetDestination(mTileMap->GetTilePosition(mHunterHome));
	goHomeStrategy->SetTileMap(mTileMap);
	mDecisionModule->AddStrategy<HunterHarvestStrategy>();
	auto goToResourceStrategy = mDecisionModule->AddStrategy<HunterGoToGathererStrategy>();
	goToResourceStrategy->SetPerception(mPerceptionModule.get());
	goToResourceStrategy->SetTileMap(mTileMap);

	for (int i = 0; i < mTextureIds.size(); ++i)
	{
		char name[128];
		sprintf_s(name, "scv_%02i.png", i + 1);
		mTextureIds[i] = X::LoadTexture(name);
	}
}

void Hunter::Unload()
{
}

void Hunter::Update(float deltaTime)
{
	mVisualSensor->viewRange = huntViewRange;
	mVisualSensor->viewHalfAngle = huntViewAngle * X::Math::kDegToRad;

	mPerceptionModule->Update(deltaTime);
	mDecisionModule->Update();
	mStateMachine.Update(deltaTime);

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

void Hunter::Render()
{
	const float angle = atan2(-heading.x, heading.y) + X::Math::kPi;
	const float percent = angle / X::Math::kTwoPi;
	const int frame = static_cast<int>(percent * mTextureIds.size()) % mTextureIds.size();
	X::DrawSprite(mTextureIds[frame], position);
}

void Hunter::ShowDebug(bool debug)
{
	mSeekBehavior->ShowDebug(debug);
	mWanderBehavior->ShowDebug(debug);
	mVisualSensor->SetDrawDebug(debug);
}

void Hunter::SetSeek(bool active)
{
	mSeekBehavior->SetActive(active);
}

void Hunter::SetArrive(bool active)
{
	mArriveBehavior->SetActive(active);
}

void Hunter::SetWander(bool active)
{
	mWanderBehavior->SetActive(active);
}

void Hunter::GoToHunt()
{
	ChangeState(HunterState::GoToHuntingSpot);
}

void Hunter::Hunt()
{
	ChangeState(HunterState::HuntGatherers);
}

void Hunter::GoHome()
{
	ChangeState(HunterState::GoHome);
}

void Hunter::StayHome()
{
	ChangeState(HunterState::StayHomeAndRest);
}

void Hunter::SetTarget(Entity* target)
{
	mTarget = target;
}

void Hunter::InitializeStates()
{
	mLocation = HunterLocation::Home;

	mStateMachine.Initialize(this);
	mStateMachine.AddState<HunterStayHomeAndRestState>();
	mStateMachine.AddState<GoToHuntingSpotState>();
	mStateMachine.AddState<HuntGatherersState>();
	mStateMachine.AddState<HunterGoHomeState>();
	ChangeState(HunterState::StayHomeAndRest);
}

void Hunter::ChangeState(HunterState state)
{
	mStateMachine.ChangeState((int)state);
	mCurrentState = state;
}

void Hunter::SetShowDebug(bool state)
{
	showDebug = state;
}
