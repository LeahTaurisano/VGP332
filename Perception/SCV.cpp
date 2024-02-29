#include "SCV.h"

#include "TypeIds.h"
#include "VisualSensor.h"
#include "MovementSensor.h"

extern float viewRange;
extern float viewAngle;

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
		case AgentType::SCV:
		{
			X::Math::Vector2 lastSeenPos = record.GetProperty<X::Math::Vector2>("lastSeenPosition");
			float distance = X::Math::Distance(agent.position, lastSeenPos);
			float distanceScore = std::max(500.0f - distance, 0.0f);
			score = distanceScore;
		}
		break;
		case AgentType::Mineral:
		{
			X::Math::Vector2 lastSeenPos = record.GetProperty<X::Math::Vector2>("lastSeenPosition");
			float distance = X::Math::Distance(agent.position, lastSeenPos);
			float distanceScore = std::max(1000.0f - distance, 0.0f);
			score = distanceScore;
		}
		break;
		default:
			break;
		}
		return score;
	}
}

extern float wanderJitter;
extern float wanderRadius;
extern float wanderDistance;

SCV::SCV(AI::AIWorld& world)
	: Agent(world, static_cast<uint32_t>(AgentType::SCV))
{
}

void SCV::Load()
{
	mPerceptionModule = std::make_unique<AI::PerceptionModule>(*this, ComputeImportance);
	mPerceptionModule->SetMemorySpan(2.0f);
	mVisualSensor = mPerceptionModule->AddSensor<VisualSensor>();
	mVisualSensor->targetType = AgentType::Mineral;
	mVisualSensor2 = mPerceptionModule->AddSensor<VisualSensor>();
	mVisualSensor2->targetType = AgentType::SCV;
	mMovementSensor = mPerceptionModule->AddSensor<MovementSensor>();
	mMovementSensor->targetType = AgentType::Invalid;

	mSteeringModule = std::make_unique<AI::SteeringModule>(*this);
	mSeekBehavior = mSteeringModule->AddBehavior<AI::SeekBehavior>();
	mFleeBehavior = mSteeringModule->AddBehavior<AI::FleeBehavior>();
	mArriveBehavior = mSteeringModule->AddBehavior<AI::ArriveBehavior>();
	mWanderBehavior = mSteeringModule->AddBehavior<AI::WanderBehavior>();
	mPursuitBehavior = mSteeringModule->AddBehavior<AI::PursuitBehavior>();
	mSeparationBehavior = mSteeringModule->AddBehavior<AI::SeparationBehavior>();
	mAlignmentBehavior = mSteeringModule->AddBehavior<AI::AlignmentBehavior>();
	mCohesionBehavior = mSteeringModule->AddBehavior<AI::CohesionBehavior>();

	const float screenWidth = static_cast<float>(X::GetScreenWidth());
	const float screenHeight = static_cast<float>(X::GetScreenHeight());
	destination = { screenWidth * 0.5f, screenHeight * 0.5f };

	for (int i = 0; i < mTextureIds.size(); ++i)
	{
		char name[128];
		sprintf_s(name, "scv_%02i.png", i + 1);
		mTextureIds[i] = X::LoadTexture(name);
	}
}

void SCV::Unload()
{
}

