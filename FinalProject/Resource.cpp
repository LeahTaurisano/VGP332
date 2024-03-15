#include "Resource.h"
#include "TypeIds.h"

Resource::Resource(AI::AIWorld& world)
	: Entity(world, static_cast<uint32_t>(AgentType::Resource))
{
}

void Resource::Initialize()
{
	mTextureId = X::LoadTexture("mushroom.png");
	position = X::RandomVector2({ 700.0f, 500.0f }, { 1000.0f, 650.0f });
}

void Resource::Render()
{
	if (mHealth > 0)
	{
		X::DrawSprite(mTextureId, position);
	}
}
