#include "MovementSensor.h"

using namespace AI;

void MovementSensor::Update(Agent& agent, MemoryRecords& memory, float deltaTime)
{
	const float viewRangeSqr = viewRange * viewRange;
	const EntityPtrs entities = agent.world.GetEntities();
	for (const Entity* entity : entities)
	{
		//ignore self
		if (entity == &agent)
		{
			continue;
		}

		//filter by type
		if (targetType != AgentType::Invalid && static_cast<uint32_t>(targetType) != entity->GetTypeId())
		{
			continue;
		}

		//check if target is in view distance
		X::Math::Vector2 posToTarget = entity->position - agent.position;
		if (X::Math::MagnitudeSqr(posToTarget) > viewRangeSqr)
		{
			continue;
		}

		// check the view angle
		X::Math::Vector2 dirToTarget = X::Math::Normalize(posToTarget);
		float dot = X::Math::Dot(agent.heading, dirToTarget);
		float angleToTarget = acos(dot);
		if (angleToTarget > viewHalfAngle)
		{
			continue;
		}

		// line of sight
		X::Math::LineSegment lineToTarget(agent.position, entity->position);
		if (!agent.world.HasLineOfSight(lineToTarget))
		{
			X::DrawScreenLine(agent.position, entity->position, X::Colors::Red);
			continue;
		}

		//have I seen this before
		auto iter = std::find_if(memory.begin(), memory.end(), 
			[&](const MemoryRecord& m)
			{
				return entity->GetUniqueId() == m.uniqueId;
			});
		if (iter != memory.end())
		{
			X::Math::Vector2 lastPos = iter->GetProperty<X::Math::Vector2>("lastSeenPosition");
			if (lastPos.x != entity->position.x && lastPos.y != entity->position.y)
			{
				iter->properties["moving"] = true;
			}
			else
			{
				iter->properties["moving"] = false;
			}
			iter->properties["lastSeenPosition"] = entity->position;
			iter->lastRecordedTime = X::GetTime();
		}
		else
		{
			auto& newRecord = memory.emplace_back();
			newRecord.uniqueId = entity->GetUniqueId();
			newRecord.properties["lastSeenPosition"] = entity->position;
			newRecord.properties["type"] = static_cast<int>(entity->GetTypeId());
			newRecord.properties["moving"] = false;
			newRecord.lastRecordedTime = X::GetTime();
		}
	}

	// debug info
	X::Math::Vector2 fovStart = X::Math::Rotate(agent.heading * viewRange, -viewHalfAngle);
	X::Math::Vector2 fovEnd = X::Math::Rotate(agent.heading * viewRange, viewHalfAngle);
	X::DrawScreenLine(agent.position, agent.position + fovStart, X::Colors::Yellow);
	X::DrawScreenLine(agent.position, agent.position + fovEnd, X::Colors::Yellow);

	float angle = atan2(agent.heading.y, agent.heading.x);
	X::DrawScreenArc(agent.position, viewRange, angle - viewHalfAngle, angle + viewHalfAngle, X::Colors::Yellow);
}