void SCV::Update(float deltaTime)
{
	mVisualSensor->viewRange = viewRange;
	mVisualSensor->viewHalfAngle = viewAngle * X::Math::kDegToRad;
	mVisualSensor2->viewRange = viewRange * 0.5f;
	mVisualSensor2->viewHalfAngle = viewAngle * X::Math::kDegToRad;
	mMovementSensor->viewRange = viewRange * 1.5f;
	mMovementSensor->viewHalfAngle = viewAngle * X::Math::kDegToRad * 1.25;
	mPerceptionModule->Update(deltaTime);

	if (mWanderBehavior != nullptr)
	{
		mWanderBehavior->Setup(wanderRadius, wanderDistance, wanderJitter);
	}
	const X::Math::Vector2 force = mSteeringModule->Calculate();
	const X::Math::Vector2 acceleration = force / mass;
	velocity += acceleration * deltaTime;

	position += velocity * deltaTime;

	if (X::Math::MagnitudeSqr(velocity) > 1.0f)
	{
		heading = X::Math::Normalize(velocity);
	}

	const float screenWidth = static_cast<float>(X::GetScreenWidth());
	const float screenHeight = static_cast<float>(X::GetScreenHeight());
	if (position.x < 0.0f)
	{
		position.x += screenWidth;
	}
	if (position.x >= screenWidth)
	{
		position.x -= screenWidth;
	}
	if (position.y < 0.0f)
	{			 
		position.y += screenHeight;
	}			 
	if (position.y >= screenHeight)
	{			 
		position.y -= screenHeight;
	}

	const auto& memoryRecords = mPerceptionModule->GetMemoryRecords();
	for (auto& memory : memoryRecords)
	{
		if (!memory.GetProperty<bool>("moving", false))
		{
			X::Math::Vector2 pos = memory.GetProperty<X::Math::Vector2>("lastSeenPosition");
			X::DrawScreenLine(position, pos, X::Colors::White);

			std::string score = std::to_string(memory.importance);
			X::DrawScreenText(score.c_str(), pos.x, pos.y, 12.0f, X::Colors::White);
		}
		else
		{
			X::Math::Vector2 pos = memory.GetProperty<X::Math::Vector2>("lastSeenPosition");
			X::DrawScreenLine(position, pos, X::Colors::Orange);

			std::string score = std::to_string(memory.importance * 2);
			X::DrawScreenText(score.c_str(), pos.x, pos.y, 12.0f, X::Colors::Orange);
		}
	}
}

void SCV::Render()
{
	const float angle = atan2(-heading.x, heading.y) + X::Math::kPi;
	const float percent = angle / X::Math::kTwoPi;
	const int frame = static_cast<int>(percent * mTextureIds.size()) % mTextureIds.size();
	X::DrawSprite(mTextureIds[frame], position);
}

void SCV::ShowDebug(bool debug)
{
	mSeekBehavior->ShowDebug(debug);
	mFleeBehavior->ShowDebug(debug);
	mArriveBehavior->ShowDebug(debug);
	mWanderBehavior->ShowDebug(debug);
	mPursuitBehavior->ShowDebug(debug);
	mSeparationBehavior->ShowDebug(debug);
	mAlignmentBehavior->ShowDebug(debug);
	mCohesionBehavior->ShowDebug(debug);
}

void SCV::SetSeek(bool active)
{
	mSeekBehavior->SetActive(active);
}
void SCV::SetFlee(bool active)
{
	mFleeBehavior->SetActive(active);
}
void SCV::SetArrive(bool active)
{
	mArriveBehavior->SetActive(active);
}
void SCV::SetWander(bool active)
{
	mWanderBehavior->SetActive(active);
}
void SCV::SetPursuit(bool active)
{
	mPursuitBehavior->SetActive(active);
}
void SCV::SetSeparation(bool active)
{
	mSeparationBehavior->SetActive(active);
}
void SCV::SetAlignment(bool active)
{
	mAlignmentBehavior->SetActive(active);
}
void SCV::SetCohesion(bool active)
{
	mCohesionBehavior->SetActive(active);
}

void SCV::SetSeekWeight(float weight)
{
	mSeekBehavior->SetWeight(weight);
}

void SCV::SetFleeWeight(float weight)
{
	mFleeBehavior->SetWeight(weight);
}

void SCV::SetArriveWeight(float weight)
{
	mArriveBehavior->SetWeight(weight);
}

void SCV::SetWanderWeight(float weight)
{
	mWanderBehavior->SetWeight(weight);
}

void SCV::SetPursuitWeight(float weight)
{
	mPursuitBehavior->SetWeight(weight);
}

void SCV::SetSeparationWeight(float weight)
{
	mSeparationBehavior->SetWeight(weight);
}

void SCV::SetAlignmentWeight(float weight)
{
	mAlignmentBehavior->SetWeight(weight);
}

void SCV::SetCohesionWeight(float weight)
{
	mCohesionBehavior->SetWeight(weight);
}
