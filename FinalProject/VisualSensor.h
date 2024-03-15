#pragma once

#include <AI.h>

#include "TypeIds.h"

class VisualSensor : public AI::Sensor
{
public:
	virtual void Update(AI::Agent& agent, AI::MemoryRecords& memory, float deltaTime) override;
	void SetDrawDebug(bool state);
	bool GetDrawDebug();

	AgentType targetType = AgentType::Invalid;
	float viewRange = 0.0f;
	float viewHalfAngle = 0.0f;

	bool drawDebug = true;
};